// example_with_cycles.c
#include <stdio.h>

void A();
void B();
void C();

void A() {
    B();
}

void B() {
    C();
}

void C() {
    A();
}

int main() {
    A();
    return 0;
}

