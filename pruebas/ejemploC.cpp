#include <iostream>

// Función recursiva simple
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Ciclo en el grafo de llamadas de funciones
void functionA();
void functionB();

void functionA() {
    std::cout << "In functionA\n";
    functionB();
}

void functionB() {
    std::cout << "In functionB\n";
    functionA();
}

int main() {
    int num = 5;
    std::cout << "Factorial of " << num << " is " << factorial(num) << "\n";

    // Llamadas que crean un ciclo
    functionA();

    return 0;
}

