#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "headers/jogo.h"
#include "headers/buffer.h"
#include "headers/pecas.h"
#include "headers/output.h"


// Função para determinar o número de peças que o jogador tem na mão


int verNumPecas(infoJogador jogador){
	int num = 0, i;

	for (i = 0; i < N_PECAS; i++){
		if (strcmp(jogador.pecas[i], "\0")){
			num++;
		}
	}

	return num;
}


// Função para definir os jogadores, assim como as informações referentes ao jogo


infoJogo definirJogadores(){
    int numJog, i, j, pos;
    infoJogo jogo;
    char *quebra_linha, escCheat[3];
	short cheatEscolhido = FALSE;
    
    for (i = 0; i < 36; i++){
        jogo.quantPecas[i] = 3;
	}

    srand(time(NULL));

	// determina o numero de jogadores

    do{
        printf("Digite o numero de jogadores (2 - 4):\t");
        scanf("%d", &numJog);
        cleanBuffer();

        if (numJog < 2 || numJog > 4){
            printf("Numero invalido de jogadores!\n");
        }

    } while (numJog < 2 || numJog > 4);

    jogo.numJogadores = numJog;
    jogo.jogadores = (infoJogador*) malloc(numJog * sizeof(infoJogador));

	// para cada jogador, recebe o nome e atribui valores de score e peças iniciais

    for (i = 0; i < numJog; i++){
        printf("Digite o nome do jogador %d:\t", i + 1);
        fgets(jogo.jogadores[i].nome, NOME_JOG, stdin);
        quebra_linha = strchr(jogo.jogadores[i].nome, '\n');
        if(quebra_linha != NULL){
            *quebra_linha = '\0';
			if (!((jogo.jogadores[i].nome[0] >= 'a' && jogo.jogadores[i].nome[0] <= 'z') || (jogo.jogadores[i].nome[0] >= 'A' && jogo.jogadores[i].nome[0] <= 'Z'))){
				printLetraErrada();
				i--;
				continue;
			}
        }
        else{
            cleanBuffer();
            printNomeErrado();
            i--;
            continue;
        }

        jogo.jogadores[i].numero = i;
        jogo.jogadores[i].score = 0;
        jogo.jogadores[i].scoreRodada = 0;

        for (j = 0; j < N_PECAS; j++){
            do{
                pos = rand() % TOTAL_PECAS;
            } while (!jogo.quantPecas[pos]);

            jogo.quantPecas[pos]--;

            strcpy(jogo.jogadores[i].pecas[j], tipoPecas[pos]);
        }        
    }

	jogo.maiorComp = 0;

	// determina o maior comprimento de nome de usuário

	for (j = 0; j < jogo.numJogadores; j++){
		if (strlen(jogo.jogadores[j].nome) > jogo.maiorComp){
			jogo.maiorComp = strlen(jogo.jogadores[j].nome);
		}
	}

	// determina o modo de cheat mode

	do{
		printf("Cheat Mode (S/N): ");
		fgets(escCheat, 3, stdin);
		
		quebra_linha = strchr(escCheat, '\n');

		if(quebra_linha != NULL){
            *quebra_linha = '\0';
			if (!strcmp(escCheat, "S")){
				jogo.cheatMode = TRUE;
				cheatEscolhido = TRUE;
			}
			else{
				if (!strcmp(escCheat, "N")){
					jogo.cheatMode = FALSE;
					cheatEscolhido = TRUE;
				}
				else{
					printEscolhaErrada();
				}
			}
        }
        else{
            cleanBuffer();
            printEscolhaErrada();
        }

	} while (!cheatEscolhido);


    return jogo;
}

// Função para reajeitar as peças de um jogador

void reajeitarPecas(infoJogo *jogo, infoJogador *jogador){
    int j, pos;

    for (j = 0; j < N_PECAS; j++){
        if (!jogo->numPecas){
            break;
        }

        if (!strcmp(jogador->pecas[j], "\0")){
            do{
                pos = rand() % TOTAL_PECAS;
            } while (!jogo->quantPecas[pos]);

			// preenche a posição com uma peça aleatória e atualiza o número de peças

            jogo->numPecas--;
            jogo->quantPecas[pos]--;
            strcpy(jogador->pecas[j], tipoPecas[pos]);
        }
    }
}

