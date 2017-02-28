/*******************************************************************
 * @file   loopcounterstack.c                                      *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'loopcounterstack.h'         *
 *         header file.                                            *
 *******************************************************************/


#include "loopcounterstack.h"


/**
 * @brief
 */
void push_funcblock(int loopcounter) {
	if (loopcounterstack == NULL) {
		loopcounterstack = (loopcounter_stack*)malloc(sizeof(loopcounter_stack));
		loopcounterstack->next = NULL;
		loopcounterstack->loopcounter = loopcounter;
	}
	else {
		loopcounter_stack* newentry = (loopcounter_stack*)malloc(sizeof(loopcounter_stack));
		newentry->next = loopcounterstack;
		newentry->loopcounter = loopcounter;
		loopcounterstack = newentry;
	}
}

/**
 * @brief
 */
int pop_funcblock(void) {
	if (loopcounterstack == NULL) {
		printf("Trying to pop from empty funcblock_stack!!\n");
		exit(1);
	}
	else {
		loopcounter_stack* new_top = loopcounterstack;
		new_top = new_top->next;
		int ret = loopcounterstack->loopcounter;
		free(loopcounterstack);
		loopcounterstack = new_top;
		return ret;
	}
}