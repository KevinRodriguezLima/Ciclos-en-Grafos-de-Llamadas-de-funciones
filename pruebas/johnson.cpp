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
        for (auto &entry : graph) {
            int startNode = entry.first;
            strongComponents.clear();
            blocked.assign(graph.size(), false);
            blockMap.clear();
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

        for (int neighbor : graph.at(vertex)) {
            if (neighbor == start) {
                currentCycle.push_back(start);
                foundCycle = true;
            } else if (!blocked[neighbor]) {
                if (johnsonDFS(start, neighbor)) {
                    foundCycle = true;
                }
            }
        }

        if (foundCycle) {
            cycles.push_back(currentCycle);
        } else {
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

    JohnsonCycleFinder finder(callGraph);
    Cycles cycles = finder.findCycles();

    printCycles(cycles, idToFuncName);

    std::cout << "DOT file creado en " << outputFile << "\n";
    return 0;
}

