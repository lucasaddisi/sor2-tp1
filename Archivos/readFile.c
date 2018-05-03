#include <stdio.h>

int main(int argc, char * argv[]){

    if (argc != 2){
        printf("Por favor, ingrese solo 1 parmetro. Número de parámetros = %i \n", argc - 1);
        return -1;
    }

    char *fileName = argv[1];
    int c;

    FILE *fp = fopen(fileName, "r");

    while ((c = getc(fp)) != EOF)
        putc(c, stdout);

    fclose(fp);
    printf("\n");
    return 0;
}
