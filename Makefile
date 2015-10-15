all: compila

compila:
	flex -i analisador_lexico.l
	bison analisador_sintatico.y
	gcc -otrab3 analisador_sintatico.tab.c hash.c -lfl
	./trab3 < ex/in1.gpt
