/*******************************************************************
 * @file   offsetstack.c                                           *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'offsetstack.h' header file. *
 *******************************************************************/


#include "offsetstack.h"



/**
 * @brief
 */
void push_offset(scopespace_t scopespace){
	if (scopeoffsetstack == NULL) {
		scopeoffsetstack = (offset_stack*)malloc(sizeof(offset_stack));
		scopeoffsetstack->next = NULL;
		scopeoffsetstack->scopespace = scopespace;
	}
	else {
		offset_stack* newentry = (offset_stack*)malloc(sizeof(offset_stack));
		newentry->next = scopeoffsetstack;
		newentry->scopespace = scopespace;
		scopeoffsetstack = newentry;
	}
}

/**
 * @brief
 */
scopespace_t pop_offset(void) {
	if (scopeoffsetstack == NULL) {
		printf("Trying to pop from empty offset_stack!!\n");
		exit(1);
	}
	else {
		offset_stack* new_top = scopeoffsetstack;
		new_top = new_top->next;
		scopespace_t ret = scopeoffsetstack->scopespace;
		free(scopeoffsetstack);
		scopeoffsetstack = new_top;
		return ret;
	}
}