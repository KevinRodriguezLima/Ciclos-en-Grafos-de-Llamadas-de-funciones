#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <limits>
#include <fstream>

class LCCycles {
private:
    llvm::CallGraph &cg;
    int k;
    std::vector<llvm::CallGraphNode*> vertices;
    std::unordered_map<llvm::CallGraphNode*, int> lock;
    std::unordered_map<llvm::CallGraphNode*, std::vector<llvm::CallGraphNode*>> blist;
    std::vector<llvm::CallGraphNode*> stack;
    std::unordered_set<llvm::CallGraphNode*> inStack;
    std::vector<std::vector<llvm::CallGraphNode*>> cycles;

public:
    LCCycles(llvm::CallGraph &CG, int k);

    void construirSubgrafo(llvm::CallGraphNode *s);
    int buscarCiclos(llvm::CallGraphNode *v, llvm::CallGraphNode *s, int k, int flen);
    void relajarLocks(llvm::CallGraphNode *u, int k, int blen);

    void encontrarCiclos();
    void imprimirCiclos();
    const std::vector<std::vector<llvm::CallGraphNode*>> &obtenerCiclos() const { return cycles; }
};

LCCycles::LCCycles(llvm::CallGraph &CG, int k) : cg(CG), k(k) {
    for (auto &nodo : cg) {
        llvm::CallGraphNode *nodoPtr = nodo.second.get();
        if (nodoPtr->getFunction() && !nodoPtr->getFunction()->isDeclaration()) {
            vertices.push_back(nodoPtr);
        }
    }
}

void LCCycles::encontrarCiclos() {
    for (llvm::CallGraphNode *s : vertices) {
        construirSubgrafo(s);
    }
}

void LCCycles::construirSubgrafo(llvm::CallGraphNode *s) {
    std::unordered_set<llvm::CallGraphNode*> alcanzables;
    std::queue<std::pair<llvm::CallGraphNode*, int>> cola;
    cola.push({s, 0});

    while (!cola.empty()) {
        auto [nodo, nivel] = cola.front();
        cola.pop();

        if (nivel < k - 1) {
            for (auto &it : *nodo) {
                llvm::CallGraphNode *nodoSiguiente = it.second;
                if (alcanzables.insert(nodoSiguiente).second) {
                    cola.push({nodoSiguiente, nivel + 1});
                }
            }
        }
    }

    for (llvm::CallGraphNode *v : alcanzables) {
        lock[v] = std::numeric_limits<int>::max();
        blist[v].clear();
    }

    buscarCiclos(s, s, k, 0);
}

int LCCycles::buscarCiclos(llvm::CallGraphNode *v, llvm::CallGraphNode *s, int k, int flen) {
    lock[v] = flen;
    stack.push_back(v);
    inStack.insert(v);

    int blen = std::numeric_limits<int>::max();
    std::vector<llvm::CallGraphNode*> cicloActual;

    for (auto &it : *v) {
        llvm::CallGraphNode *w = it.second;

        if (w == s) {
            cicloActual.assign(stack.begin(), stack.end());
            cicloActual.push_back(s);
            cycles.push_back(cicloActual);
            llvm::errs() << "Ciclo encontrado: ";
            for (auto *nodo : stack) {
                llvm::errs() << nodo->getFunction()->getName() << " ";
            }
            llvm::errs() << s->getFunction()->getName() << "\n";
            blen = 1;
        } else if (flen + 1 < lock[w] && flen + 1 < k) {
            blen = std::min(blen, 1 + buscarCiclos(w, s, k, flen + 1));
        }
    }

    if (blen < std::numeric_limits<int>::max()) {
        relajarLocks(v, k, blen);
    } else {
        for (auto &it : *v) {
            llvm::CallGraphNode *w = it.second;
            if (inStack.find(w) == inStack.end()) {
                blist[w].push_back(v);
            }
        }
    }

    stack.pop_back();
    inStack.erase(v);
    return blen;
}

void LCCycles::relajarLocks(llvm::CallGraphNode *u, int k, int blen) {
    if (lock[u] < k - blen + 1) {
        lock[u] = k - blen + 1;
        for (llvm::CallGraphNode *w : blist[u]) {
            if (inStack.find(w) == inStack.end()) {
                relajarLocks(w, k, blen + 1);
            }
        }
    }
}

void LCCycles::imprimirCiclos() {
    for (const auto &ciclo : cycles) {
        llvm::errs() << "Ciclo: ";
        for (llvm::CallGraphNode *nodo : ciclo) {
            llvm::errs() << nodo->getFunction()->getName() << " ";
        }
        llvm::errs() << "\n";
    }
}

void eliminarLlamadasRecursivasRedundantes(llvm::Function *F) {
    for (auto &BB : *F) {
        for (auto it = BB.begin(); it != BB.end(); ) {
            if (llvm::CallInst *llamada = llvm::dyn_cast<llvm::CallInst>(&*it)) {
                if (llamada->getCalledFunction() == F) {
                    llvm::errs() << "Eliminando llamada recursiva redundante en la función: " << F->getName() << "\n";
                    it = llamada->eraseFromParent();
                    continue;
                }
            }
            ++it;
        }
    }
}

void eliminarFuncionesNoUsadas(llvm::Module &M) {
    std::vector<llvm::Function*> funcionesAEliminar;
    for (auto &F : M) {
        if (F.isDeclaration() || F.use_empty()) {
            funcionesAEliminar.push_back(&F);
        }
    }
    for (auto *F : funcionesAEliminar) {
        llvm::errs() << "Eliminando función no usada: " << F->getName() << "\n";
        F->eraseFromParent();
    }
}

void optimizarLlamadasRecursivas(llvm::Function *F) {
    // Aplicar optimizaciones adicionales a llamadas recursivas si es necesario.
}

void optimizarFuncion(llvm::Function *F) {
    eliminarLlamadasRecursivasRedundantes(F);
    optimizarLlamadasRecursivas(F);
}

void optimizarModulo(llvm::Module &M, const std::vector<std::vector<llvm::CallGraphNode*>> &ciclos) {
    std::unordered_set<llvm::Function*> funcionesOptimizadas;

    for (const auto &ciclo : ciclos) {
        for (llvm::CallGraphNode *nodo : ciclo) {
            llvm::Function *F = nodo->getFunction();
            if (F && !F->isDeclaration()) {
                if (funcionesOptimizadas.insert(F).second) {
                    llvm::errs() << "Optimizando función: " << F->getName() << "\n";
                    optimizarFuncion(F);
                }
            }
        }
    }

    // Después de optimizar funciones, eliminar funciones no usadas
    eliminarFuncionesNoUsadas(M);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo IR>\n";
        return 1;
    }

    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    std::unique_ptr<llvm::Module> Mod = llvm::parseIRFile(argv[1], Err, Context);

    if (!Mod) {
        std::cerr << "Error al leer el módulo\n";
        Err.print(argv[1], llvm::errs());
        return 1;
    }

    llvm::CallGraph CG(*Mod);
    int k = 4;  // Longitud máxima de los nodos del ciclo

    LCCycles lcCycles(CG, k);
    lcCycles.encontrarCiclos();
    lcCycles.imprimirCiclos();

    optimizarModulo(*Mod, lcCycles.obtenerCiclos());

    // Salida del LLVM IR optimizado a archivo
    std::string strOptimizado;
    llvm::raw_string_ostream OS(strOptimizado);
    Mod->print(OS, nullptr);
    OS.flush();

    std::ofstream archivoSalida("optimizado.ll");
    archivoSalida << strOptimizado;
    archivoSalida.close();

    return 0;
}

