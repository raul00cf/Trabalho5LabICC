#include<stdio.h>

// Função que limpa o buffer para não apresetar error na leitura de fgets e scanf

void cleanBuffer(){
    char ch;               
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}
