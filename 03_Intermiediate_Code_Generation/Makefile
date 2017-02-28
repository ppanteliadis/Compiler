#————————-——————————————————————————————————#
#		cangonj, panteliad, paraskevas		#
#		3rd Phase @ Compilers				#
#		Makefile							#
#		The file that builds the entire		#
#		project.							#
#————————-——————————————————————————————————#


## Implicit rules

all: syntaxout

syntaxout: parser.o al.o symtable.o quad.o offsetstack.o breakcontlist.o loopcounterstack.o
	gcc -o syntax.out symtable.o al.o parser.o  quad.o breakcontlist.o offsetstack.o loopcounterstack.o -lm

parser.o: parser.y symtable.h quad.h offsetstack.h breakcontlist.h loopcounterstack.h
	bison -v -y -d --output=parser.c parser.y
	gcc -c -g -lm parser.c

al.o: al.l parser.h symtable.h
	flex --outfile=al.c al.l
	gcc -c -g -lm al.c

symtable.o: symtable.c symtable.h
	gcc -c -g -lm symtable.c

quad.o: quad.c quad.h symtable.h
	gcc -c -g -lm quad.c

offsetstack.o: offsetstack.c offsetstack.h
	gcc -c -g offsetstack.c

breakcontlist.o: breakcontlist.c breakcontlist.h
	gcc -c -g breakcontlist.c

loopcounterstack.o: loopcounterstack.c loopcounterstack.h
	gcc -c -g loopcounterstack.c



clean:
	rm -rfv syntax.out parser.output parser.c al.c *.o parser.h *.*~ parser.y.save
