#include "hash.h"

#define TAM 97

struct variavel {
    char *nome;
    void *valor;
    int tipo; //real = 0, inteiro = 1, caractere = 2, literal = 3, logico = 4, matriz = 5
    int escopo; //global=1, local=1
    int usada;
    int dimensaoMatriz;
    int* dimensoes;
};

struct funcao {
    char *nome;
    int retorno; //real = 0, inteiro = 1, caractere = 2, literal = 3, logico = 4, void = -1
    int aridade;
    int escopo;
    int tipo_parametros[10]; //cada funcao pode ter no maximo 10 parametros
};

struct lista {
    void *info;
    int tipo; //real = 0, inteiro = 1, caractere = 2, literal = 3, logico = 4, void = -1, variavel = 6, funcao = 7
    struct lista *prox;
};

/**************************FUNCOES BASICAS DE LISTAS***************************/
Lista* inicializarLista(void) {
    return NULL;
}

/*Cria um novo no no inicio da lista*/
Lista* criarNovoNoLista(int tipo, void* info, Lista* prox) {
    Lista* l = (Lista*) malloc(sizeof (Lista));
    l->tipo = tipo;
    if (tipo == TIPO_LITERAL) {
        l->info = (char*) malloc((strlen(info) + 1) * sizeof (char));
        strcpy(l->info, info);
    } else {
        l->info = info;
    }
    l->prox = prox;
    return l;
}

/*Cria um novo no no fim da lista*/
Lista* criarNovoNoListaFim(int tipo, void* info, Lista* primeiro) {
    Lista* l = (Lista*) malloc(sizeof (Lista));
    l->tipo = tipo;
    if (tipo == TIPO_LITERAL) {
        l->info = (char*) malloc((strlen(info) + 1) * sizeof (char));
        strcpy(l->info, info);
    } else {
        l->info = info;
    }

    l -> prox = NULL;

    if (primeiro == NULL) {
        primeiro = l;
    } else {
        Lista* aux = primeiro;
        while (aux -> prox != NULL) {
            aux = aux -> prox;
        }
        aux -> prox = l;
    }
    return primeiro;
}

Lista* liberarMemoriaLista(Lista* lista) {
    Lista* l;
    Lista* aux;
    for (l = lista; l != NULL;) {
        aux = l -> prox;
        switch (l->tipo) {
            case TIPO_VARIAVEL:
            {
                Variavel* v = (Variavel*) l->info;
                liberarMemoriaVariavel(v);
                break;
            }
            case TIPO_FUNCAO:
            {
                Funcao* f = (Funcao*) l->info;
                //liberarMemoriaFuncao(f);
                break;
            }
            case TIPO_LITERAL:
                free(l->prox);
                break;
            default:
                break;
        }
        free(l);
        l = aux;
    }
    return NULL;
}

/**************************FUNCOES TABELA HASH*********************************/
Lista** inicializarTabelaHash() {
    int i;
    Lista **v;
    v = (Lista**) malloc(TAM * sizeof (Lista*));
    for (i = 0; i < TAM; i++) {
        v[i] = inicializarLista();
    }
    return v;
}

Lista* buscarVariavelTabelaHash(Lista** tabelaHash, char nome[], int escopo) {
    int chave = 0, i;
    for (i = 0; i < nome[i] != '\0'; i++) {
        chave = chave + (int) nome[i];
    }

    Lista* p;
    chave = chave % TAM;
    for (p = tabelaHash[chave]; p != NULL; p = p->prox) {
        Variavel *v = (Variavel*) p->info;
        if (strcmp(v->nome, nome) == 0 && v->escopo == escopo) {
            return p;
        }
    }
    return NULL;
}

Lista** liberarMemoriaTabelaHash(Lista** tabelaHash) {
    int i;
    if (tabelaHash != NULL) {
        for (i = 0; i < TAM; i++) {
            if (tabelaHash[i] != NULL) {
                tabelaHash[i] = liberarMemoriaLista(tabelaHash[i]);
            }
        }
        free(tabelaHash);
    }
    return NULL;
}

/**************************MANIPULACAO DE VARIAVEIS****************************/

/*Cria um novo objeto do tipo variavel
 * nome: o nome que sera atribuido a variavel
 * valor: o valor que sera a tribuido a variavel
 * dimensoesMatriz: uma lista que contém as dimensões da matriz. NULL se a variavel nao for do tipo matriz
 * tipo: inteiro que define qual o tipo da variavel. DE 0 ATÉ 4
 * escopo: escopo da variavel
 */
