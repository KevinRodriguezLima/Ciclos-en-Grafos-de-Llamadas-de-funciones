int nestedFibonacci(int n) {
    if (n <= 1) return n;
    else return nestedFibonacci(nestedFibonacci(n - 1)) + nestedFibonacci(n - 2);
}

