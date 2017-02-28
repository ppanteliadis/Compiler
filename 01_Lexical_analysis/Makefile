#      cangonj, panteliad, paraskevas      #
#      1st Phase @ Compilers               #
#      MAKEFILE                            #
#    ——————————————————————————————————    #

CC=gcc -g
FL=flex
FFLAGS= --outfile

OFLAGS=-o

REM=rm -f

XF=.out

CF=.c

HF=.h



all: 	al.c
	$(CC) $(OFLAGS) al.out al.c


al.c:	al.l
	$(FL) $(FFLAGS)=al.c al.l
	
clean:	
	rm -rf al.out al.c
