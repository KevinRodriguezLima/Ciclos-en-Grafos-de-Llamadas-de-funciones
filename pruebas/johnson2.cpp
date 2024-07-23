#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <limits>

using Graph = std::unordered_map<int, std::unordered_set<int>>;
using Cycles = std::vector<std::vector<int>>;

// Generamos el .dot
Graph generateCallGraph(const char *inputFile, const char *outputFile, std::unordered_map<std::string, int> &funcNameToId, std::unordered_map<int, std::string> &idToFuncName) {
    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    std::unique_ptr<llvm::Module> Mod(llvm::parseIRFile(inputFile, Err, Context));

    if (!Mod) {
        std::cerr << "Fallo al leer el modulo\n";
        Err.print(inputFile, llvm::errs());
        exit(1);
    }

    Graph graph;
    int funcId = 0;

    for (llvm::Function &Func : *Mod) {
        if (!Func.isDeclaration()) {
            std::string funcName = Func.getName().str();
            if (funcNameToId.find(funcName) == funcNameToId.end()) {
                funcNameToId[funcName] = funcId;
                idToFuncName[funcId] = funcName;
                funcId++;
            }
            int funcId1 = funcNameToId[funcName];

            for (llvm::BasicBlock &BB : Func) {
                for (llvm::Instruction &Inst : BB) {
                    if (llvm::CallInst *Call = llvm::dyn_cast<llvm::CallInst>(&Inst)) {
                        llvm::Function *CalleeFunc = Call->getCalledFunction();
                        if (CalleeFunc) {
                            std::string calleeName = CalleeFunc->getName().str();
                            if (funcNameToId.find(calleeName) == funcNameToId.end()) {
                                funcNameToId[calleeName] = funcId;
                                idToFuncName[funcId] = calleeName;
                                funcId++;
                            }
                            int funcId2 = funcNameToId[calleeName];
                            graph[funcId1].insert(funcId2);
                        }
                    }
                }
            }
        }
    }

    std::ofstream dotFile(outputFile);
    if (!dotFile.is_open()) {
        std::cerr << "Fallo en abrir el archivo de salida\n";
        exit(1);
    }

    dotFile << "digraph CallGraph {\n";
    for (const auto &entry : graph) {
        dotFile << "\"" << idToFuncName[entry.first] << "\";\n";
        for (int calleeId : entry.second) {
            dotFile << "\"" << idToFuncName[entry.first] << "\" -> \"" << idToFuncName[calleeId] << "\";\n";
        }
    }
    dotFile << "}\n";
    dotFile.close();

    std::cout << "El grafo contiene:\n";
    for (const auto &entry : graph) {
        std::cout << "Funcion ID: " << entry.first << " llamadas: ";
        for (int calleeId : entry.second) {
            std::cout << calleeId << " ";
        }
        std::cout << "\n";
    }

    return graph;
}

class JohnsonCycleFinder {
public:
    JohnsonCycleFinder(const Graph &graph) : graph(graph), blocked(graph.size(), false), index(graph.size(), 0), stack() {}

    Cycles findCycles() {
        for (const auto &entry : graph) {
            int startNode = entry.first;
            strongComponents.clear();
            blocked.assign(graph.size(), false);
            blockMap.clear();
            currentCycle.clear();
            if (johnsonDFS(startNode, startNode)) {
                cycles.push_back(currentCycle);
            }
        }
        return cycles;
    }

private:
    const Graph &graph;
    std::vector<bool> blocked;
    std::vector<int> index;
    std::stack<int> stack;
    Cycles cycles;
    std::vector<int> currentCycle;
    std::unordered_map<int, std::unordered_set<int>> blockMap;
    std::vector<int> strongComponents;

    bool johnsonDFS(int start, int vertex) {
        bool foundCycle = false;
        stack.push(vertex);
        blocked[vertex] = true;
        currentCycle.push_back(vertex);

        if (graph.find(vertex) != graph.end()) {
            for (int neighbor : graph.at(vertex)) {
                if (neighbor == start) {
                    currentCycle.push_back(start);
                    cycles.push_back(currentCycle);
                    currentCycle.pop_back();
                    foundCycle = true;
                } else if (!blocked[neighbor]) {
                    if (johnsonDFS(start, neighbor)) {
                        foundCycle = true;
                    }
                }
            }
        }

        if (foundCycle) {
            unblock(vertex);
        }

        stack.pop();
        currentCycle.pop_back();

        return foundCycle;
    }

