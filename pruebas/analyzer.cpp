#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Analysis/CallGraph.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input LLVM IR file> [output DOT file]\n";
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = (argc > 2) ? argv[2] : "callgraph.dot";

    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    std::unique_ptr<llvm::Module> Mod(llvm::parseIRFile(inputFile, Err, Context));

    if (!Mod) {
        std::cerr << "Failed to read the module\n";
        Err.print(argv[0], llvm::errs());
        return 1;
    }

    llvm::CallGraph CG(*Mod);
    std::ofstream dotFile(outputFile);

    if (!dotFile.is_open()) {
        std::cerr << "Failed to open the output file\n";
        return 1;
    }

    dotFile << "digraph CallGraph {\n";

    for (auto &Node : CG) {
        llvm::Function *Func = Node.second->getFunction();
        if (Func && !Func->isDeclaration()) {
            std::string funcName = Func->getName().str();
            dotFile << "\"" << funcName << "\";\n";
            for (auto &CS : *Node.second) {
                if (CS.second->getFunction()) {
                    dotFile << "\"" << funcName << "\" -> \"" << CS.second->getFunction()->getName().str() << "\";\n";
                }
            }
        }
    }

    dotFile << "}\n";
    dotFile.close();

    std::cout << "DOT file created at " << outputFile << "\n";
    return 0;
}

