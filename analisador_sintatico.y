%{
#include <stdio.h>
#include "hash.h"
    
    extern int line_num;
    extern char* yytext;
    int tipo;
    Lista** hashVariavel;
    Lista* variaveis;
    int escopo = 0;
    Lista* dimensoesMatriz;

    void finalizarProgramaComErro(char* erro){
        printf("Erro semantico na linha %d. %s.\n", line_num, erro);
        variaveis = liberarMemoriaLista(variaveis);
        dimensoesMatriz = liberarMemoriaLista(dimensoesMatriz);
        hashVariavel = liberarMemoriaTabelaHash(hashVariavel);
        exit(0);
}
%}

/* TOKENS DEFINICOES BASICAS */
%token token_identificador
%token token_inteiro
%token token_inteiroNegativo
%token token_real
%token token_realNegativo
%token token_literal
%token token_caractere

/* TOKENS PALAVRAS RESERVADAS */
%token token_algoritmo
%token token_variaveis
%token token_fimVariaveis
%token token_inicio
%token token_fim
%token token_se
%token token_entao
%token token_senao
%token token_fimSe
%token token_de
%token token_leia
%token token_imprima
%token token_verdadeiro
%token token_falso
%token token_enquanto
%token token_faca
%token token_fimEnquanto
%token token_para
%token token_ate
%token token_fimPara
%token token_funcao
%token token_fimFuncao
%token token_retorne
%token token_passo
%token token_avalie
%token token_fimAvalie
%token token_caso
%token token_pare
%token token_facaEnquanto
%token token_fimFacaEnquanto

/* TOKENS TIPOS DE DADOS */
%token token_tipoReal
%token token_tipoInteiro
%token token_tipoCaractere
%token token_tipoLiteral
%token token_tipoLogico
%token token_tipoMatriz

/* TOKENS OPERADORES */
%token token_operadorAtribuicao
%token token_operadorPotencia
%token token_operadorSomaSoma
%token token_operadorSubtraiSubtrai
%token token_operadorPorcento
%token token_operadorMais
%token token_operadorMenos
%token token_operadorVezes
%token token_operadorDividir
%token token_operadorMaior
%token token_operadorMaiorIgual
%token token_operadorMenor
%token token_operadorMenorIgual
%token token_operadorIgualIgual
%token token_operadorDiferente
%token token_operadorE
%token token_operadorOu
%token token_operadorNao

/*TOKENS SIMBOLOS */
%token token_simboloPontoVirgula
%token token_simboloDoisPontos
%token token_simboloVirgula
%token token_simboloAbreParentese
%token token_simboloFechaParentese
%token token_simboloAbreColchete
%token token_simboloFechaColchete
%token token_simboloEspecial

%start ALGORITMO

%%

ALGORITMO
    : token_algoritmo token_identificador token_simboloPontoVirgula PROGRAMA
    ;

PROGRAMA
    : VARIAVEIS DECLARACAO_FUNCAO PROGRAMA_PRINCIPAL
    | VARIAVEIS PROGRAMA_PRINCIPAL
    | DECLARACAO_FUNCAO PROGRAMA_PRINCIPAL
    | PROGRAMA_PRINCIPAL
    ;

VARIAVEIS
    : token_variaveis DECLARACAO_VARIAVEL token_fimVariaveis
    ;

DECLARACAO_VARIAVEL
    : DECLARACAO_VARIAVEL DECLARACAO_VARIAVEL_GERAL
    | DECLARACAO_VARIAVEL_GERAL
    ;

DECLARACAO_VARIAVEL_GERAL
    : LISTA_VARIAVEIS token_simboloDoisPontos TIPO_VARIAVEIS { 
        hashVariavel = inserirListaVariaveisTabelaHash(hashVariavel, dimensoesMatriz, variaveis, tipo, escopo);
        if(hashVariavel == NULL){
            finalizarProgramaComErro("Variavel redeclara");
        }
        variaveis = liberarMemoriaLista(variaveis);
	    dimensoesMatriz = liberarMemoriaLista(dimensoesMatriz);
    }  token_simboloPontoVirgula
    ;

