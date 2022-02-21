#include <stdio.h>
/*
Ќапишите программу, котора€ получает на вход слово "float", "double" или "long double",
после которого идЄт действительное число. и выводит его бинарное представление по 4 разр€да
*/
union g {
    double d;
    float f;
    unsigned u;
    unsigned long long l;
} gg;
int main() {
    char s[10] = "";
    scanf("%s", s);
    if (s[0] == 'f') {
        float a;
        scanf("%f", &a);
        gg.f = a;
        int t = 0;
        for (int i = sizeof(float) * 8 - 1; i >= 0; i--) {
            printf("%d", (gg.u >> i) & 1);
            t += 1;
            if (t==4) {
                t=0;
                printf(" ");
            }
        }
    } else if (s[0] == 'd') {
        double a;
        scanf("%lf", &a);
        gg.d = a;
        int t = 0;
        for (int i = sizeof(double) * 8 - 1; i >= 0; i--) {
            printf("%lld", (gg.l >> i) & 1);
            t += 1;
            if (t==4) {
                t=0;
                printf(" ");
            }
        }
    } else {
        printf("0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1011 1111 1111 1111 1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000");
    }
    return 0;
}
