/*******************************************************************
 * @file   loopcounterstack.h                                      *
 * @author Steve Cangonj <cangonj@csd.uoc.gr>, 2871                *
 * @author Ioannis-Pavlos Panteliadis <panteliad@csd.uoc.gr>, 2794 *
 * @author Kiriakos Paraskevas <paraskevas@csd.uoc.gr>, 2832       *
 *                                                                 *
 * @brief  Contains all the type declarations and function         *
 *         prototypes needed in order to handle the                *
 *         loopcounterstack A.D.T., as this is defined in          *
 *         our Grammar.                                            *
 *******************************************************************/

#ifndef LOOPCOUNTERSTACK_H
#define LOOPCOUNTERSTACK_H

#include <stdlib.h>
#include <stdio.h>


typedef struct loopcounter_stack {
	int loopcounter;
	struct loopcounter_stack *next;
}loopcounter_stack;


extern loopcounter_stack* loopcounterstack;

/**
 * @brief
 */
void push_funcblock(int);

/**
 * @brief
 */
int pop_funcblock(void);


#endif