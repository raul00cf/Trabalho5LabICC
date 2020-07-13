#ifndef _TAB_
#define _TAB_

typedef struct{
    int minX;
    int centroX;
    int maxX;
    int minY;
    int centroY;
    int maxY;
    char ***local;
} infoTab;

#endif

void iniciarTabuleiro(infoTab *tab);
int posicaoAceita(infoTab tab, int x, int y, int x0, int y0, char peca[], char esc);
void colocarMatriz(infoTab *tab, char peca[], int x, int y);
void realocarMatriz(infoTab *tab, int x, int y);
int pontuar(infoTab tab, int **pos, int p);