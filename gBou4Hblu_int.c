#include <stdio.h>
int main() {
    int n;
    printf("Write an integer in decimal notation to convert it to binary notation: ");
    scanf("%d", &n);
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\nPress Enter to close ");
    getchar();
    getchar();
    return 0;
}
