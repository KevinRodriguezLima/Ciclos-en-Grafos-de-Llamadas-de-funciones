#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/Instructions.h" // Necesario para obtener definición completa de CallInst

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

using Graph = std::unordered_map<int, std::unordered_set<int>>;
using Cycles = std::vector<std::vector<int>>;

//generamos el .dot|
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

            // Buscar llamadas recursivas directas dentro de la función
            for (llvm::BasicBlock &BB : Func) {
                for (llvm::Instruction &Inst : BB) {
                    if (llvm::CallInst *Call = llvm::dyn_cast<llvm::CallInst>(&Inst)) {
                        llvm::Function *CalleeFunc = Call->getCalledFunction();
                        if (CalleeFunc && CalleeFunc->getName() == Func.getName()) {
                            // Encontramos una llamada recursiva directa
                            graph[funcId1].insert(funcId1);
                            break;
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

    std::cout << "El grafo contiende:\n";
    for (const auto &entry : graph) {
        std::cout << "Funcion ID: " << entry.first << " llamadas:: ";
        for (int calleeId : entry.second) {
            std::cout << calleeId << " ";
        }
        std::cout << "\n";
    }

    return graph;
}

// Funcion para el caso especial de ciclos directos
class DirectCycleFinder {
public:
    DirectCycleFinder(const Graph &graph) : graph(graph), visited(graph.size(), false), onStack(graph.size(), false) {}

    Cycles findCycles() {
        for (const auto &entry : graph) {
            int node = entry.first;
            if (!visited[node]) {
                if (dfs(node)) {
                    cycles.push_back(currentCycle);
                }
            }
        }
        return cycles;
    }

private:
    const Graph &graph;
    std::vector<bool> visited;
    std::vector<bool> onStack;
    std::vector<int> currentCycle;
    Cycles cycles;
	// realizamos una busqueda en profundidad para marcar los nodos visitados
    bool dfs(int v) {
        visited[v] = true;
        onStack[v] = true;
        currentCycle.push_back(v);

        for (int w : graph.at(v)) {
            if (!visited[w]) {
                if (dfs(w)) {
                    return true;
                }
            } else if (onStack[w]) {
                // Ciclo detectado
                currentCycle.push_back(w);
                return true;
            }
        }

        // No hay ciclo desde este nodo
        currentCycle.pop_back();
        onStack[v] = false;
        return false;
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

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input LLVM IR file> [output DOT file]\n";
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = (argc > 2) ? argv[2] : "callgraph.dot";

    std::unordered_map<std::string, int> funcNameToId;
    std::unordered_map<int, std::string> idToFuncName;

    Graph callGraph = generateCallGraph(inputFile, outputFile, funcNameToId, idToFuncName);

    DirectCycleFinder finder(callGraph);
    Cycles cycles = finder.findCycles();

    printCycles(cycles, idToFuncName);

    std::cout << "DOT file creado" << outputFile << "\n";
    return 0;
}

