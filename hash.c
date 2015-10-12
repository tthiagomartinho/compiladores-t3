#include "hash.h"

#define TAM 97

struct variavel{
	char *nome;
	void *valor;
	int tipo; //inteiro=0, caracter=1, string=2, real=3, booleano =4
	int escopo; //local=0, global=1
	int  usada;
};

struct funcao{
	char *nome;
	int  retorno; // inteiro=0, caracter=1, string=2, real=3, booleano =4, void = -1
	int  aridade; 
	int  tipo_parametros[10];//cada funcao pode ter no maximo 10 parametros
};

struct lista{
	void *info;
	struct lista *prox;
};

Lista* inicializa (void){
	return NULL;
}

Lista** inicializa_hash (){
	int i;
	Lista **v;
	v = (Lista**) malloc(TAM*sizeof(Lista*));
	for(i=0; i<TAM;i++){
		v[i] = inicializa();	
	}
	return v;
}

int vazia (Lista* l){
	if(l==NULL)
		return 1;
	else
		return 0;

}

void libera (Lista* l){
	Lista* p=l;
	while (p!=NULL){
		Lista* t = p->prox;
		free(p->info);
		free (p);
		p=t;	
	}
}