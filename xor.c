#include <stdio.h>
#include <conio.h>

#define LEN 10

int main(int argc, char *argv[]) {
    char key[LEN];
    int len = 0;
    char buf;
    int f = 0, ff = 0;
    while(1) {
        buf = getch();
        if(buf == 13) {
            break;
        }
        if(f) {
            ff = 1;
            continue;
        }
        key[len] = buf;
        len++;
        if(len == LEN) {
            f = 1;
        }
    }
    if(ff) {
        return 0;
    }
    if(len == 0) {
        return 0;
    }
    if(argc != 3) {
        return 0;
    }
    FILE* fin = fopen(argv[1], "rb");
    if(fin == NULL) {
        return 0;
    }
    FILE* fout = fopen(argv[2], "wb");
    if(fout == NULL) {
        return 0;
    }
    int i = 0;
    while(1 == fread(&buf, sizeof(char), 1, fin)) {
        buf ^= key[i];
        i++;
        if(i == len) {
            i = 0;
        }
        fwrite(&buf, sizeof(char), 1, fout);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
