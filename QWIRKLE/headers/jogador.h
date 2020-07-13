#ifndef _JOGADOR_
#define _JOGADOR_

#define N_PECAS 6
#define NOME_JOG 20

typedef struct{
    int numero;
    char nome[NOME_JOG];
    int score;
    int scoreRodada;
    char pecas[N_PECAS][3];
} infoJogador;

#endif