LISTA_VARIAVEIS
    : LISTA_VARIAVEIS token_simboloVirgula token_identificador{
        Variavel* var = criarNovaVariavel(yytext, NULL, tipo, escopo) ;
        Lista* l = criarNovoNoLista(TIPO_LISTA_VARIAVEL, var, variaveis);
        variaveis = l;
    } 
    | token_identificador {
        Variavel* var = criarNovaVariavel(yytext, NULL, tipo, escopo) ;
        Lista* l = criarNovoNoLista(TIPO_LISTA_VARIAVEL, var, variaveis);
        variaveis = l;
    } 
    ;

DECLARACAO_FUNCAO
    : DECLARACAO_FUNCAO token_funcao token_identificador token_simboloAbreParentese PARAMETRO_DECLARACAO_FUNCAO token_simboloFechaParentese token_simboloDoisPontos TIPO_VARIAVEL_PRIMITIVO ROTINA_FUNCAO token_fimFuncao
    | DECLARACAO_FUNCAO token_funcao token_identificador token_simboloAbreParentese PARAMETRO_DECLARACAO_FUNCAO token_simboloFechaParentese token_simboloDoisPontos ROTINA_FUNCAO token_fimFuncao
    | DECLARACAO_FUNCAO token_funcao token_identificador token_simboloAbreParentese token_simboloFechaParentese token_simboloDoisPontos TIPO_VARIAVEL_PRIMITIVO ROTINA_FUNCAO token_fimFuncao
    | DECLARACAO_FUNCAO token_funcao token_identificador token_simboloAbreParentese token_simboloFechaParentese token_simboloDoisPontos ROTINA_FUNCAO token_fimFuncao
    | token_funcao token_identificador token_simboloAbreParentese PARAMETRO_DECLARACAO_FUNCAO token_simboloFechaParentese token_simboloDoisPontos TIPO_VARIAVEL_PRIMITIVO ROTINA_FUNCAO token_fimFuncao
    | token_funcao token_identificador token_simboloAbreParentese PARAMETRO_DECLARACAO_FUNCAO token_simboloFechaParentese token_simboloDoisPontos ROTINA_FUNCAO token_fimFuncao
    | token_funcao token_identificador token_simboloAbreParentese token_simboloFechaParentese token_simboloDoisPontos TIPO_VARIAVEL_PRIMITIVO ROTINA_FUNCAO token_fimFuncao
    | token_funcao token_identificador token_simboloAbreParentese token_simboloFechaParentese token_simboloDoisPontos ROTINA_FUNCAO token_fimFuncao
    ;

PARAMETRO_DECLARACAO_FUNCAO
    : PARAMETRO_DECLARACAO_FUNCAO token_simboloVirgula token_identificador token_simboloDoisPontos TIPO_VARIAVEL_PRIMITIVO 
    | token_identificador token_simboloDoisPontos TIPO_VARIAVEL_PRIMITIVO
    ;

ROTINA_FUNCAO
    : DECLARACAO_VARIAVEL token_inicio LISTA_COMANDOS token_fim
    | token_inicio LISTA_COMANDOS token_fim
    ;

COMANDO_RETORNO
    : token_retorne EXPRESSAO token_simboloPontoVirgula
    | token_retorne token_simboloPontoVirgula
    ;

TIPO_VARIAVEIS
    : MATRIZ
    | TIPO_VARIAVEL_PRIMITIVO
    ;

TIPO_VARIAVEL_PRIMITIVO
    : token_tipoReal{
        tipo = 0;
    }
    | token_tipoInteiro{
        tipo = 1;
    }
    | token_tipoCaractere{
        tipo = 2;
    }
    | token_tipoLiteral{
        tipo = 3;
    }
    | token_tipoLogico {
        tipo = 4;
    }
    ;

