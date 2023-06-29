int main(int argc, const char** argv) {
    int a    = 1;
    int b    = 2;
    int* ptr = 0;

    ptr  = &a;
    *ptr = 1;

    a* b;
    a / b;
    a + b;
    a - b;
    a % b;
    a << b;
    a >> b;
    a | b;
    a& b;
    a || b;
    a&& b;
    a *= b;
    a /= b;
    a += b;
    a -= b;
    a %= b;
    a <<= b;
    a >>= b;
    a |= b;
    a &= b;
    !a;
    ~a;
    ++a;
    --b;
    a++;
    b--;
    +a;
    -b;

    return main(
        argc,
        argv);
}