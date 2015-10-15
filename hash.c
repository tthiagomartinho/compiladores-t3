#include "hash.h"

#define TAM 97

struct variavel {
    char *nome;
    void *valor;
    int tipo; //real = 0, inteiro = 1, caractere = 2, literal = 3, logico = 4, matriz = 5
    int escopo; //global=1, local=1
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

Lista* criaNovoNoLista(void* info, Lista* prox) {
    Lista* l = (Lista*)malloc(sizeof(Lista));
    l->info = info;
    l->prox = prox;
    return l;
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
        if (strcmp(v->nome, nome) == 0 && v->escopo == escopo) {
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
        printf("VALOR %s JA EXISTE \n", nome);
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
    }
    return tabelaHash;
}

Variavel* novaVariavel(char* nome, void* valor, int tipo, int escopo) {
    Variavel* v = (Variavel*) malloc(sizeof (Variavel));
    v -> nome = (char*) malloc((strlen(nome) + 1) * sizeof (char));
    strcpy(v->nome, nome);
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

void imprimeHash(Lista** tabelaHash){
    int i;
    for(i = 0; i < TAM; i++){
        if(tabelaHash[i] != NULL){
            Lista* l;
            Lista* aux;
            for(l = tabelaHash[i]; l != NULL; l = l->prox){
                Variavel* v = (Variavel*)l->info;
                printf("Nome: %s\n", v-> nome);
                printf("valor: %p\n", v-> valor);
                printf("tipo: %d\n", v-> tipo);
                printf("escopo: %d\n", v-> escopo);
                printf("usada: %d\n", v-> usada);
                printf("\n");
            }
        }
    }
}

Lista** insereListaVariaveisHash(Lista** tabelaHash, Lista* variaveis, int tipo, int escopo){
    Lista* l;
    for(l = variaveis; l != NULL; l = l ->prox){
        Variavel* v = (Variavel*) l->info;
        tabelaHash = insereVariavel(tabelaHash, v->nome, NULL, tipo, escopo);
    }
    return tabelaHash;

}

void libera(Lista** tabelaHash) {
    int i;
    for(i = 0; i < TAM; i++){
        if(tabelaHash[i] != NULL){
            liberaLista(tabelaHash[i]);
        }
    }
}

Lista* liberaLista(Lista* lista){
    Lista* l;
    Lista* aux;
    for(l = lista; l != NULL; l = l->prox){
        aux = l -> prox;
        Variavel* v = (Variavel*) l->info;
        liberaVariavel(v);
        free(l);
        //l = aux;
    }
   l = NULL;
}

Variavel* liberaVariavel(Variavel* v){
    free(v->nome);
    if(v->valor != NULL){
        free(v->valor);
    }
    free(v);

}
Funcao* liberaFuncao(Funcao* f){

}