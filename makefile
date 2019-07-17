all:
	make clean
	flex lexicalStructure.lex
	bison -v grammar.y
	gcc -o comp grammar.tab.c symbolTable.c ASCGenerator.c ICRGenerator.c
	./comp -st -asc < input.txt

clean:
	rm -f *.o
	rm -f lexer
	rm -f lex.yy.c
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.output
	rm -f compiler
	rm -f comp
