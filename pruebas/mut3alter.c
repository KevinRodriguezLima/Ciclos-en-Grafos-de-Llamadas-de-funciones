int alpha(int n);
int beta(int n) {
    if (n <= 1) return 1;
    else return alpha(n - 1);
}
int alpha(int n) {
    if (n <= 1) return 1;
    else return beta(n - 1);
}

