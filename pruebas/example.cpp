#include <iostream>

void foo() {
    std::cout << "In foo" << std::endl;
}

void bar() {
    foo();
}

void recur(int a) {
    if(a == 0) std::cout << "fin" << std::endl;
    else {
        std::cout << a << std::endl;
        recur(a - 1);
    }
}

int main() {
    recur(5);
    return 0;
}
