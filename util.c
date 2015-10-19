/* 
 * File:   Util.c
 * Author: Thiago
 *
 * Created on 18 de Outubro de 2015, 23:31
 */

#include "util.h"

int mapearTipoString(char* nomeTipo) {
    int tipo;
    if (strcmp(nomeTipo, "real") == 0) {
        tipo = 0;
    } else if (strcmp(nomeTipo, "inteiro") == 0) {
        tipo = 1;
    } else if (strcmp(nomeTipo, "caractere") == 0) {
        tipo = 2;
    } else if (strcmp(nomeTipo, "literal") == 0) {
        tipo = 3;
    } else if (strcmp(nomeTipo, "logico") == 0) {
        tipo = 4;
    } else if (strcmp(nomeTipo, "matriz") == 0) {
        tipo = 5;
    }
    return tipo;

}
