#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dll.h"
#include "stack.h"

/* Author: Logan Jordan
 * The stack class, containing the functions necessary for
 * defining the behavior of the stack.
 */

//Defines a stack
struct stack{
	DLL *list;
	void (*display)(void *, FILE *);
};

//Creates a new stack
STACK *newSTACK(void (*d)(void *, FILE *),void (*f)(void *)){
	STACK *items = malloc(sizeof(STACK));
	items->list = newDLL(d, f);
	assert(items != NULL);
	
	items->display = d;

	return items;
}

//Inserts a new item at front of the DLL
void push(STACK *items, void *value){
	insertDLL(items->list, 0, value);
	return;
}

//Removes and returns the last item added to the stack
void *pop(STACK *items){
	assert(sizeDLL(items->list) > 0);
	return removeDLL(items->list, 0);
}

//Returns the last value added to the stack but does not delete it
void *peekSTACK(STACK *items){
	assert(sizeDLL(items->list) > 0);
	return getDLL(items->list, 0);
}

//Returns the size of the stack
int sizeSTACK(STACK *items){
	return sizeDLL(items->list);
}

//Displays the contents of items to the place specified by fp
//in the format |1,2,3|, where 1 is the last item added
void displaySTACK(STACK *items, FILE *fp){
	fprintf(fp, "|");
	int i;
	for(i = 0; i < sizeDLL(items->list); i++){
		items->display(getDLL(items->list, i), fp);
		if(i != sizeDLL(items->list) - 1){
			fprintf(fp, ",");
		}
	}
	fprintf(fp, "|");
	return;
}

//Calls the displayDLLdebug function to show the contents of the DLL
void displaySTACKdebug(STACK *items, FILE *fp){
	displayDLLdebug(items->list, fp);
	return;
}

//Frees the DLL and then frees the stack
void freeSTACK(STACK *items){
	freeDLL(items->list);
	free(items);
	return;
}