MATRIZ
    : token_tipoMatriz POSICAO_MATRIZ token_de TIPO_VARIAVEL_PRIMITIVO
    ;

POSICAO_MATRIZ
    : POSICAO_MATRIZ token_simboloAbreColchete token_inteiro {
            dimensoesMatriz = criarNovoNoListaFim(TIPO_LISTA_CHAR, yytext, dimensoesMatriz);
    } token_simboloFechaColchete 
    | token_simboloAbreColchete token_inteiro {
            dimensoesMatriz = criarNovoNoListaFim(TIPO_LISTA_CHAR, yytext, dimensoesMatriz);
    }token_simboloFechaColchete
    ;

PROGRAMA_PRINCIPAL
    : token_inicio LISTA_COMANDOS token_fim
    | token_inicio token_fim
    ;

LISTA_COMANDOS
    : LISTA_COMANDOS COMANDO_ATRIBUICAO token_simboloPontoVirgula 
    | COMANDO_ATRIBUICAO token_simboloPontoVirgula
    | LISTA_COMANDOS COMANDO_ENQUANTO 
    | COMANDO_ENQUANTO
    | LISTA_COMANDOS COMANDO_PARA 
    | COMANDO_PARA
    | LISTA_COMANDOS COMANDO_LEIA
    | COMANDO_LEIA
    | LISTA_COMANDOS COMANDO_IMPRIMA
    | COMANDO_IMPRIMA
    | LISTA_COMANDOS COMANDO_CHAMADA_FUNCAO token_simboloPontoVirgula 
    | COMANDO_CHAMADA_FUNCAO token_simboloPontoVirgula
    | LISTA_COMANDOS COMANDO_SE 
    | COMANDO_SE
    | LISTA_COMANDOS COMANDO_FACA_ENQUANTO
    | COMANDO_FACA_ENQUANTO
    | LISTA_COMANDOS COMANDO_AVALIE
    | COMANDO_AVALIE
    | LISTA_COMANDOS COMANDO_RETORNO
    | COMANDO_RETORNO
    | LISTA_COMANDOS COMANDO_MAIS_MAIS_MENOS_MENOS
    | COMANDO_MAIS_MAIS_MENOS_MENOS
    ;

COMANDO_ATRIBUICAO
    : token_identificador token_operadorAtribuicao VALOR_A_SER_ATRIBUIDO
    | ACESSO_MATRIZ token_operadorAtribuicao VALOR_A_SER_ATRIBUIDO
    ;

VALOR_A_SER_ATRIBUIDO
    : VALOR_A_SER_ATRIBUIDO EXPRESSAO 
    | EXPRESSAO
    | VALOR_A_SER_ATRIBUIDO COMANDO_CHAMADA_FUNCAO
    | COMANDO_CHAMADA_FUNCAO
    ;

COMANDO_ENQUANTO
    : token_enquanto EXPRESSAO token_faca LISTA_COMANDOS token_fimEnquanto
    ;

COMANDO_PARA 
    : token_para token_identificador token_de EXPRESSAO token_ate EXPRESSAO token_faca LISTA_COMANDOS token_fimPara
    | token_para token_identificador token_de EXPRESSAO token_ate EXPRESSAO token_passo NUMERO token_faca LISTA_COMANDOS token_fimPara
    ;

COMANDO_SE
    : token_se EXPRESSAO token_entao LISTA_COMANDOS token_senao LISTA_COMANDOS token_fimSe
    | token_se EXPRESSAO token_entao LISTA_COMANDOS token_fimSe
    ;

COMANDO_FACA_ENQUANTO
    : token_faca token_simboloDoisPontos LISTA_COMANDOS token_enquanto token_simboloAbreParentese EXPRESSAO token_simboloFechaParentese token_fimEnquanto
    ;

COMANDO_AVALIE
    : token_avalie token_simboloAbreParentese token_identificador token_simboloFechaParentese token_simboloDoisPontos AVALIE_CASO token_fimAvalie
    ;

