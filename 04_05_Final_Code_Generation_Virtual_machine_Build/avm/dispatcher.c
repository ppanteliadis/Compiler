#include "oneheader.h"

unsigned char	executionFinished = 0;
unsigned		pc = 0;
unsigned		currLine = 0;
unsigned		totalActuals = 0;

extern unsigned char executionFinished;

execute_func_t executeFuncs[] = {
	execute_assign,
	execute_add,
	execute_sub,
	execute_mul,
	execute_div,
	execute_mod,
	0, /*execute_uminus*/
	0, /*execute_and*/
	0, /*execute_or*/
	0, /*execute_not*/
	execute_jeq,
	execute_jne,
	execute_jle,
	execute_jge,
	execute_jlt,
	execute_jgt,
	execute_jump,
	execute_call,
	execute_pusharg,
	execute_tablecreate,
	execute_tablegetelem,
	execute_tablesetelem,
	execute_funcenter,
	execute_funcexit,
	execute_ret,
	execute_getretval,
	execute_nop
};

char* debug[]={
	"execute_assign",
	"execute_add",
	"execute_sub",
	"execute_mul",
	"execute_div",
	"execute_mod",
	"0 uminus", /*execute_uminus,*/
	"0 and", /*execute_and,*/
	"0 or", /*execute_or*/
	"0 not", /*execute_not*/
	"execute_jeq",
	"execute_jne",
	"execute_jle",
	"execute_jge",
	"execute_jlt",
	"execute_jgt",
	"execute_jump",
	"execute_call",
	"execute_pusharg",
	"execute_tablecreate",
	"execute_tablegetelem",
	"execute_tablesetelem",
	"execute_funcenter",
	"execute_funcexit",
	"execute_ret",
	"execute_getretval",
	"execute_nop"
};


void execute_cycle() {
	printf("execute_cycle()\n");
	if (executionFinished) {
		return;
	}
	else {
		if (pc == AVM_ENDING_PC) {
			executionFinished = 1;
			return;
		}
		else {
			assert(pc < AVM_ENDING_PC);
			printf("pc = %u\n", pc);
			instruction* instr = instruction_table + pc;
			assert(instr->opcode >= 0 &&
				   instr->opcode <= AVM_MAX_INSTRUCTIONS);
			if (instr->srcLine) {
				currLine = instr->srcLine;
			}
			unsigned oldPC = pc;
			printf("executing %s\n", debug[instr->opcode]);
			(*executeFuncs[instr->opcode])(instr);
			if (pc == oldPC) {
				++pc;
			}
		}
	}
}


void execute_nop(instruction* instr) {
	printf("execute_nop();\n");
	return;
}

void avm_error(char* format,...)
{
    static char error[] = "Alpha error: ";
    fputs(error,stdout);
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;
    char *traverse;
    unsigned int i;
    char *s;
    int num=0;
    int base;
    va_list arg;
    va_start(arg, format);

    for(traverse = format; *traverse != '\0'; traverse++)
    {
        while( *traverse != '%' )
        {
            putchar(*traverse);
            traverse++;
        }
        traverse++;
   	int z;
    	for(z=0;z<50;z++)
        	buffer[z]='\0';
        if(*traverse == 'c'){
        	i = va_arg(arg,int);
                putchar(i);
	}
        else if(*traverse == 'd'){
		i = va_arg(arg,int);
                        if(i<0)
                        {
                            i = -i;
                            putchar('-');
                        }
		base=10;
                ptr = &buffer[49];
                *ptr = '\0';
                do
                {
                        *--ptr = Representation[i%base];
                        i /= base;
                }while(i != 0);
                fputs(ptr,stdout);

		}
            else if(*traverse == 'o'){
		i = va_arg(arg,unsigned int);
		base=8;
                ptr = &buffer[49];
                *ptr = '\0';
                do
                {
                        *--ptr = Representation[i%base];
                        i /= base;
                }while(i != 0);
                fputs(ptr,stdout);
		}

            else if(*traverse == 's'){
			s = va_arg(arg,char *);
                        fputs(s,stdout);
	    }
            else if (*traverse =='x'){
		i= va_arg(arg,unsigned int);
 		base=16;
    		ptr = &buffer[49];
    		*ptr = '\0';
		do
    		{
        		*--ptr = Representation[i%base];
        		i /= base;
    		}while(i != 0);
		fputs(ptr,stdout);
	   }
        }

    va_end(arg);
}
void avm_warning(char* format,...)
{
    static char warning[] = "Alpha warning: ";
    fputs(warning,stdout);
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;
    char *traverse;
    unsigned int i;
    char *s;
    int num=0;
    int base;
    va_list arg;
    va_start(arg, format);

    for(traverse = format; *traverse != '\0'; traverse++)
    {
        while( *traverse != '%' )
        {
            putchar(*traverse);
            traverse++;
        }
        traverse++;
   	int z;
    	for(z=0;z<50;z++)
        	buffer[z]='\0';
        if(*traverse == 'c'){
        	i = va_arg(arg,int);
                putchar(i);
	}
        else if(*traverse == 'd'){
		i = va_arg(arg,int);
                        if(i<0)
                        {
                            i = -i;
                            putchar('-');
                        }
		base=10;
                ptr = &buffer[49];
                *ptr = '\0';
                do
                {
                        *--ptr = Representation[i%base];
                        i /= base;
                }while(i != 0);
                fputs(ptr,stdout);

		}
            else if(*traverse == 'o'){
		i = va_arg(arg,unsigned int);
		base=8;
                ptr = &buffer[49];
                *ptr = '\0';
                do
                {
                        *--ptr = Representation[i%base];
                        i /= base;
                }while(i != 0);
                fputs(ptr,stdout);
		}

            else if(*traverse == 's'){
			s = va_arg(arg,char *);
                        fputs(s,stdout);
	    }
            else if (*traverse =='x'){
		i= va_arg(arg,unsigned int);
 		base=16;
    		ptr = &buffer[49];
    		*ptr = '\0';
		do
    		{
        		*--ptr = Representation[i%base];
        		i /= base;
    		}while(i != 0);
		fputs(ptr,stdout);
	   }
        }

    va_end(arg);
}