// Função para a jogada de uma peça no tabuleiro

int jogarPeca(infoJogo *jogo, infoJogador *jogador, char peca[], int x, int y, int *x0, int *y0, char *esc, char pecaInicial[]){
    int j, pos, existe = FALSE;
	short primVez = FALSE;

	// verifica se a peça existe na mão do jogador, para assim retira-la posteriormente

	for (j = 0; j < N_PECAS; j++){
        if (!strcmp(peca, jogador->pecas[j])){
            existe = TRUE;
            pos = j;
            break;
        }
    }

	// verifica se a peça apresenta o mesmo x ou y, dependendo das peças anteriores

    if (*esc == 'x' && x != *x0){
        return -1;
    }

    if (*esc == 'y' && y != *y0){
        return -1;
    }

	// atribui, para a primeira peça jogada, x0 e y0 como as posições iniciais
	// para a segunda peça, determina se na jogada sera mantido x ou y

    if (*x0 == INF && *y0 == INF){
        *x0 = x;
        *y0 = y;
        strcpy(pecaInicial, peca);
    }
    else{
        if (*esc == '\0'){
			if (x != *x0 && y != *y0){
				return -1;
			}
            if (x == *x0){
                *esc = 'x';
				primVez = TRUE;
            }
            else{
                *esc = 'y';
				primVez = TRUE;
            }
        }
    }

	// verifica se a posição é aceita com essa peça

    if (!posicaoAceita(jogo->tabuleiro, x, y, *x0, *y0, peca, *esc)){
		if (primVez){
			*esc = '\0';
		}
        return -1;
    }

	// realoca a matriz e coloca a peça na posição
    
    realocarMatriz(&jogo->tabuleiro, x, y);
    colocarMatriz(&jogo->tabuleiro, peca, x, y);

	// se a peça existe na mão do jogador, retira a mesma da mão

	if (existe){
        strcpy(jogador->pecas[pos], "\0");
    }

    return 1;
}

// Função para a troca de peças

void trocarPecas(infoJogo *jogo, infoJogador *jogador, int *pecas, int quant){
    int i = 0, j, pos;

    while (i < quant){
        do{
            pos = rand() % TOTAL_PECAS;
        } while (!jogo->quantPecas[pos]);

		// seleciona uma peça aleatória e decrementa seu número no grupo de reposição

        (jogo->quantPecas[pos])--;

		// incrementa o número da peça a ser devolvida no grupo de reposição

        for (j = 0; j < TOTAL_PECAS; j++){
            if (!strcmp(jogador->pecas[pecas[i]], tipoPecas[j])){
                (jogo->quantPecas[j])++;
                break;
            }
        }

		// atribui a nova peça na posição a ser trocada

        strcpy(jogador->pecas[pecas[i++]], tipoPecas[pos]);
    }

    free(pecas);
}

// Função para o menu de jogadas dos jogadores

