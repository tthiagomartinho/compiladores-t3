#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista Lista;
typedef struct funcao Funcao;
typedef struct variavel Variavel;

#define TIPO_LISTA_VARIAVEL 0
#define TIPO_LISTA_FUNCAO 1
#define TIPO_LISTA_CHAR 2

/**************************FUNCOES BASICAS DE LISTAS***************************/
Lista* inicializarLista(void);

/*Cria um novo no no inicio da lista*/
Lista* criarNovoNoLista(int tipo, void* info, Lista* prox);

/*Cria um novo no no fim da lista*/
Lista* criarNovoNoListaFim(int tipo, void* info, Lista* primeiro);

Lista* liberarMemoriaLista(Lista* lista);

/**************************FUNCOES TABELA HASH*********************************/
Lista** inicializarTabelaHash();

Lista* buscarVariavelTabelaHash(Lista** tabelaHash, char nome[], int escopo);

Lista** liberarMemoriaTabelaHash(Lista** tabelaHash);

/**************************MANIPULACAO DE VARIAVEIS****************************/

Variavel* criarNovaVariavel(char* nome, Lista* dimensoesMatriz, int tipo, int escopo);

Variavel* liberarMemoriaVariavel(Variavel* v);

/**************************MANIPULACAO TABELA HASH DE VARIAVEIS*****************/

Lista** inserirVariavelTabelaHash(Lista** tabelaHash, char* nome, Lista* dimensoesMatriz, int tipo, int escopo);

void imprimirTabelaHash(Lista** tabelaHash);

Lista** inserirListaVariaveisTabelaHash(Lista** tabelaHash, Lista* dimensoesMatriz, Lista* variaveis, int tipo, int escopo);

/**************************MANIPULACAO DE FUNCOES******************************/
Lista* buscaFuncao(Lista** l, char nome[], int escopo);

Funcao* liberaFuncao(Funcao* f);