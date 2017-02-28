/*******************************************************************
 * @file   breakcontlist.h                                         *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all the type declarations and function         *
 *         prototypes needed in order to handle the breakcontlist  *
 *         A.D.T., as this is defined in our Grammar               *
 *******************************************************************/

#ifndef BREAKCONTLIST_H
#define BREAKCONTLIST_H

#include <stdio.h>
#include <stdlib.h>
#include "quad.h"



typedef struct list {
	unsigned int quadNo;
	struct list* next;
}list;


typedef struct breakcontlist {
	list* breaklist;
	list* contlist;
}breakcontlist;



extern breakcontlist* breakconthead;

void init_breakcontlist(void);

void break_insert(unsigned int, unsigned int);

void cont_insert(unsigned int, unsigned int);

void while_backpatch(void);

void for_backpatch(unsigned int);



#endif