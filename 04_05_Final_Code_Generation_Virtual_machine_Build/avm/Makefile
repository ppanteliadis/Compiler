#————————-——————————————————————————————————#
#		cangonj, panteliad, paraskevas		#
#		5th Phase @ Compilers				#
#		Makefile							#
#		The file that builds the entire		#
#		project.							#
#————————-——————————————————————————————————#

## Implicit rules

all: avmout

avmout: avm.o alpha.o avm_stack.o avm_table.o dispatcher.o libfuncs.o exec_arithmetics.o exec_assign.o exec_funcs.o exec_jumps.o exec_logical.o exec_tables.o tables_hash.o
	gcc -o avm.out avm.o alpha.o avm_stack.o avm_table.o dispatcher.o libfuncs.o exec_arithmetics.o exec_assign.o exec_funcs.o exec_jumps.o exec_logical.o exec_tables.o tables_hash.o -lm

avm.o: avm.c oneheader.h libfuncs.h
	gcc -c -g avm.c

alpha.o: alpha.c oneheader.h
	gcc -c -g alpha.c

avm_stack.o: avm_stack.c oneheader.h
	gcc -c -g avm_stack.c

avm_table.o: avm_table.c oneheader.h
	gcc -c -g -std=c99 avm_table.c

dispatcher.o: dispatcher.c oneheader.h
	gcc -c -g dispatcher.c

libfuncs.o: libfuncs.c libfuncs.h
	gcc -c -g libfuncs.c

exec_arithmetics.o: exec_arithmetics.c oneheader.h
	gcc -c -g exec_arithmetics.c -lm

exec_assign.o: exec_assign.c oneheader.h
	gcc -c -g exec_assign.c

exec_funcs.o: exec_funcs.c oneheader.h
	gcc -c -g exec_funcs.c

exec_jumps.o: exec_jumps.c oneheader.h
	gcc -c -g exec_jumps.c

exec_logical.o: exec_logical.c oneheader.h
	gcc -c -g exec_logical.c

exec_tables.o: exec_tables.c oneheader.h
	gcc -c -g exec_tables.c

tables_hash.o: tables_hash.c oneheader.h
	gcc -c -g tables_hash.c -lm

clean:
	rm -rfv avm.out *.o