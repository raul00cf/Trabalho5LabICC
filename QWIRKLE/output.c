#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/jogo.h"
#include "headers/colors.h"

// mensagem de bem vindo

void welcome(){
    printf("\n---- QWIRKLE ----\n");
}

// erro de nome grande demais

void printNomeErrado(){
    printf("Nome do jogador invalido (maximo 18 caracteres)!\n");
}

// erro de primeira da letra do nome errada

void printLetraErrada(){
    printf("Primeiro caractere precisa ser uma letra!\n");
}

// erro de peça específica errada na troca

void printPecaErrada(char peca[3]){
    printf("Peca %s invalida!\n", peca);
}

// erro de peça errada

void printPecaErrada2(){
    printf("Peca invalida!\n");
}

// erro de escolha

void printEscolhaErrada(){
    printf("Escolha incorreta!\n");
}

// erro de posicao

void printPosicaoErrada(){
    printf("Posicao invalida!\n");
}

// erro de falta de não mais peças de reposição

void printFaltaPecas(){
	printf("Nao ha mais pecas para ser trocado!\n");
}

// erro de peças de reposição em menor número que o solicitado para a troca

void printFaltaAlgumasPecas(int i){
	printf("Numero de pecas a serem trocadas maior que o possível!\n");
	printf("(Somente %d pecas restantes)", i);
}

// apresenta vencedor

void printVencedor(infoJogador jogador){
	printf("\n\n--------FIM DE JOGO--------\n\n");
	printf("Vencedor\t= %s\n", jogador.nome);
	printf("Score\t= %3d\n", jogador.score);
}

// apresenta empate

void printVencedores(infoJogo jogo, int *pos, int p){
	int i;

	printf("\n\n--------FIM DE JOGO--------\n\n");
	printf("Empate entre:\n");
	for (i = 0; i < p; i++){
		printf("Jogador %s\n", jogo.jogadores[pos[i]].nome);
	}
	printf("Score = %3d\n", jogo.jogadores[pos[0]].score);

	free(pos);
}

// apresenta as informações de todos os jogadores

void printInfoJogadores(infoJogo jogo){
    int i, j, base;

	for (i = jogo.maiorComp; i >= 0; i--){
		if (i % 4 == 0){
			base = i;
			break;
		}
	}

    for (j = 0; j < jogo.numJogadores; j++){
		if (strlen(jogo.jogadores[j].nome) > base){
			printf("Jogador %s\t(Score %3d):", jogo.jogadores[j].nome, jogo.jogadores[j].score);
		}
		else{
			printf("Jogador %s", jogo.jogadores[j].nome);
			for (i = base; i >=0; i-= 4){
				if (strlen(jogo.jogadores[j].nome) >= i){
					printf("\t(Score %3d):", jogo.jogadores[j].score);
					break;
				}
				else{
					printf("\t");
				}
			}
		}
        for (i = 0; i < N_PECAS; i++){
            if (strcmp(jogo.jogadores[j].pecas[i], "\0")){
                selecionarCor(jogo.jogadores[j].pecas[i]);
                printf(" %s", jogo.jogadores[j].pecas[i]);
                reset();
            }
        }
        printf("\n");
    }

    printf("-----------------\n");
}

// imprime as informações da jogada do jogador

void printJogada(infoJogador jogador){
    int i;

    printf("Jogada de %s\n", jogador.nome);
    printf("Pecas disponiveis:");

    for(i = 0; i < N_PECAS; i++){
        if (strcmp(jogador.pecas[i], "\0")){
            selecionarCor(jogador.pecas[i]);
            printf(" %s", jogador.pecas[i]);
            reset();
        }
    }
    printf("\nOpcoes: trocar p1 [p2 p3...] | jogar p1 x y | passar\n");
}

// imprime as informações da segunda jogada do jogador

void printSegundaJogada(infoJogador jogador){
    int i;

    printf("Jogada de %s\n", jogador.nome);
    printf("Pecas disponiveis:");

    for(i = 0; i < N_PECAS; i++){
        if (strcmp(jogador.pecas[i], "\0")){
            selecionarCor(jogador.pecas[i]);
            printf(" %s", jogador.pecas[i]);
            reset();
        }
    }
    printf("\nOpcoes: jogar p1 x y | passar\n");
}

// imrpime o tabuleiro

void printTabuleiro(infoTab tab){
    int i, j;
    printf("\n");

    for (i = tab.minY - 2; i <= tab.maxY + 1; i++){
        for (j = tab.minX - 2; j <= tab.maxX + 1; j++){
            if (i == tab.minY - 2){
                if (j == tab.minX - 2  || j == tab.minX - 1 || j == tab.maxX + 1){
                    printf("   ");
                }
                else{
                    printf(" x ");
                }
                printf(" ");
            }
            else if (i == tab.minY - 1 || i == tab.maxY + 1){
                if (j == tab.minX - 2  || j == tab.minX - 1 || j == tab.maxX + 1){
                    printf("    ");
                }
                else{
					if (j < -9){
						printf("%2d ", j);
					}
					else{
						printf("%2d  ", j);
					}
                }
            }
            else{
                if (j == tab.minX - 2){
                    printf("y   ");
                }
                else if (j == tab.minX - 1 || j == tab.maxX + 1){
                    printf("%3d ", i);
                }
                else{
                    if (strcmp(tab.local[tab.centroY + i][tab.centroX + j], "\0")){
                        selecionarCor(tab.local[tab.centroY + i][tab.centroX + j]);
                        printf(" %s|", tab.local[tab.centroY + i][tab.centroX + j]);
                        reset();
                    }
                    else{
                        printf("   |");
                    }
                }
            }
        }

        printf("\n");

        if (i != tab.maxY + 1 && i != tab.minY - 1 && i != tab.minY - 2){
            for (j = tab.minX - 2; j <= tab.maxX + 1; j++){
                if (j == tab.minX - 2 || j == tab.minX - 1 || j == tab.maxX + 1){
                    printf("    ");
                }
                else{
                    printf("----");
                }
            }
            printf("\n");
        }
    }

    printf("\n");
}

// imprime a pontuação do fim da jogada

void printPontJogadorRodada(infoJogador jogador){
    printf("-------------------------------------------------------------\n");
    printf("\tPontuacao %s: %3d | Pontuacao acumulada: %3d\n", jogador.nome, jogador.scoreRodada, jogador.score);
    printf("-------------------------------------------------------------\n");
}