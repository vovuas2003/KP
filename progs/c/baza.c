#include <stdio.h>
#include <stdlib.h>
int ma(int, int);
int ma(int x, int y) {
    if (x < y) {
        return y;
    }
    else {
        return x;
    }
}
int main() {
    int n, m, c;
    FILE *fin;
    FILE *fout;
    fin = fopen("dynprog.txt", "r");
    fout = fopen("answer.txt", "w");
    fscanf(fin, "%d", &n);
    fscanf(fin, "%d", &m);
    int *a = (int*)calloc(n * m, sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(fin, "%d", &a[i * m + j]);
        }
    }
    fscanf(fin, "%d", &c);
    fclose(fin);
    a[0] = c - a[0];
    for (int j = 1; j < m; j++) {
        a[j] = a[j-1] - a[j];
    }
    for (int i = 1; i < n; i++) {
        a[i * m] = a[(i - 1) * m] - a[i * m];
    }
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            a[i * m + j] = ma(a[(i-1) * m + j], a[i * m + j - 1]) - a[i * m + j];
        }
    }
    int check = a[(n-1) * m + m - 1];
    free(a);
    if (check < 0) {
        fprintf(fout, "Impossible");
    }
    else {
        fprintf(fout, "%d", check);
    }
    fclose(fout);
    return 0;
}
