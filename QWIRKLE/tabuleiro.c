#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/jogo.h"

// definição de max e min de dois numeros

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

// inicia o tabuleiro

void iniciarTabuleiro(infoTab *tab){
    tab->minX = 0;
    tab->minY = 0;
    tab->centroX = 0;
    tab->centroY = 0;
    tab->maxX = 0;
    tab->maxY = 0;

    tab->local = (char***) malloc(sizeof(char**));
    tab->local[0] = (char**) malloc(sizeof(char*));
    tab->local[0][0] = (char*) malloc(3 * sizeof(char));
    strcpy(tab->local[0][0], "\0");
}

// pontua a jogada

int pontuar(infoTab tab, int **pos, int p){
    int score = 0, scoreAnt;
    int *xMais, *xMenos, *yMais, *yMenos;
    int pXMais = 0, pXMenos = 0, pYMais = 0, pYMenos = 0;
    int i, j, k, l;
    int jaSomouX, jaSomouY;
    short pular, ignorarX = FALSE, ignorarY = FALSE;

	// verifica se é referente a primeira jogada com somente uma peça

	if (p == 1){
		if (pos[0][0] == 0 && pos[0][1] == 0){
			return 1;
		}
	}

	// se não, verifica se ignora as porções x ou y a partir das peças

	else{
		if (pos[0][0] == pos[1][0]){
			ignorarX = TRUE;
		}
		else{
			ignorarY = TRUE;
		}
	}

	// percorre todas as posições das peças jogadas

    for (i = 0; i < p; i++){

        pular = FALSE;

		// verifica se a posição deve ser ignorada em x

        if (!ignorarX){

			// se não, verifica se a posição deve ser ignorada em x mais, uma vez que já foi contabilizado por outra peça

            for (k = 0; k < pXMenos; k++){
                if (pos[i][0] == xMenos[k]){
                    pular = TRUE;
                    break;
                }
            }

            if (!pular){
                for (k = 0; k < pXMais; k++){
                    if (pos[i][0] == xMais[k]){
                        pular = TRUE;
                        break;
                    }
                }
            }
        }

        jaSomouX = 0;

        if (!pular){

			// se não pular, percorre os elementos em x mais somando no score

            scoreAnt = score;
            for (j = pos[i][0]; j <= tab.maxX; j++){
                if (strcmp(tab.local[tab.centroY + pos[i][1]][tab.centroX + j], "\0")){
                    score++;

                    if (i != p - 1 && !ignorarX){
                        for (k = i + 1; k < p; k++){
                            if (j == pos[k][0]){
                                if (!pXMais){
                                    xMais = (int*) malloc(sizeof(int));
                                }
                                else{
                                    xMais = (int*) realloc(xMais, (pXMais + 1) * sizeof(int));
                                }
                                xMais[pXMais++] = j;
                            }
                        }
                    }
                }
                else{

					// se não ouver nenhuma peça, não contabilizar

                    if (j == pos[i][0] + 1){
                        score--;
                    }
					if (score - scoreAnt == 6){
                        score += 6;
                    }
                    jaSomouX = score - scoreAnt;
                    break;
                }
            }
        }

        pular = FALSE;

		// verifica se a posição deve ser ignorada em x

        if (!ignorarX){

			// se não, verifica se a posição deve ser ignorada em x menos, uma vez que já foi contabilizado por outra peça

            for (k = 0; k < pXMais; k++){
                if (pos[i][0] == xMais[k]){
                    pular = TRUE;
                    break;
                }
            }

            if (!pular){
                for (k = 0; k < pXMenos; k++){
                    if (pos[i][0] == xMenos[k]){
                        pular = TRUE;
                        break;
                    }
                }
            }
        }

        if (!pular){

			// se não pular, percorre os elementos em x menos somando no score

            scoreAnt = score;
            for (j = pos[i][0]; j >= tab.minX; j--){
                if (strcmp(tab.local[tab.centroY + pos[i][1]][tab.centroX + j], "\0")){
                    score++;

                    if (i != p - 1 && !ignorarX){
                        for (k = i + 1; k < p; k++){
                            if (j == pos[k][0]){
                                if (!pXMenos){
                                    xMenos = (int*) malloc(sizeof(int));
                                }
                                else{
                                    xMenos = (int*) realloc(xMenos, (pXMenos + 1) * sizeof(int));
                                }
                                xMenos[pXMenos++] = j;
                            }
                        }
                    }
                }
                else{
                    if (jaSomouX && score - scoreAnt != 1){
                        score--;
                    }

					// se não ouver nenhuma peça, não contabilizar

                    if (j == pos[i][0] - 1){
                        score--;
                    }
					if (score - scoreAnt + jaSomouX == 6){
                        score += 6;
                    }
                    break;
                }
            }
        }

        pular = FALSE;

		// verifica se a posição deve ser ignorada em y

        if (!ignorarY){

			// se não, verifica se a posição deve ser ignorada em y mais, uma vez que já foi contabilizado por outra peça

            for (k = 0; k < pYMenos; k++){
                if (pos[i][1] == yMenos[k]){
                    pular = TRUE;
                    break;
                }
            }

            if (!pular){
                for (k = 0; k < pYMais; k++){
                    if (pos[i][1] == yMais[k]){
                        pular = TRUE;
                        break;
                    }
                }
            }
        }

        jaSomouY = 0;


        if (!pular){

			// se não pular, percorre os elementos em y mais somando no score

            scoreAnt = score;
            for (j = pos[i][1]; j <= tab.maxY; j++){
                if (strcmp(tab.local[tab.centroY + j][tab.centroX + pos[i][0]], "\0")){
                    score++;

                    if (i != p - 1 && !ignorarY){
                        for (k = i + 1; k < p; k++){
                            if (j == pos[k][1]){
                                if (!pYMais){
                                    yMais = (int*) malloc(sizeof(int));
                                }
                                else{
                                    yMais = (int*) realloc(yMais, (pYMais + 1) * sizeof(int));
                                }
                                yMais[pYMais++] = j;
                            }
                        }
                    }
                }
                else{

					// se não ouver nenhuma peça, não contabilizar

                    if (j == pos[i][1] + 1){
                        score--;
                    }
					if (score - scoreAnt == 6){
                        score += 6;
                    }
                    jaSomouY = score - scoreAnt;
                    break;
                }
            }
        }

        pular = FALSE;

		// verifica se a posição deve ser ignorada em y

        if (!ignorarY){

			// se não, verifica se a posição deve ser ignorada em y menos, uma vez que já foi contabilizado por outra peça

            for (k = 0; k < pYMais; k++){
                if (pos[i][1] == yMais[k]){
                    pular = TRUE;
                    break;
                }
            }

            if (!pular){
                for (k = 0; k < pYMenos; k++){
                    if (pos[i][1] == yMenos[k]){
                        pular = TRUE;
                        break;
                    }
                }
            }
        }

        if (!pular){

			// se não pular, percorre os elementos em y menos somando no score

            scoreAnt = score;
            for (j = pos[i][1]; j >= tab.minY; j--){
                if (strcmp(tab.local[tab.centroY + j][tab.centroX + pos[i][0]], "\0")){
                    score++;

                    if (i != p - 1 && !ignorarY){
                        for (k = i + 1; k < p; k++){
                            if (j == pos[k][1]){
                                if (!pYMenos){
                                    yMenos = (int*) malloc(sizeof(int));
                                }
                                else{
                                    yMenos = (int*) realloc(yMenos, (pYMenos + 1) * sizeof(int));
                                }
                                yMenos[pYMenos++] = j;
                            }
                        }
                    }
                }
                else{
                    if (jaSomouY && score - scoreAnt != 1){
                        score--;
                    }

					// se não ouver nenhuma peça, não contabilizar

                    if (j == pos[i][1] - 1){
                        score--;
                    }
					if (score - scoreAnt + jaSomouY == 6){
                        score += 6;
                    }
                    break;
                }
            }
        }
    }

	// libera as posições salvas na função

    if (!xMais){
        free(xMais);
    }

    if (!xMenos){
        free(xMenos);
    }

    if (!yMais){
        free(yMais);
    }

    if (!yMenos){
        free(yMenos);
    }

    for (i = 0; i < p; i++){
        free(pos[i]);
    }

    free(pos);

    return score;
}

