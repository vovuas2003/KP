#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int c2i(char);
char alf[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int num[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

int main() {
    int n, k;
    char *a = (char *) calloc (33, sizeof(char));
    scanf("%d", &n);
    scanf("%s", a);
    scanf("%d", &k);
    int l = strlen(a);
    int s = 0;
    for (int i = 0; i < l; i++) {
        s = s * n + c2i(a[i]);
    }
    free(a);
    if (s == 0) {
        printf("0");
        return 0;
    }
    char *b = (char *) calloc (33, sizeof(char));
    int i = 0;
    while (s > 0) {
        b[i] = alf[s % k];
        i++;
        s /= k;
    }
    b[i] = '\0';
    int ll = strlen(b);
    char *c = (char *) calloc (ll + 1, sizeof(char));
    c[ll] = '\0';
    int j;
    for (i = 0, j = ll - 1; i < ll; i++, j--) {
        c[i] = b[j];
    }
    free(b);
    printf("%s", c);
    free(c);
    return 0;
}

int c2i(char c) {
    int t = 0;
    for (int i = 0; i < 36; i++) {
        if (alf[i] == c) {
            t = num[i];
            break;
        }
    }
    return t;
}
