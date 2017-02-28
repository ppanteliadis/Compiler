#      cangonj, panteliad, paraskevas      #
#      2nd Phase @ Compilers               #
#      MAKEFILE                            #
#    ——————————————————————————————————    #

syntax.out: parser.o al.o symtable.o
	gcc -g -Wall -o syntax.out parser.o al.o symtable.o

parser.o: parser.y
	@echo 'yaccing parser.y --> parser.c'
	bison -v -y -d --output=parser.c parser.y
	gcc -c -g parser.c

al.o: al.l
	@echo 'flexing al.l --> al.c'
	flex --outfile=al.c al.l
	gcc -c -g al.c

symtable.o: symtable.c symtable.h
	@echo 'compiling symtable.c --> symtable.o'
	gcc -c -g -w symtable.c

## Rules

all: syntax.out
	@echo 'create .out file --> syntax.out'
	gcc -c -o -g al.c parser.c

clean:
	@echo 'kill useless files'
	rm -rfv syntax.out parser.output parser.c scanner.c *.o sa.out parser.h *.*~ al.c