int menuJogada(infoJogo *jogo, infoJogador *jogador){
    char escolha[100], *quebra_linha, *frase, peca[3], pecaInicial[3], esc2 = '\0';
    short escolhaCorreta = FALSE, segundoMenu = FALSE, existe, existeCheat, continuar;
    int i, j, k, esc, tam, *pecas = NULL, x, y, x0, y0;
    int **posPecas, pPos = 0, score;

    do{
        if (!pecas){
            free(pecas);
        }
        tam = 0;
        x0 = INF;
        y0 = INF;
        x = INF;
        y = INF;

		// apresenta o tabuleiro e as informações do menu

        printTabuleiro(jogo->tabuleiro);
        printInfoJogadores(*jogo);

        printJogada(*jogador);

        escolhaCorreta = TRUE;
        fgets(escolha, 100, stdin);
        quebra_linha = strchr(escolha, '\n');
        if(quebra_linha != NULL) *quebra_linha = '\0';

		// verifica se a primeira letra da frase entrada é aceita

        if (escolha[0] == 't' || escolha[0] == 'p' || escolha[0] == 'j'){
            frase = strtok(escolha, " ");
            strcpy(peca, "\0");
            i = 0;

			// percorre a frase entrada a cada espaço encontrado

            while (frase != NULL){
				
				// verifica se é a primeira porção da frase condiz com as escolhas
				
                if (!i){
                    if (!strcmp(frase, "trocar")){
						if (jogo->numPecas){
							esc = 0;
						}
						else{
							printFaltaPecas();
							esc = -1;
							break;
						}
						
                    }
                    else if (!strcmp(frase, "jogar")){
                        esc = 1;
                    }
                    else if (!strcmp(frase, "passar")){
						quebra_linha = strchr(frase, '\0');
        				*quebra_linha = '1';

						// se já leu passar mas ainda apresentar porções da frase, apresenta erro

						if (!strcmp(frase, "passar1")){
							esc = 2;
							break;
						}
						else{
							esc = -1;
							printEscolhaErrada();
							break;
						}
                    }
                    else{
                        printEscolhaErrada();
                        esc = -1;
                        break;
                    }
                }

				// verifica se as outras porções da frase

                else{

					// se a primeira porção for trocar

                    if (!esc){

						// verifica se as peças digitadas apresentam-se na mão do jogador

                        strcpy(peca, frase);
                        peca[2] = '\0';

                        existe = FALSE;

                        for (j = 0; j < N_PECAS; j++){
                            continuar = TRUE;
                            for (k = 0; k < tam; k++){
                                if (j == pecas[k]){
                                    continuar = FALSE;
                                    break;
                                }
                            }

                            if (!continuar){
                                continue;
                            }

                            if (!strcmp(peca, jogador->pecas[j])){
                                if (!tam){
                                    pecas = (int*) malloc(sizeof(int));
                                }
                                else{
                                    pecas = (int*) realloc(pecas, (tam + 1) * sizeof(int));
                                }
                                existe = TRUE;
                                pecas[tam++] = j;
                                break;
                            }
                        }

                        if (!existe){
                            printPecaErrada(peca);
                            esc = -1;
                            break;
                        }
                    }

					// se a primeira porção da frase for jogar

                    else{

						// verifica se já leu a peça

                        if (!strcmp(peca, "\0")){

							// verifica se a peça apresenta-se na mão do jogador

                            strcpy(peca, frase);
                            peca[2] = '\0';
                            existe = FALSE;

                            for (j = 0; j < N_PECAS; j++){
                                if (!strcmp(peca, jogador->pecas[j])){
                                    existe = TRUE;
                                    break;
                                }
                            }

                            if (!existe){
								existeCheat = FALSE;

								if (jogo->cheatMode){
									for (j = 0; j < TOTAL_PECAS; j++){
										if (!strcmp(peca, tipoPecas[j])){
											existeCheat = TRUE;
											break;
										}
									}
								}

								if (!existeCheat){
									printPecaErrada2();
									esc = -1;
									break;
								}
                            }
                        }

						// se ja leu a peça

                        else{

							// se x e y ja forem salvos mas ainda apresentar porções da frase, apresenta erro

							if (x != INF && y != INF){
								esc = -1;
								printEscolhaErrada();
								break;
							}

							// salva x e depois y

                            if (x == INF){
                                x = atoi(frase);
                            }
                            else{
                                y = atoi(frase);
                            }
                        }
                    }
                }
                i++;

                frase = strtok(NULL, " ");
            }

			// verifica a escolha do jogador

            switch (esc){

				// se erro

                case -1:
                    escolhaCorreta = FALSE;
                    continue;
                break;

				// se trocar peças

                case 0:

					// verifica se ainda há peças para poder trocar

					if (jogo->numPecas >= tam){

						// troca as peças

						trocarPecas(jogo, jogador, pecas, tam);
						if (!pecas){
							free(pecas);
						}
						printPontJogadorRodada(*jogador);
						return 0;
					}
					else{
						printFaltaAlgumasPecas(jogo->numPecas);
						escolhaCorreta = FALSE;
					}
                break;

				// se jogar peça

                case 1:

					// tenta jogar a peça

                    k = jogarPeca(jogo, jogador, peca, x, y, &x0, &y0, &esc2, pecaInicial);

					// se a posição não for possivel

                    if (k == -1){
                        printPosicaoErrada();
                        escolhaCorreta = FALSE;
                    }

					// se possivel jogar, salva a posição da peça

                    else{
                        segundoMenu = TRUE;
                        posPecas = (int**) malloc(sizeof(int*));
                        posPecas[pPos] = (int*) malloc(2 * sizeof(int));
                        posPecas[pPos][0] = x;
                        posPecas[pPos++][1] =  y;
                    }
                break;

				// se passar

                case 2:

					// apresenta a pontuação e sai da função

                    printPontJogadorRodada(*jogador);
					return 1;
                break;
            }
        }
        else{
            printEscolhaErrada();
            escolhaCorreta = FALSE;
        }

    } while (!escolhaCorreta);

	// segundo menu caso escolha jogar peça

    while (segundoMenu){

		// verifica se o jogador ainda apresenta peças na mão

		if (!verNumPecas(*jogador)){

			// atribui os score, reajeita as peças e sai da função

			jogador->scoreRodada = pontuar(jogo->tabuleiro, posPecas, pPos);
			jogador->score += jogador->scoreRodada;
			printPontJogadorRodada(*jogador);
			jogador->scoreRodada = 0;
			if (jogo->numPecas){
				reajeitarPecas(jogo, jogador);
			}
			return 0;
		}

        x = INF;
        y = INF;

		// apresenta o tabuleiro e as informações do menu

        printTabuleiro(jogo->tabuleiro);
        printInfoJogadores(*jogo);

        printSegundaJogada(*jogador);

        fgets(escolha, 100, stdin);
        quebra_linha = strchr(escolha, '\n');
        if(quebra_linha != NULL) *quebra_linha = '\0';

		// verifica se a primeira letra da frase entrada é aceita

        if (escolha[0] == 'p' || escolha[0] == 'j'){
            frase = strtok(escolha, " ");
            strcpy(peca, "\0");
            i = 0;

			// percorre a frase entrada a cada espaço encontrado

            while (frase != NULL){

				// verifica se é a primeira porção da frase condiz com as escolhas

                if (!i){
                    if (!strcmp(frase, "jogar")){
                        esc = 1;
                    }
                    else if (!strcmp(frase, "passar")){
                        quebra_linha = strchr(frase, '\0');
        				*quebra_linha = '1';

						// se já leu passar mas ainda apresentar porções da frase, apresenta erro

						if (!strcmp(frase, "passar1")){
							esc = 2;
							break;
						}
						else{
							esc = -1;
							printEscolhaErrada();
							break;
						}
                    }
                    else{
                        printEscolhaErrada();
                        esc = -1;
                        break;
                    }
                }

                // verifica se as outras porções da frase

                else{

					// verifica se já leu a peça

                    if (!strcmp(peca, "\0")){

						// verifica se a peça apresenta-se na mão do jogador

                        strcpy(peca, frase);
                        peca[2] = '\0';
                        existe = FALSE;

                        for (j = 0; j < N_PECAS; j++){
                            if (!strcmp(peca, jogador->pecas[j])){
                                existe = TRUE;
                                break;
                            }
                        }

						if (!existe){
							existeCheat = FALSE;

							if (jogo->cheatMode){
								for (j = 0; j < TOTAL_PECAS; j++){
									if (!strcmp(peca, tipoPecas[j])){
										existeCheat = TRUE;
										break;
									}
								}
							}

							if (!existeCheat){
								printPecaErrada2();
								esc = -1;
								break;
							}
						}
                    }

                    // se ja leu a peça

					else{

						// se x e y ja forem salvos mas ainda apresentar porções da frase, apresenta erro

						if (x != INF && y != INF){
							esc = -1;
							printEscolhaErrada();
							break;
						}

						// salva x e depois y

                        if (x == INF){
                            x = atoi(frase);
                        }
                        else{
                            y = atoi(frase);
                        }
                    }
                }
                i++;

                frase = strtok(NULL, " ");
            }

			// verifica a escolha do jogador

            switch (esc){

				// se jogar peça

                case 1:

					// tenta jogar a peça

                    k = jogarPeca(jogo, jogador, peca, x, y, &x0, &y0, &esc2, pecaInicial);

					// se a posição não for possivel
                    
                    if (k == -1){
                        printPosicaoErrada();
                    }

					// se possivel jogar, salva a posição da peça

                    else{
                        posPecas = (int**) realloc(posPecas, (pPos + 1) * sizeof(int*));
                        posPecas[pPos] = (int*) malloc(2 * sizeof(int));
                        posPecas[pPos][0] = x;
                        posPecas[pPos++][1] =  y;
                    }
                break;

				// se passar

                case 2:

					// atribui a pontuação, apresenta a mesma, reajeita as peças e sai da função

                    jogador->scoreRodada = pontuar(jogo->tabuleiro, posPecas, pPos);
                    jogador->score += jogador->scoreRodada;
                    printPontJogadorRodada(*jogador);
                    jogador->scoreRodada = 0;
                    if (jogo->numPecas){
                        reajeitarPecas(jogo, jogador);
                    }
					return 0;
                break;
            }
        }
        else{
            printEscolhaErrada();
        }
    }

	return 0;
}

