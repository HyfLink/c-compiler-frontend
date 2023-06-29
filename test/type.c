static int a = 0;

static int f1(int b, float a) { return a + b; }
extern int f2(int b, float a) { return a - b; }
extern int* f3(void);

int main(int, const char**) {
    auto int a0           = 0;
    register short int a1 = 0;
    long int a2           = 0;
    signed char a3        = 0;
    unsigned char a4      = 0;
    float a5              = 0;
    long double a6        = 0;
    long int a7           = 0;
    int t[1];
    int(*s)[1][10];
    int (*r[1][10][10 * 10])(int);
    const int volatile unsigned* const* volatile* const volatile c = 0;
    int (*f)(int, unsigned int*, long double, ...)                 = 0;
    return 0;
}