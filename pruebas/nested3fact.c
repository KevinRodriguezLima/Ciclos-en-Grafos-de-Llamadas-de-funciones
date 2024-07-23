int nestedFactorial(int n) {
    if (n <= 1) return 1;
    else return n * nestedFactorial(nestedFactorial(n - 1));
}

