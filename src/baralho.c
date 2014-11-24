#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "baralho.h"

static char intValToChar(int valor);
static int charValToInt(char valor);
static int getNaipeIndex(char naipe);

static FILE *arq;
static int separadorVertical = 0, separadorHorizontal = 0, bordas[4] = {0,0,0,0};
static char firstCarta, naipes[4];
static Cabecalho cabecalho;

Baralho init(char *imgFile, int cw, int ch, char primCarta, char *ordNaipes) {
    Baralho baralho; // Cartas[NAIPE][CARTA];
    int i, j;
    if(!(arq = fopen(imgFile, "r"))) {
        fputs("* Erro: Não foi possível abrir o arquivo da imagem.\n", stderr);
        return NULL;
    }
    baralho = calloc(NAIPES, sizeof(Carta *));
    if(!baralho) {
        fputs("* Erro: Não foi possível alocar as estruturas de dados.\n", stderr);
        return NULL;
    }
    firstCarta = primCarta;
    for(i = 0; i < NAIPES; i++) {
        naipes[i] = ordNaipes[i];
        baralho[i] = calloc(CARTAS, sizeof(Carta));
        if(!baralho[i]) {
            fputs("* Erro: Não foi possível alocar as estruturas de dados.\n", stderr);
            return NULL;
        }
        for(j = 0; j < CARTAS; j++) {
            baralho[i][j].naipe = naipes[i];
            baralho[i][j].valor = intValToChar(j+1);
            baralho[i][j].img = calloc(sizeof(Pixel), cw * ch);
            if(!baralho[i][j].img) {
                fputs("* Erro: Não foi possível alocar espaço para as cartas.\n", stderr);
                return NULL;
            }
        }
    }
    cabecalho.w = cw;
    cabecalho.h = ch;
    cabecalho.maxval = 255;
    return baralho;
}

Baralho geraBaralho(Baralho baralho) {
    char filetype[4];
    int larg, alt, maxval;
    long opos;
    fgets(filetype, 4, arq);
    if(strcmp(filetype, "P6\n") || fscanf(arq, "%d %d", &larg, &alt) != 2 || fscanf(arq, "%d", &maxval) != 1) {
        fputs("* Erro: Arquivo PPM errado e/ou corrompido.\n", stderr);
        return NULL;
    }
    if((larg - bordas[BRDLFT] - bordas[BRDRGT] - cabecalho.w * CARTAS) % (CARTAS-1) != 0) {
        fputs("* Erro: Separadores horizontais inconsistentes.\n", stderr);
        return NULL;
    }
    if((alt - bordas[BRDTOP] - bordas[BRDBTM] - cabecalho.h * NAIPES) % (NAIPES-1) != 0) {
        fputs("* Erro: Separadores verticais inconsistentes.\n", stderr);
        return NULL;
    }
    separadorHorizontal = (larg - bordas[BRDLFT] - bordas[BRDRGT] - cabecalho.w * CARTAS) / (CARTAS-1);
    separadorVertical = (alt - bordas[BRDTOP] - bordas[BRDBTM] - cabecalho.h * NAIPES) / (NAIPES-1);
    fseek(arq, 1, SEEK_CUR); // Consome o '\n' após o MAXVAL do cabeçalho.
    opos = ftell(arq); // Salva o cursor do arquivo logo no começo do corpo da imagem (pós-cabeçalho)
    int i, j, k, p;
    for(i = 0; i < NAIPES; i++) {
        for(j = 0; j < CARTAS; j++) {
            int indice = (charValToInt(firstCarta) - 1 + j) % 13;
            fseek(arq, opos, SEEK_SET); // Leva o cursor do arquivo para o começo do corpo da imagem
            fseek(arq, 3 * (bordas[BRDTOP] * larg + i * (cabecalho.h + separadorVertical) * larg
                        + j * (cabecalho.w + separadorHorizontal) + bordas[BRDLFT]), SEEK_CUR);
            for(k = 0; k < cabecalho.h; k++) {
                for(p = 0; p < cabecalho.w; p++) {
                    baralho[i][indice].img[p + cabecalho.w * k].R = fgetc(arq);
                    baralho[i][indice].img[p + cabecalho.w * k].G = fgetc(arq);
                    baralho[i][indice].img[p + cabecalho.w * k].B = fgetc(arq);
                }
                fseek(arq, 3 * (larg - cabecalho.w), SEEK_CUR);
            }
        }
    }
    return baralho;
}

Carta getCarta(char naipe, char valor, Baralho baralho) {
    return baralho[getNaipeIndex(naipe)][charValToInt(valor) - 1];
}

void geraCarta(char naipe, char valor, Baralho baralho) {
    if(!baralho) {
        fputs("* Erro: Tentando acessar um baralho não inicializado.\n", stderr);
        return;
    }
    FILE *fileCarta;
    char filename[8];
    int i, j, indexNaipe = getNaipeIndex(naipe), indexValor = charValToInt(valor) - 1;
    if(indexNaipe == -1 || indexValor < 0 || indexValor > CARTAS-1) {
        fputs("* Erro: Valor e/ou naipe inválidos.\n", stderr);
        return;
    }
    sprintf(filename, "%c-%c.ppm", valor, naipe);
    if(!(fileCarta = fopen(filename, "w"))) {
        fputs("* Erro: Não foi possível criar o arquivo da carta.\n", stderr);
        return;
    }
    fprintf(fileCarta, "P6\n%d %d\n%d\n", cabecalho.w, cabecalho.h, cabecalho.maxval);
    for(i = 0; i < cabecalho.h; i++) {
        for(j = 0; j < cabecalho.w; j++) {
            Pixel pxl = baralho[indexNaipe][indexValor].img[j + cabecalho.w * i];
            fprintf(fileCarta, "%c%c%c", pxl.R, pxl.G, pxl.B);
        }
    }
    fclose(fileCarta);
}

