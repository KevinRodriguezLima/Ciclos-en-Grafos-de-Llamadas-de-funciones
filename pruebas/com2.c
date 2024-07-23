int combinedRecursion1(int n);
int combinedRecursion2(int n) {
    if (n <= 1) return 1;
    else return combinedRecursion1(n - 1);
}
int combinedRecursion1(int n) {
    if (n <= 1) return 1;
    else return n * combinedRecursion2(n - 1);
}

