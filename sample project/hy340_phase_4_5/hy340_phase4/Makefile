P1 = gcc
P2 = flex
P3 = bison

all : bison1 analyzer  al

bison1 : parser.y
	bison  --yacc --defines --output=parser.c parser.y

analyzer : lexicalanalyzer.l
	$(P2)  $^

al : scanner.c
	gcc symbolTable.c manage_grammar.c  parser.c scanner.c icode.c stack.c tcode.c constTables.c
