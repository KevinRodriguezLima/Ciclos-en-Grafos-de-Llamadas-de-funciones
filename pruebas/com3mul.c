int multipleRecursion1(int n);
int multipleRecursion2(int n) {
    if (n <= 1) return n;
    else return multipleRecursion1(n - 1) + multipleRecursion1(n - 2);
}
int multipleRecursion1(int n) {
    if (n <= 1) return n;
    else return multipleRecursion2(n - 1) + multipleRecursion2(n - 2);
}

