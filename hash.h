#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista Lista;
typedef struct pilha Pilha;
typedef struct funcao Funcao;
typedef struct variavel Variavel;

Lista* inicializa (void);
Lista** inicializa_hash (void);
int vazia (Lista* l);
void imprimeHash(Lista** tabelaHash);
void libera(Lista** l);
Lista* liberaLista(Lista* l);
Lista* criaNovoNoLista(void* info, Lista* prox);

Variavel* liberaVariavel(Variavel* v);
Funcao* liberaFuncao(Funcao* f);

Lista* buscaVariavel(Lista** l, char nome[], int escopo);
Lista** insereVariavel(Lista** tabelaHash, char* nome, void* valor, int tipo, int escopo);
Variavel* novaVariavel(char* nome, void* valor, int tipo, int escopo);
Lista** insereListaVariaveisHash(Lista** tabelaHash, Lista* variaveis, int tipo, int escopo);