Monte geraMonteVazio() {
    Monte monte = calloc(1, sizeof(char *));
    return monte;
}
   
Monte geraMonte(Baralho baralho, int n) {
    Monte monte = calloc((NAIPES * CARTAS * n) + 1, sizeof(char *));
    int i, j, k, index;
    for(i = 0; i < NAIPES; i++) {
        for(j = 0; j < CARTAS; j++) {
            for(k = 0; k < n; k++) {
                index = k + n*j + n*CARTAS*i;
                monte[index] = calloc(3, sizeof(char));
                sprintf(monte[index], "%c%c", baralho[i][j].valor, baralho[i][j].naipe);
            }
        }
    }
    monte[NAIPES * CARTAS * n] = NULL;
    return monte;
}

Monte embaralha(Monte monte) {
    if(getMonteSize(monte) < 2) return monte; //Não é necessário embaralhar montes vazios ou unitários.
    int i, monteSize = getMonteSize(monte);
    for(i = 0; i < 2000; i++) {
        int r1, r2;
        char tmp[3];
        do {
            r1 = rand() % monteSize;
            r2 = rand() % monteSize;
        } while(r1 == r2);
        strcpy(tmp, monte[r1]);
        strcpy(monte[r1], monte[r2]);
        strcpy(monte[r2], tmp);
    }
    return monte;
}

Carta tiraCarta(Monte* pmonte, Baralho baralho) {
    int i;
    Monte deck = *pmonte; //Variável para ajudar na legibilidade
    Carta carta = getCarta(deck[0][1], deck[0][0], baralho);
    for(i = 1; deck[i-1] != NULL; i++) {
        if(deck[i] != NULL) strcpy(deck[i-1], deck[i]);
        else {
            free(deck[i-1]);
            deck[i-1] = NULL;
        }
    }
    *pmonte = realloc(*pmonte, sizeof(char **) * (getMonteSize(*pmonte) + 1)); // Realoca espaço para as strings + 1 NULL.
    return carta;
}

int removeCarta(Monte *pmonte, char valor, char naipe) {
    int i, deckSize, sucesso=0;
    char str[3];
    if((deckSize = getMonteSize(*pmonte)) == 0) return 0;
    Monte deck = *pmonte;
    sprintf(str, "%c%c", valor, naipe);
    for(i = 0; !sucesso && i < deckSize; i++) {
        if(!strcmp(deck[i], str)) sucesso = 1;

    }
    if(!sucesso) return 0;
    for(; i <= deckSize; i++) {
        if(deck[i] != NULL) strcpy(deck[i-1], deck[i]);
        else {
            free(deck[i-1]);
            deck[i-1] = NULL;
        }
    }
    *pmonte = realloc(*pmonte, sizeof(char **) * (getMonteSize(*pmonte) + 1));
    return 1;
}

void insereCarta(Monte* pmonte, Carta carta) {
    int i, oldSize = getMonteSize(*pmonte);
    *pmonte = realloc(*pmonte, sizeof(char**) * (oldSize + 2)); // Aloca espaço para uma carta (e um NULL).
    if(!*pmonte) {
        fputs("* Ocorreu um erro na hora de alocar a memória. Finalizando.\n", stderr);
        exit(0);
    }
    Monte deck = *pmonte;
    deck[oldSize+1] = NULL;
    for(i = oldSize; i > 0; i--) {
        if(deck[i] == NULL) deck[i] = realloc(deck[i], sizeof(char) * 3);
        if(deck[i-1] != NULL) strcpy(deck[i], deck[i-1]);
        else deck[i] = NULL;
    }
    if(deck[0] == NULL) deck[0] = malloc(sizeof(char) * 3);
    sprintf(deck[0], "%c%c", carta.valor, carta.naipe);
}

int isNextCard(Carta x, Carta y) {
    if(charValToInt(x.valor) + 1 == charValToInt(y.valor) && x.naipe == y.naipe) return 1;
    else return 0;
}

int isPreviousCard(Carta x, Carta y) {
    if(charValToInt(x.valor) == charValToInt(y.valor) + 1 && x.naipe == y.naipe) return 1;
    else return 0;
}

int getMonteSize(Monte deck) {
    int size;
    for(size = 0; deck[size] != NULL; size++);
    return size;
}

void setBordas(int top, int right, int bottom, int left) {
    bordas[BRDTOP] = top;
    bordas[BRDRGT] = right;
    bordas[BRDBTM] = bottom;
    bordas[BRDLFT] = left;
}

static char intValToChar(int valor) {
    if(valor < 1 || valor > 13) return 0;
    switch(valor) {
        case 1: return 'A';
        case 10: return 'D';
        case 11: return 'J';
        case 12: return 'Q';
        case 13: return 'K';
        default: return '0' + valor;
    }
}

static int charValToInt(char valor) {
    switch(valor) {
        case 'A': return 1;
        case 'D': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        default: return valor - '0';
    }
}

static int getNaipeIndex(char naipe) {
    int i;
    for(i = 0; i < 4; i++) {
        if(naipes[i] == naipe) return i;
    }
    return -1;
}
