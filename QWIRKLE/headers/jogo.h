#include "jogador.h"
#include "tabuleiro.h"

#ifndef _JOGO_
#define _JOGO_

#define TRUE 1
#define FALSE 0
#define INF 1000

typedef struct{
    int quantPecas[36];
    int numPecas;
    int numJogadores;
    infoJogador *jogadores;
    infoTab tabuleiro;
    int fim;
	short cheatMode;
	int maiorComp;
} infoJogo;

#endif

void menu();
