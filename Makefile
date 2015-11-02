all: compila

compila:
	flex -i analisador_lexico.l
	bison -v analisador_sintatico.y
	gcc -otrab3 analisador_sintatico.tab.c hash.c -lfl
	pdflatex relatorio-trab3.tex
