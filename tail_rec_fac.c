#include <stdio.h>

int f(int, int);
int fac(int);

int main() {
    int n = 0;
    scanf("%d", &n);
    printf("%d", fac(n));
    return 0;
}

int f(int n, int temp) {
    if(n <= 0) {
        return temp;
    }
    return f(n - 1, temp * n);
}

int fac(int n) {
    return f(n, 1);
}
