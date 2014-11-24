#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "baralho.h"

int main() {
    chdir("..");
    char *dir = getcwd(NULL, 0), cmd, subcmd, naipe, valor;
    int i;
    if(dir) printf("* Diretório corrente: %s\n", dir);
    free(dir); 
    setBordas(0, 1, 0, 0);
    Baralho baralho = init("baralho.ppm", 71, 96, '2', "PECO");
    if(!baralho) {
        fputs("* Ocorreu um erro fatal em init().\n", stderr);
        return 0;
    }
    baralho = geraBaralho(baralho);
    if(!baralho) {
        fputs("* Ocorreu um erro fatal em geraBaralho().\n", stderr);
        return 0;
    }
    Monte monte = geraMonte(baralho, 2);
    printf("==============================\nLIBBARALHO: Demonstração.\n============================\n\n");
    printf("* Um monte NÃO embaralhado com 104 (2x52) cartas foi criado.\n");
    do {
        Carta tmp;
        printf("* O que deseja fazer?\n[R] Retirar uma carta.\n[I] Inserir uma carta.\n[E] Embaralha o monte.\n[P] Imprime o conteúdo do monte.\n[X] Remove uma carta especifica.\n[S] Finalizar o teste.\nOpção: ");
        cmd = getchar();
        if(cmd != '\n') while(getchar() != '\n');
        switch(cmd) {
            case 'r':
            case 'R':
                tmp = tiraCarta(&monte, baralho);
                printf("* Você puxou uma carta %c-%c. Deseja gerar um arquivo .ppm na raiz do projeto? [S/n]: ", tmp.valor, tmp.naipe);
                subcmd = getchar();
                if(subcmd != '\n') while(getchar() != '\n');
                if(subcmd == 'S' || subcmd == 's') geraCarta(tmp.naipe, tmp.valor, baralho);
                printf("* O monte possuí agora %d cartas.\n", getMonteSize(monte));
                break;
            case 'i':
            case 'I':
                printf("* Insira o valor da carta ['A', '2' - '9', 'D', 'J', 'Q', 'R']: ");
                valor = getchar();
                while(getchar() != '\n');
                printf("* Insira o naipe da carta ['P', 'E', 'C', 'O']: ");
                naipe = getchar();
                while(getchar() != '\n');
                tmp = getCarta(toupper(naipe), toupper(valor), baralho);
                insereCarta(&monte, tmp);
                printf("* Você inseriu a carta %c-%c no monte. Ele possui %d cartas agora.\n", tmp.valor, tmp.naipe, getMonteSize(monte));
                break;
            case 'e':
            case 'E':
                embaralha(monte);
                printf("* O monte foi embaralhado.\n");
                break;
            case 'p':
            case 'P':
                for(i = 0; monte[i] != NULL; i++) {
                    printf("%c-%c", monte[i][0], monte[i][1]);
                    if(monte[i+1] != NULL) printf(" - ");
                }
                printf("\n");
                break;
            case 'x':
            case 'X':
                printf("* Insira o valor da carta ['A', '2' - '9', 'D', 'J', 'Q', 'R']: ");
                valor = getchar();
                while(getchar() != '\n');
                printf("* Insira o naipe da carta ['P', 'E', 'C', 'O']: ");
                naipe = getchar();
                while(getchar() != '\n');
                removeCarta(&monte, valor, naipe);
                break;
            case 's':
            case 'S':
                break;
            default:
                printf("* Opção inválida.\n");
        }
        printf("\n");
    } while(cmd != 's' && cmd != 'S');
}