Variavel* criarNovaVariavel(char* nome, Lista* dimensoesMatriz, int tipo, int escopo) {
    Variavel* v = (Variavel*) malloc(sizeof (Variavel));
    v -> nome = (char*) malloc((strlen(nome) + 1) * sizeof (char));
    strcpy(v->nome, nome);

    switch (tipo) {
        case TIPO_REAL:
            v->valor = (float*) malloc(sizeof (float));
            break;
        case TIPO_INTEIRO:
            v->valor = (int*) malloc(sizeof (int));
            break;
        case TIPO_LITERAL:
            v->valor = (char*) malloc(sizeof (char));
            break;
        case TIPO_LOGICO:
            v->valor = (int*) malloc(sizeof (int));
            break;
        default:
            break;
    }

    if (dimensoesMatriz != NULL) {
        Lista* l;
        int i = 0, j;
        for (l = dimensoesMatriz; l != NULL; l = l -> prox) {
            i++;
        }
        v -> dimensoes = (int*) malloc(i * sizeof (int));
        v -> dimensaoMatriz = i;
        l = dimensoesMatriz;
        int totalEspacoNecessario = 1;
        for (j = 0; j < i; j++) {
            char* q = (char*) l->info;
            l = l->prox;
            int dimensao = atoi(q);
            v->dimensoes[j] = dimensao;
            totalEspacoNecessario = totalEspacoNecessario * dimensao;
        }
        v->valor = (int*) malloc(totalEspacoNecessario * sizeof (int));
    }
    v -> tipo = tipo;
    v -> escopo = escopo;
    return v;
}

Variavel* liberarMemoriaVariavel(Variavel* v) {
    free(v->nome);
    if (v->valor != NULL) {
        free(v->valor);
    }
    if (v->dimensoes != NULL) {
        free(v->dimensoes);
    }
    free(v);
}

/**************************MANIPULACAO TABELA HASH DE VARIAVEIS*****************/

/*Cria um novo objeto na tabela Hash
 * tabelaHash: a tabela onde sera inserida a variavel
 * nome: o nome que sera atribuido a variavel
 * valor: o valor que sera a tribuido a variavel
 * dimensoesMatriz: uma lista que contém as dimensões da matriz. NULL se a variavel nao for do tipo matriz
 * tipo: inteiro que define qual o tipo da variavel. DE 0 ATÉ 4
 * escopo: escopo da variavel
 */
Lista** inserirVariavelTabelaHash(Lista** tabelaHash, char* nome, Lista* dimensoesMatriz, int tipo, int escopo) {
    if (buscarVariavelTabelaHash(tabelaHash, nome, escopo) != NULL) {
        return NULL;
    } else {
        Variavel* variavel = criarNovaVariavel(nome, dimensoesMatriz, tipo, escopo);
        int chave = 0, i;
        for (i = 0; i < nome[i] != '\0'; i++) {
            chave = chave + (int) nome[i];
        }
        chave = chave % TAM;
        Lista* lista = (Lista*) malloc(sizeof (Lista));
        lista -> info = (void*) variavel;
        lista -> tipo = TIPO_VARIAVEL;
        lista -> prox = tabelaHash[chave];
        tabelaHash[chave] = lista;
    }
    return tabelaHash;
}

void imprimirTabelaHash(Lista** tabelaHash) {
    int i;
    for (i = 0; i < TAM; i++) {
        if (tabelaHash[i] != NULL) {
            Lista* l;
            for (l = tabelaHash[i]; l != NULL; l = l->prox) {
                Variavel* v = (Variavel*) l->info;
                printf("Nome: %s\n", v-> nome);
                printf("valor: %p\n", v-> valor);
                printf("tipo: %d\n", v-> tipo);
                printf("escopo: %d\n", v-> escopo);
                printf("usada: %d\n", v-> usada);
                int i;
                printf("Dimensoes: ");
                for (i = 0; i < v->dimensaoMatriz; i++) {
                    printf("%d ", v->dimensoes[i]);
                }
                printf("\n\n");
            }
        }
    }
}

Lista** inserirListaVariaveisTabelaHash(Lista** tabelaHash, Lista* dimensoesMatriz, Lista* variaveis, int tipo, int escopo) {
    Lista* l;
    for (l = variaveis; l != NULL; l = l ->prox) {
        Variavel* v = (Variavel*) l->info;
        tabelaHash = inserirVariavelTabelaHash(tabelaHash, v->nome, dimensoesMatriz, tipo, escopo);
        if (tabelaHash == NULL) {
            break;
        }
    }
    return tabelaHash;

}

/**************************MANIPULACAO DE FUNCOES******************************/
Funcao* criarFuncao(char* nome){
    Funcao* f = (Funcao*)malloc(sizeof(Funcao));
    f->nome = (char*)malloc((strlen(nome) + 1) * sizeof(char));
    strcpy(f->nome, nome);
    return f;
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

Funcao* liberaFuncao(Funcao* f) {

}