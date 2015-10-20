#include "hash.h"

#define TAM 97

struct variavel {
    char *nome;
    void *valor;
    char* escopo;
    int tipo; //real = 0, inteiro = 1, caractere = 2, literal = 3, logico = 4, matriz = 5
    int usada;
    int dimensaoMatriz;
    int* dimensoes;
};

struct funcao {
    char* nome;
    int* tipoParametros; //cada funcao pode ter no maximo 10 parametros
    int retorno; //real = 0, inteiro = 1, caractere = 2, literal = 3, logico = 4, void = -1
    int aridade;
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
                free(l->info);
                break;
            case TIPO_MATRIZ:
                free(l->info);
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

Lista* buscarVariavelTabelaHash(Lista** tabelaHash, char nome[], char* escopo) {
    int chave = 0, i;
    for (i = 0; i < nome[i] != '\0'; i++) {
        chave = chave + (int) nome[i];
    }

    Lista* p;
    chave = chave % TAM;
    for (p = tabelaHash[chave]; p != NULL; p = p->prox) {
        Variavel *v = (Variavel*) p->info;
        if (strcmp(v->nome, nome) == 0 && strcmp(v->escopo, escopo) == 0) {
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
Variavel* criarNovaVariavel(char* nome, Lista* dimensoesMatriz, int tipo, char* escopo) {
    Variavel* v = (Variavel*) malloc(sizeof (Variavel));
    v -> nome = (char*) malloc((strlen(nome) + 1) * sizeof (char));
    strcpy(v->nome, nome);

    v -> escopo = (char*) malloc((strlen(escopo) + 1) * sizeof (char));
    strcpy(v->escopo, escopo);

    v -> tipo = tipo;

    switch (tipo) {
        case TIPO_REAL:
            v->valor = (float*) malloc(sizeof (float));
            break;
        case TIPO_INTEIRO:
            v->valor = (int*) malloc(sizeof (int));
            break;
        case TIPO_LITERAL:
            v->valor = (char**) malloc(sizeof (char*));
            break;
        case TIPO_LOGICO:
            v->valor = (int*) malloc(sizeof (int));
            break;
        case TIPO_CARACTERE:
            v->valor = (char*) malloc(sizeof (char));
            break;
        case TIPO_MATRIZ:{
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
        default:
            break;
    }
    return v;
}

Variavel* liberarMemoriaVariavel(Variavel* v) {
    free(v->nome);
    free(v->valor);
    free(v->escopo);
    if (v->tipo == TIPO_MATRIZ) {
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
Lista** inserirVariavelTabelaHash(Lista** tabelaHash, char* nome, Lista* dimensoesMatriz, int tipo, char* escopo) {
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
                printf("escopo: %s\n", v-> escopo);
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

Lista** inserirListaVariaveisTabelaHash(Lista** tabelaHash, Lista* dimensoesMatriz, Lista* variaveis, int tipo, char* escopo) {
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
    Funcao* funcao = (Funcao*) malloc(sizeof (Funcao));
    funcao->nome = (char*) malloc((strlen(nome) + 1)* sizeof (char));
    strcpy(funcao->nome, nome);
    funcao->tipoParametros = NULL;
    return funcao;
}

Funcao* buscarFuncaoTabelaHash(Lista** tabelaHash, char* nome, int aridade, int retorno, int* tipoParametros) {
    int chave = 0, i;
    for (i = 0; i < nome[i] != '\0'; i++) {
        chave = chave + (int) nome[i];
    }

    Lista* p;
    chave = chave % TAM;
    int achou = 0;
    for (p = tabelaHash[chave]; p != NULL; p = p->prox) {
        Funcao *f = (Funcao*) p->info;
        if (strcmp(f->nome, nome) == 0 && f->aridade == aridade && f->retorno == retorno){
            for(i = 0; i < aridade; i++){
                if(f->tipoParametros[i] == tipoParametros[i]){
                    achou = 1;
                }else{
                    achou = 0;
                }
            }
            if(achou == 1){
                return f;
            }
        }
    }
    return NULL;
}

Lista** inserirFuncaoTabelaHash(Funcao* funcao, Lista* variaveis, int tipo, Lista** tabelaHash){
    if(variaveis != NULL){
        funcao = setConfiguracaoParametrosFuncao(funcao, variaveis);
    }
    funcao->retorno = tipo;

    if (buscarFuncaoTabelaHash(tabelaHash, funcao->nome, funcao->aridade, funcao->retorno, funcao->tipoParametros) != NULL) {
        return NULL;
    } else {
        int chave = 0, i;
        for (i = 0; i < funcao->nome[i] != '\0'; i++) {
            chave = chave + (int) funcao->nome[i];
        }
        chave = chave % TAM;
        Lista* lista = (Lista*) malloc(sizeof (Lista));
        lista -> info = (void*) funcao;
        lista -> tipo = TIPO_FUNCAO;
        lista -> prox = tabelaHash[chave];
        tabelaHash[chave] = lista;
    }
    return tabelaHash;

}

Funcao* setConfiguracaoParametrosFuncao(Funcao* funcao, Lista* variaveis){
    Lista* l;
    int i = 0;
    for(l = variaveis; l != NULL; l = l -> prox){
        i++;
    }
    funcao->tipoParametros = (int*)malloc(i*sizeof(int));
    i = 0;
    for(l = variaveis; l != NULL; l = l -> prox){
        Variavel* v = (Variavel*) l->info;
        funcao->tipoParametros[i] = v->tipo;
        i++;
    }
    funcao->aridade = i;
    return funcao;
}

Funcao* liberaFuncao(Funcao* f) {
    if(f != NULL){
        free(f->nome);
        if(f->tipoParametros != NULL){
            free(f->tipoParametros);
        }
        free(f);
    }
    return NULL;
}

char* getNomeFuncao(Funcao* funcao){
    if(funcao != NULL){
        return funcao->nome;
    }else{
        return NULL;
    }
}

void imprimirTabelaHashFuncao(Lista** tabelaHash) {
    int i;
    for (i = 0; i < TAM; i++) {
        if (tabelaHash[i] != NULL) {
            Lista* l;
            for (l = tabelaHash[i]; l != NULL; l = l->prox) {
                Funcao* f = (Funcao*) l->info;
                printf("Nome: %s\n", f-> nome);
                printf("Retorno: %d\n", f-> retorno);
                printf("Aridade: %d\n", f-> aridade);
                int i;
                printf("Tipo de Parametro: ");
                for (i = 0; i < f->aridade; i++) {
                    printf("%d ", f->tipoParametros[i]);
                }
                printf("\n\n");
            }
        }
    }
}