/*******************************************************************
 * @file   arrays.c                                                *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'arrays.h' header file       *
 *******************************************************************/

#include "funcstack.h"


/****** FUNCTIONS STACK ******/


/**
 * @brief
 */
func* push_func(func* head, SymbolTableEntry** ret) {
	func* tmp = (func*)malloc(sizeof(func));
	if (!tmp) {
		fprintf(stderr, "push_func: Allocation error!\n");
		exit(-1);
	}
	tmp->sym = *ret;
	tmp->next = head;
	head = tmp;
	return head;
}

/**
 * @brief
 */
func* pop_func(func* head, SymbolTableEntry** ret) {
	if (!head) {
		printf("pop_func: Trying to pop from empty funcstack!!\n");
		exit(-1);
	}
	func* tmp = head;
	*ret = head->sym;
	head = head->next;
	free(tmp);
	return head;
}


/**
 * @brief
 */
func* top_func(func* head, SymbolTableEntry** ret) {
	if (!head) {
		printf("top_func: Trying to top from empty funcstack!!\n");
		exit(-1);
	}
	else {
		*ret = head->sym;
		return head;
	}
}




