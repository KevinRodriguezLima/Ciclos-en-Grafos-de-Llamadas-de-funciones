void funcB();

void funcA(int n) {
    if (n > 0) {
        funcB();
        funcA(n - 1);
    }
}

void funcB() {
    // Función de ejemplo llamada por funcA de manera indirecta
}

int main() {
    funcA(3);
    return 0;
}