    void unblock(int vertex) {
        blocked[vertex] = false;
        for (int neighbor : blockMap[vertex]) {
            if (blocked[neighbor]) {
                unblock(neighbor);
            }
        }
        blockMap[vertex].clear();
    }
};

// Función para imprimir los ciclos
void printCycles(const Cycles &cycles, const std::unordered_map<int, std::string> &idToFuncName) {
    if (cycles.empty()) {
        std::cout << "No se encontraron ciclos." << std::endl;
        return;
    }

    std::cout << "Ciclos encontrados:" << std::endl;
    for (const auto &cycle : cycles) {
        for (int node : cycle) {
            std::cout << idToFuncName.at(node) << " ";
        }
        std::cout << std::endl;
    }
}

void removeRedundantRecursiveCalls(llvm::Function *F) {
    for (auto &BB : *F) {
        for (auto it = BB.begin(); it != BB.end(); ) {
            if (llvm::CallInst *call = llvm::dyn_cast<llvm::CallInst>(&*it)) {
                if (call->getCalledFunction() == F) {
                    llvm::errs() << "Eliminando llamada recursiva redundante en la función: " << F->getName() << "\n";
                    it = call->eraseFromParent();
                    continue;
                }
            }
            ++it;
        }
    }
}

void removeUnusedFunctions(llvm::Module &M) {
    std::vector<llvm::Function*> functionsToRemove;
    for (auto &F : M) {
        if (F.isDeclaration() || F.use_empty()) {
            functionsToRemove.push_back(&F);
        }
    }
    for (auto *F : functionsToRemove) {
        llvm::errs() << "Eliminando función no utilizada: " << F->getName() << "\n";
        F->eraseFromParent();
    }
}

void optimizeRecursiveCalls(llvm::Function *F) {
    // Aquí se pueden aplicar técnicas adicionales para optimizar llamadas recursivas,
    // como desenrollar la recursión o ajustar la lógica según el análisis de ciclos.
    // Esta función puede ser extendida para técnicas más avanzadas si es necesario.
}

void optimizeFunction(llvm::Function *F) {
    removeRedundantRecursiveCalls(F);
    optimizeRecursiveCalls(F);
}

void optimizeModule(llvm::Module &M, const Cycles &cycles, const std::unordered_map<int, std::string> &idToFuncName) {
    std::unordered_set<llvm::Function*> optimizedFunctions;

    for (const auto &cycle : cycles) {
        for (int nodeId : cycle) {
            llvm::Function *F = M.getFunction(idToFuncName.at(nodeId));
            if (F && !F->isDeclaration() && optimizedFunctions.insert(F).second) {
                llvm::errs() << "Optimizando función: " << F->getName() << "\n";
                optimizeFunction(F);
            }
        }
    }

    // Después de optimizar funciones, eliminamos las funciones no utilizadas
    removeUnusedFunctions(M);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo IR>\n";
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = (argc > 2) ? argv[2] : "callgraph.dot";

    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    std::unique_ptr<llvm::Module> Mod = llvm::parseIRFile(inputFile, Err, Context);

    if (!Mod) {
        std::cerr << "Fallo al leer el módulo\n";
        return 1;
    }

    std::unordered_map<std::string, int> funcNameToId;
    std::unordered_map<int, std::string> idToFuncName;

    Graph graph = generateCallGraph(inputFile, outputFile, funcNameToId, idToFuncName);

    JohnsonCycleFinder johnsonFinder(graph);
    Cycles cycles = johnsonFinder.findCycles();

    printCycles(cycles, idToFuncName);

    optimizeModule(*Mod, cycles, idToFuncName);

    // Guardar el módulo optimizado en un archivo nuevo
    std::error_code EC;
    llvm::raw_fd_ostream outFile("optimized_module.ll", EC, llvm::sys::fs::OF_None);

    if (EC) {
        std::cerr << "Error al crear el archivo de salida: " << EC.message() << "\n";
        return 1;
    }

    Mod->print(outFile, nullptr);
    return 0;
}

