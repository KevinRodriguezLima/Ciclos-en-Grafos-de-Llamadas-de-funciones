int isEven(int n);
int isOdd(int n) {
    return n == 0 ? 0 : isEven(n - 1);
}
int isEven(int n) {
    return n == 0 ? 1 : isOdd(n - 1);
}