// Função para liberar a memória

void limpar(infoJogo *jogo){
    int i, j;

    free(jogo->jogadores);

    for (i = 0; i <= jogo->tabuleiro.maxY - jogo->tabuleiro.minY; i++){
        for (j = 0; j <= jogo->tabuleiro.maxX - jogo->tabuleiro.minX; j++){
            free(jogo->tabuleiro.local[i][j]);
        }
        free(jogo->tabuleiro.local[i]);
    }

    free(jogo->tabuleiro.local);
}

// Função para ver o vencedor

void verVencedor(infoJogo jogo){
	int i, *pos, p = 0;
	int maior = -1;

	// percorre os jogadores e salva as posições daqueles com a maior pontuação

	for (i = 0; i < jogo.numJogadores; i++){
		if (jogo.jogadores[i].score > maior){
			p = 0;
			if (!pos){
				free(pos);
			}

			pos = (int*) malloc(sizeof(int));
			pos[p++] = i;

			maior = jogo.jogadores[i].score;
		}
		else if (jogo.jogadores[i].score == maior){
			pos = (int*) realloc(pos, (p + 1) * sizeof(int));
			pos[p++] = i;
		}
	}

	// dependendo do numero de vencedores, chama a função do vencedor ou de empate

	if (p > 1){
		printVencedores(jogo, pos, p);
	}
	else{
		printVencedor(jogo.jogadores[pos[0]]);
	}
}

// Função para comandar o programa

void menu(){
    int i = 0;
	int passadas = 0, vez;
    infoJogo jogo;

	// inicia as informações do jogo

    jogo = definirJogadores();
    jogo.fim = FALSE;
    jogo.numPecas = 108 - (6 * jogo.numJogadores);

    iniciarTabuleiro(&jogo.tabuleiro);

	// laço que mantem o jogo funcionando

    while (!jogo.fim){

		// menu do jogador

		if (verNumPecas(jogo.jogadores[i])){
			vez = menuJogada(&jogo, &jogo.jogadores[i++]);
		}
		else{
			i++;
			vez = 1;
		}

		if (!vez){
			passadas = 0;
		}
		else{
			passadas++;
		}

		if (!jogo.numPecas){
			if (passadas == jogo.numJogadores){
				jogo.fim = TRUE;
			}
		}

        if (i == jogo.numJogadores){
            i = 0;
        }
    }

	// ve os vencedores e limpa a memoria

	verVencedor(jogo);

    limpar(&jogo);
}