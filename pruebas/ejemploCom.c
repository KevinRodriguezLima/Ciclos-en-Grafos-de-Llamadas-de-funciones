#include <stdio.h>

// Función recursiva que calcula el factorial de un número
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Función que realiza la suma de los primeros n números enteros
int sum(int n) {
    if (n <= 0) return 0;
    return n + sum(n - 1);
}

// Función que realiza la multiplicación de dos números usando suma
int multiply(int a, int b) {
    if (b == 0) return 0;
    return a + multiply(a, b - 1);
}

// Función que calcula el máximo común divisor usando el algoritmo de Euclides
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Función que realiza una serie de cálculos para probar optimización
void complexCalculation(int x) {
    int fact = factorial(x);
    int s = sum(x);
    int prod = multiply(x, 10);
    int divisor = gcd(x, 100);
    
    printf("Factorial of %d: %d\n", x, fact);
    printf("Sum of first %d numbers: %d\n", x, s);
    printf("Product of %d and 10: %d\n", x, prod);
    printf("GCD of %d and 100: %d\n", x, divisor);
}

int main() {
    for (int i = 5; i < 10; i++) {
        complexCalculation(i);
    }
    return 0;
}

