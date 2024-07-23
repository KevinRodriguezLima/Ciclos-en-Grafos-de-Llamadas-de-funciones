void funcC();

void funcA() {
    funcC();
}

void funcB() {
    funcA();
}

void funcC() {
    funcB();
}

int main() {
    funcA();
    return 0;
}

