int func1(int n);
int func2(int n) {
    if (n <= 1) return 1;
    else return func1(n - 1);
}
int func1(int n) {
    if (n <= 1) return 1;
    else return func2(n - 1);
}

