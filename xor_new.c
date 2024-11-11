#include <stdio.h>

int my_strlen(char*);

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Need 3 arguments: input_name output_name key\n");
        return 0;
    }
    char* key = argv[3];
    int len = my_strlen(key);
    char buf;
    FILE* fin = fopen(argv[1], "rb");
    if(fin == NULL) {
        printf("Wrong input_name, exit!\n");
        return 0;
    }
    FILE* fout = fopen(argv[2], "wb");
    if(fout == NULL) {
        printf("Wrong output_name, exit!\n");
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

int my_strlen(char* s) {
    int l = 0;
    while(s[l] != '\0') {
        l++;
    }
    return l;
}