// verifica se a peça faz-se condizente com as peças na mesma linha

int compPecas(int pos, char pecas[5][3], char peca[]){
    int i;
    short letra = FALSE;
    short numero = FALSE;

    for (i = 0; i < pos; i++){

		// inicialmente salva se a letra ou o numero são os mesmos
		// se nenhum dos dois for igual, ou os dois forem iguais, retorna 0

        if (!letra && !numero){
            if (peca[0] == pecas[i][0]){
                letra = TRUE;
            }
            if (peca[1] == pecas[i][1]){
                numero = TRUE;
            }

            if (!letra && !numero){
                return 0;
            }
        }
        else{
            if (letra && numero){
                return 0;
            }
            else{
                if (letra){
                    if (peca[0] != pecas[i][0] || peca[1] == pecas[i][1]){
                        return 0;
                    }
                }
                else{
                    if (peca[1] != pecas[i][1] || peca[0] == pecas[i][0]){
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

// verifica se a peça em tal posição é valida

int posicaoAceita(infoTab tab, int x, int y, int x0, int y0, char peca[], char esc){
    int i, pos;
    char pecas[5][3];
    short contorno = FALSE;

	// verifica no tamanho da matriz e se a posição esta livre
    
    if (x < tab.minX || x > tab.maxX || y < tab.minY || y > tab.maxY || strcmp(tab.local[tab.centroY + y][tab.centroX + x], "\0")){
        return 0;
    }

    if (x == 0 && y == 0){
        return 1;
    }

	// verifica, com base de serem todas as peças ja jogadas com x ou y iguais se a mesma se faz condizente

    if (esc == 'x'){
        if (y < y0){
            for (i = y + 1; i <= y0; i++){
                if (!strcmp(tab.local[tab.centroY + i][tab.centroX + x], "\0")){
                    return 0;
                }
            }
        }
        else{
            for (i = y - 1; i >= y0; i--){
                if (!strcmp(tab.local[tab.centroY + i][tab.centroX + x], "\0")){
                    return 0;
                }
            }
        }
    }

    if (esc == 'y'){
        if (x < x0){
            for (i = x + 1; i <= x0; i++){
                if (!strcmp(tab.local[tab.centroY + y][tab.centroX + i], "\0")){
                    return 0;
                }
            }
        }
        else{
            for (i = x - 1; i >= x0; i--){
                if (!strcmp(tab.local[tab.centroY + y][tab.centroX + i], "\0")){
                    return 0;
                }
            }
        }
    }

	// verifica se existe uma peça ao entorno da posição

    if (x != tab.minX){
        if (strcmp(tab.local[tab.centroY + y][tab.centroX + x - 1], "\0")){
            contorno = TRUE;
        }
    }

    if (x != tab.maxX){
        if (strcmp(tab.local[tab.centroY + y][tab.centroX + x + 1], "\0")){
            contorno = TRUE;
        }
    }

    if (y != tab.minY){
        if (strcmp(tab.local[tab.centroY + y - 1][tab.centroX + x], "\0")){
            contorno = TRUE;
        }
    }

    if (y != tab.maxY){
        if (strcmp(tab.local[tab.centroY + y + 1][tab.centroX + x], "\0")){
            contorno = TRUE;
        }
    }
    
    if (!contorno){
        return 0;
    }

    pos = 0;

	// para cada lado possível, verifica se a peça apresenta a mesma letra ou número

    if (x != tab.maxX){
        for (i = x + 1; i <= tab.maxX; i++){
            if (strcmp(tab.local[tab.centroY + y][tab.centroX + i], "\0")){
                strcpy(pecas[pos++], tab.local[tab.centroY + y][tab.centroX + i]);
            }
            else{
                break;
            }
        }
    }

    if (pos){
        if (!compPecas(pos, pecas, peca)){
            return 0;
        }
    }

    pos = 0;

    if (x != tab.minX){
        for (i = x - 1; i >= tab.minX; i--){
            if (strcmp(tab.local[tab.centroY + y][tab.centroX + i], "\0")){
                strcpy(pecas[pos++], tab.local[tab.centroY + y][tab.centroX + i]);
            }
            else{
                break;
            }
        }
    }

    if (pos){
        if (!compPecas(pos, pecas, peca)){
            return 0;
        }
    }

    pos = 0;

    if (y != tab.maxY){
        for (i = y + 1; i <= tab.maxY; i++){
            if (strcmp(tab.local[tab.centroY + i][tab.centroX + x], "\0")){
                strcpy(pecas[pos++], tab.local[tab.centroY + i][tab.centroX + x]);
            }
            else{
                break;
            }
        }
    }

    if (pos){
        if (!compPecas(pos, pecas, peca)){
            return 0;
        }
    }

    pos = 0;

    if (y != tab.minY){
        for (i = y - 1; i >= tab.minY; i--){
            if (strcmp(tab.local[tab.centroY + i][tab.centroX + x], "\0")){
                strcpy(pecas[pos++], tab.local[tab.centroY + i][tab.centroX + x]);
            }
            else{
                break;
            }
        }
    }

    if (pos){
        if (!compPecas(pos, pecas, peca)){
            return 0;
        }
    }

    return 1;
}

// Função para colocar a peça na posição

void colocarMatriz(infoTab *tab, char peca[], int x, int y){
    strcpy(tab->local[tab->centroY + y][tab->centroX + x], peca);
}

// realoca a matriz se necessário

void realocarMatriz(infoTab *tab, int x, int y){
    int i, j;
    int antY = tab->maxY - tab->minY + 1;
    int antX = tab->maxX - tab->minX + 1;
    short andarY = FALSE;
    short andarX = FALSE;

	// verifica se a porção y da posição encontra-se na borda do tabuleiro

    if (y == tab->minY){
        (tab->minY)--;
        (tab->centroY)++;
        andarY = TRUE;
    }
    if (y == tab->maxY){
        (tab->maxY)++;
    }

	// realoca as linhas da matriz

    tab->local = (char***) realloc(tab->local, (tab->maxY - tab->minY + 1) * sizeof(char**));

    for (i = antY; i <= tab->maxY - tab->minY; i++){
        tab->local[i] = (char**) malloc((tab->maxX - tab->minX + 1) * sizeof(char*));

        for (j = 0; j <= tab->maxX - tab->minX; j++){
            tab->local[i][j] = (char*) malloc(3 * sizeof(char));
            strcpy(tab->local[i][j], "\0");
        }
    }

	// se necessário, disloca os dados do tabuleiro para manter as posições relativas ao centro

    if (andarY){
        for (i = tab->maxY - tab->minY - 1; i >= 0; i--){
            for (j = 0; j <= tab->maxX - tab->minX; j++){
                if (i == 0){
                    strcpy(tab->local[i][j], "\0");
                }
                else{
                    strcpy(tab->local[i][j], tab->local[i - 1][j]);
                }
            }
        }
    }

	// verifica se a porção x da posição encontra-se na borda do tabuleiro

    if (x == tab->minX){
        (tab->minX)--;
        (tab->centroX)++;
        andarX = TRUE;
    }
    if (x == tab->maxX){
        (tab->maxX)++;
    }

	// realoca as colunas da matriz

    for (i = 0; i <= tab->maxY - tab->minY; i++){
        tab->local[i] = (char**) realloc(tab->local[i], (tab->maxX - tab->minX + 1) * sizeof(char*));

        for (j = antX; j <= tab->maxX - tab->minX; j++){
            tab->local[i][j] = (char*) malloc(3 * sizeof(char));
            strcpy(tab->local[i][j], "\0");
        }
    }

	// se necessário, disloca os dados do tabuleiro para manter as posições relativas ao centro

    if (andarX){
        for (i = 0; i <= tab->maxY - tab->minY; i++){
            for (j = tab->maxX - tab->minX - 1; j >= 0; j--){
                if (j == 0){
                    strcpy(tab->local[i][j], "\0");
                }
                else{
                    strcpy(tab->local[i][j], tab->local[i][j - 1]);
                }
            }
        }
    }
}