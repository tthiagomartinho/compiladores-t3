all: compila executa

compila:
	flex -i analisador_lexico.l
	bison analisador_sintatico.y
	gcc -otrab3 analisador_sintatico.tab.c hash.c util.c -lfl

executa:
	./trab3 < ex/in1.gpt

clean:
	-rm -f *.o *.output lex.yy.c analisador_sintatico.tab.c
