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
void libera(List* l);