AVALIE_CASO
    : AVALIE_CASO token_caso token_identificador token_simboloDoisPontos LISTA_COMANDOS token_pare token_simboloPontoVirgula
    | AVALIE_CASO token_caso NUMERO token_simboloDoisPontos LISTA_COMANDOS token_pare token_simboloPontoVirgula
    | token_caso token_identificador token_simboloDoisPontos LISTA_COMANDOS token_pare token_simboloPontoVirgula
    | token_caso NUMERO token_simboloDoisPontos LISTA_COMANDOS token_pare token_simboloPontoVirgula
    ;

COMANDO_LEIA
    : token_identificador token_operadorAtribuicao token_leia token_simboloAbreParentese token_simboloFechaParentese token_simboloPontoVirgula
    ;

COMANDO_IMPRIMA
    : token_imprima token_simboloAbreParentese PARAMETROS_FUNCAO token_simboloFechaParentese token_simboloPontoVirgula
    ;

PARAMETROS_FUNCAO
    : PARAMETROS_FUNCAO token_simboloVirgula EXPRESSAO  
    | EXPRESSAO
    | PARAMETROS_FUNCAO token_simboloVirgula COMANDO_CHAMADA_FUNCAO  
    | COMANDO_CHAMADA_FUNCAO
    ;

COMANDO_CHAMADA_FUNCAO
    : token_identificador token_simboloAbreParentese token_simboloFechaParentese
    | token_identificador token_simboloAbreParentese PARAMETROS_FUNCAO token_simboloFechaParentese
    ;

COMANDO_MAIS_MAIS_MENOS_MENOS
    : token_identificador token_operadorSomaSoma token_simboloPontoVirgula
    | token_operadorSomaSoma token_identificador token_simboloPontoVirgula
    | token_identificador token_operadorSubtraiSubtrai token_simboloPontoVirgula
    | token_operadorSubtraiSubtrai token_identificador token_simboloPontoVirgula
    ;
    
EXPRESSAO
    : EXPRESSAO_SIMPLES
    | EXPRESSAO_SIMPLES OPERADORES_RELACIONAIS EXPRESSAO_SIMPLES
    ;

EXPRESSAO_SIMPLES
    : TERMO
    | EXPRESSAO_SIMPLES OPERADORES_BAIXA_PRECEDENCIA TERMO
    ;

TERMO
    : FATOR
    | TERMO OPERADORES_ALTA_PRECEDENCIA FATOR
    ;

FATOR
    : token_identificador
    | token_operadorNao FATOR
    | NUMERO
    | ACESSO_MATRIZ
    | token_verdadeiro
    | token_falso
    | token_literal
    | token_caractere
    | token_simboloAbreParentese EXPRESSAO token_simboloFechaParentese
    ;

NUMERO
    : token_inteiro
    | token_inteiroNegativo
    | token_real
    | token_realNegativo
    ;

ACESSO_MATRIZ
    : token_identificador POSICAO_MATRIZ
    ;

OPERADORES_RELACIONAIS
    : token_operadorIgualIgual
    | token_operadorMenor
    | token_operadorMenorIgual
    | token_operadorMaiorIgual
    | token_operadorMaior
    | token_operadorDiferente
    ;

OPERADORES_BAIXA_PRECEDENCIA
    : token_operadorMais
    | token_operadorMenos
    | token_operadorOu
    ;

OPERADORES_ALTA_PRECEDENCIA
    : token_operadorVezes
    | token_operadorDividir
    | token_operadorPorcento
    | token_operadorPotencia
    | token_operadorE
    ;

%%

#include "lex.yy.c"

main(){
    hashVariavel = inicializarTabelaHash();
    variaveis = NULL;
    yyparse();
    imprimirTabelaHash(hashVariavel);
    hashVariavel = liberarMemoriaTabelaHash(hashVariavel);
}

/* rotina chamada por yyparse quando encontra erro */
yyerror (void){
    printf("Erro na Linha: %d\n", line_num);
}