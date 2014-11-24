#ifndef BARALHO_H_
#define BARALHO_H_

//Bordas
#define BRDTOP 0
#define BRDRGT 1
#define BRDBTM 2
#define BRDLFT 3
// Quantidades padrões
#define NAIPES 4
#define CARTAS 13

/*
 * Estruturas de dados
 */

typedef struct {
    unsigned char R, G, B;
} Pixel;

typedef struct {
    char naipe, valor;
    Pixel *img;
} Carta;

typedef struct {
    int h, w, maxval;
} Cabecalho;

typedef Carta** Baralho;

typedef char** Monte;

// ==============================
//       Funções públicas     
// ==============================

// init:
// Abre arquivo (Não é realizada uma verificação de integridade nesse ponto!
// Inicializa variáveis internas, como o cabeçalho e os naipes.
// Aloca a memória para o baralho, bem como para as imagens de cada carta <= return.
// Retorna: Uma estrutura baralho (sem as imagens das cartas).
Baralho init(char *imgFile, int cw, int ch, char primCarta, char *ordNaipes);

// geraBaralho:
// Verifica se o arquivo é um arquivo PPM e/ou se está corrompido.
// Preenche o baralho (criado anteriormente por init) com as imagens das cartas. <= return
// Retorna: Uma estrutura baralho (com as imagens das cartas).
Baralho geraBaralho(Baralho cartas);

// getCarta:
// Retorna: Carta especificada em baralho.
Carta getCarta(char naipe, char valor, Baralho baralho);

// geraCarta:
// Gera um arquivo .PPM no diretório corrente, contendo a imagem da carta especificada.
void geraCarta(char naipe, char valor, Baralho baralho);

// geraMonteVazio:
// Aloca e retorna um monte vazio (apenas com um NULL).
// Retorna: Monte gerado.
Monte geraMonteVazio();

// geraMonte:
// Gera um monte com n * 52 cartas, embaralhando-as em seguida (2000 trocas aleatórias)
// Retorna: Monte gerado.
Monte geraMonte(Baralho cartas, int n);

// embaralha:
// Simula o processo de embaralhamento, trocando 2 cartas de posição aleatoriamente várias vezes.
// Retorna: O próprio monte.
Monte embaralha(Monte monte);

// tiraCarta:
// Obtem a primeira Carta de monte, em seguida desloca tudo uma posição, removendo a primeira carta do monte.
// É importante se certificar que o monte não está vazio (NULL) antes de chamar essa função.
// Retorna: A carta removida do baralho.
Carta tiraCarta(Monte* pmonte, Baralho baralho);

// removeCarta:
// Remove a primeira ocorrência da carta especificada no monte.
int removeCarta(Monte* pmonte, char valor, char naipe);

// insereCarta:
// Acrescenta 'carta' no topo do monte.
void insereCarta(Monte* pmonte, Carta carta);

// isNextCard:
// Verifica se y é a próxima carta em relação a x (levando em conta o naipe).
// Retorna: Inteiro 1 caso seja verdadeiro, 0 caso contrário.
int isNextCard(Carta x, Carta y);

// isPreviousCard:
// Verifica se y é a carta anterior em relação a x (levando em conta o naipe).
// Retorna: Inteiro 1 caso seja verdadeiro, 0 caso contrário.
int isPreviousCard(Carta x, Carta y);

// getMonteSize:
// Retorna: Número de cartas em monte.
int getMonteSize(Monte monte);

// setBordas:
// Define o espaço que a imagem possui de bordas em todos os 4 cantos (medida em pixels).
void setBordas(int top, int right, int bottom, int left);

#endif
