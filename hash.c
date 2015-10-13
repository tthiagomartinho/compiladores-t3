#include "hash.h"

#define TAM 97

struct variavel {
    char *nome;
    void *valor;
    int tipo; //inteiro=0, caracter=1, string=2, real=3, booleano =4
    int escopo; //local=0, global=1
    int usada;
};

struct funcao {
    char *nome;
    int retorno; // inteiro=0, caracter=1, string=2, real=3, booleano =4, void = -1
    int aridade;
    int escopo;
    int tipo_parametros[10]; //cada funcao pode ter no maximo 10 parametros
};

struct lista {
    void *info;
    struct lista *prox;
};

Lista* inicializa(void) {
    return NULL;
}

Lista** inicializa_hash() {
    int i;
    Lista **v;
    v = (Lista**) malloc(TAM * sizeof (Lista*));
    for (i = 0; i < TAM; i++) {
        v[i] = inicializa();
    }
    return v;
}

int vazia(Lista* l) {
    if (l == NULL)
        return 1;
    else
        return 0;

}

Lista* buscaVariavel(Lista** l, char nome[], int escopo) {
    int chave = 0, i;
    for (i = 0; i < nome[i] != '\0'; i++) {
        chave = chave + (int) nome[i];
    }

    Lista* p;
    chave = chave % TAM;
    for (p = l[chave]; p != NULL; p = p->prox) {
        Variavel *v = (Variavel*) p->info;
        if ( v->escopo == escopo) {
            return p;
        }
    }
    return NULL;
}

Lista* buscaFuncao(Lista** l, char nome[], int escopo) {
    int chave = 0, i;
    for (i = 0; i < nome[i] != '\0'; i++) {
        chave = chave + (int) nome[i];
    }

    Lista* p;
    chave = chave % TAM;
    for (p = l[chave]; p != NULL; p = p->prox) {
        Funcao *f = (Funcao*) p->info;
        if (strcmp(f->nome, nome) == 0 && f->escopo == escopo) {
            return p;
        }
    }
    return NULL;
}

Lista** insereVariavel(Lista** tabelaHash, char* nome, void* valor, int tipo, int escopo) {
    if (buscaVariavel(tabelaHash, nome, escopo) != NULL) {
        //JA EXISTE
        //O QUE FAZER?
    } else {
        Variavel* variavel = novaVariavel(nome, valor, tipo, escopo);
        int chave = 0, i;
        for (i = 0; i < nome[i] != '\0'; i++) {
            chave = chave + (int) nome[i];
        }
        chave = chave % TAM;
        Lista* lista = (Lista*) malloc(sizeof (Lista));
        lista -> info = (void*) variavel;

        lista -> prox = tabelaHash[chave];
        tabelaHash[chave] = lista;

        return tabelaHash;
    }
}

Variavel* novaVariavel(char* nome, void* valor, int tipo, int escopo) {
    Variavel* v = (Variavel*) malloc(sizeof (Variavel));
    v -> nome = (char*) malloc((strlen(nome) + 1) * sizeof (char));
    strcmp(v->nome, nome);
    if (tipo == 2) {
        v -> valor = (char*) malloc(strlen((char*) valor + 1) * sizeof (char));
        strcpy(v -> valor, (char*) valor);
    } else {
        v -> valor = valor;
    }
    v -> tipo = tipo;
    v -> escopo = escopo;
    return v;
}

void libera(Lista** tabelaHash) {
    int i;
    for(i = 0; i < TAM; i++){
        if(tabelaHash[i] != NULL){
            Lista* l;
            Lista* aux;
            for(l = tabelaHash[i]; l -> prox != NULL; l = l->prox){
                aux = l -> prox;
                free(l->info);
                free(l);
                l = aux;
            }
        }
    }
}