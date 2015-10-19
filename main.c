/* 
 * File:   main.c
 * Author: Thiago
 *
 * Created on 12 de Outubro de 2015, 21:24
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

/*
 * 
 */
int main(int argc, char** argv) {
    Lista** hashVariavel = inicializarTabelaHash();
    char nome[] = "teste";
    //    Lista** hashFuncao = inicializa_hash();
    
    hashVariavel = inserirVariavelTabelaHash(hashVariavel, nome, 1, 0, 0);
    if (buscarVariavelTabelaHash(hashVariavel, nome, 0) != NULL) {
        printf("A busca funciona\n");
    } else {
        printf("A busca nao funciona\n");
    }
    liberarMemoriaTabelaHash(hashVariavel);
    return (EXIT_SUCCESS);
}