#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <fstream>

// Función para ejecutar un comando y medir el tiempo de ejecución
double executeAndMeasureTime(const std::string& command) {
    std::string fullCommand = command + " > /dev/null 2>&1"; // Redirige stdout y stderr a /dev/null
    auto start = std::chrono::high_resolution_clock::now();
    int result = std::system(fullCommand.c_str());
    auto end = std::chrono::high_resolution_clock::now();
    
    if (result != 0) {
        std::cerr << "Error al ejecutar el comando: " << command << std::endl;
        return -1.0;
    }
    
    // Usa la duración en segundos
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000.0; // Convertir a milisegundos
}

int main() {
    const char* examples[] = {
    "direct1.ll",
    "direct2fib.ll",
    "direct3sum.ll",
    "indirect1odd.ll",
    "indirect2mut.ll",
    "indirect3.ll",
    "mut1mut.ll",
    "mut2checkodd.ll",
    "mut3alter.ll",
    "nested1Ackermann.ll",
    "nested2fib.ll",
    "nested3fact.ll",
    "com1.ll",
    "com2.ll",
    "com3mul.ll",
    "ejemploCom.ll",
    "directo.ll",
    "cicloMultiple.ll",
    "ejemploCiclos.ll",
    "recurIndirecta.ll",
    "factorial.ll"};

    const char* lcCyclesCommand = "./lcCycles";
    const char* evalJohnsonCommand = "./evalJohnson";

    std::ofstream resultsFile("resultados.txt");
    if (!resultsFile) {
        std::cerr << "No se pudo abrir el archivo de resultados." << std::endl;
        return 1;
    }

    resultsFile << "Archivo,lcCycles (ms),evalJohnson (ms)" << std::endl;

    for (const char* example : examples) {
        std::cout << "Ejecutando en " << example << "..." << std::endl;

        std::string lcCyclesCmd = std::string(lcCyclesCommand) + " " + example;
        double lcCyclesTime = executeAndMeasureTime(lcCyclesCmd);
        if (lcCyclesTime != -1.0) {
            std::cout << "Tiempo de ejecución de lcCycles en " << example << ": " << lcCyclesTime << " ms" << std::endl;
        }

        std::string evalJohnsonCmd = std::string(evalJohnsonCommand) + " " + example;
        double evalJohnsonTime = executeAndMeasureTime(evalJohnsonCmd);
        if (evalJohnsonTime != -1.0) {
            std::cout << "Tiempo de ejecución de evalJohnson en " << example << ": " << evalJohnsonTime << " ms" << std::endl;
        }

        resultsFile << example << "," << lcCyclesTime << "," << evalJohnsonTime << std::endl;
    }

    resultsFile.close();

    return 0;
}

