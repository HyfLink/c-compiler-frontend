#define STR "Hello, world!\n"

int main() {
    char const *str1 = STR;
    char const *str2 = STR;

#undef STR

    int STR = 1;
    for (; STR < 5; ++STR) {
        char ch = str1[STR];
    }
    return 0;
}