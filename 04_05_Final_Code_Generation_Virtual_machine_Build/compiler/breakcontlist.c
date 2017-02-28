/*******************************************************************
 * @file   breakcontlist.c                                         *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Implementation file of the 'breakcontlist.h'            *
 *         header file                                             *
 *******************************************************************/

#include "breakcontlist.h"


void init_breakcontlist(){
	breakconthead = (breakcontlist*)malloc(sizeof(breakcontlist));
}

void break_insert(unsigned int label, unsigned int line) {
	list* newentry = (list*)malloc(sizeof(list));
	if (newentry == NULL) {
		printf("break_insert: Memory allocation error!\n");
		exit(-1);
	}
	else {
		newentry->quadNo = label;

		newentry->next = breakconthead->breaklist;
		breakconthead->breaklist = newentry;
		emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, -10, line);
	}
}

void cont_insert(unsigned int label, unsigned int line) {
	list* newentry = (list*)malloc(sizeof(list));
	if (newentry == NULL) {
		printf("cont_insert: Memory allocation error!\n");
		exit(-1);
	}
	else {
		newentry->quadNo = label;

		newentry->next = breakconthead->contlist;
		breakconthead->contlist = newentry;
		emit(jump, (expr*)NULL, (expr*)NULL, (expr*)NULL, -10, line);
	}
}



void while_backpatch() {
	while (breakconthead->breaklist != NULL) {
		patchlabel(breakconthead->breaklist->quadNo, nextquadlabel());
		breakconthead->breaklist = breakconthead->breaklist->next;
	}

	while (breakconthead->contlist != NULL) {
		patchlabel(breakconthead->contlist->quadNo, nextquadlabel());
		breakconthead->contlist = breakconthead->contlist->next;
	}
}

void for_backpatch(unsigned int label) {
	while (breakconthead->breaklist != NULL) {
		patchlabel(breakconthead->breaklist->quadNo, nextquadlabel());
		breakconthead->breaklist = breakconthead->breaklist->next;
	}

	while (breakconthead->contlist != NULL) {
		patchlabel(breakconthead->contlist->quadNo, label + 1);
		breakconthead->contlist = breakconthead->contlist->next;
	}
}
