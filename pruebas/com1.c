int complexFunc1(int n);
int complexFunc2(int n) {
    if (n <= 1) return n;
    else return complexFunc1(n - 1) + complexFunc1(n - 2);
}
int complexFunc1(int n) {
    if (n <= 1) return n;
    else return complexFunc2(n - 1) + complexFunc2(n - 2);
}

