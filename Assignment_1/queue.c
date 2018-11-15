#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sll.h"
#include "queue.h"

/* Author: Logan Jordan
 * The queue class, containing the functions necessary for
 * defining the behavior of the queue.
 */

//Defines a queue
struct queue{
	SLL *list;
	void (*display)(void *, FILE *);
};

//Creates a new queue
QUEUE *newQUEUE(void (*d)(void *,FILE *), void (*f)(void *)){
	QUEUE *items = malloc(sizeof(QUEUE));
	items->list = newSLL(d, f);
	assert(items != NULL);

	items->display = d;

	return items;
}

//Inserts a new item at the back of the SLL
void enqueue(QUEUE *items, void *value){
	insertSLL(items->list, sizeSLL(items->list), value);
	return;
}

//Removes an item from the front of the DLL and returns that item
void *dequeue(QUEUE *items){
	assert(sizeSLL(items->list) > 0);
	return removeSLL(items->list, 0);
}

//Returns the item at the front of the DLL without deleting the item
void *peekQUEUE(QUEUE *items){
	assert(sizeSLL(items->list) > 0);
	return getSLL(items->list, 0);
}

//Returns the size of the SLL
int sizeQUEUE(QUEUE *items){
	return sizeSLL(items->list);
}

//Displays the contents of items to the place specified by fp
//in the format <1,2,3>, where 3 is the last item added
void displayQUEUE(QUEUE *items, FILE *fp){
	fprintf(fp, "<");
	int i;
	for(i = 0; i < sizeSLL(items->list); i++){
		items->display(getSLL(items->list, i), fp);
		if(i != sizeSLL(items->list) - 1){
			fprintf(fp, ",");
		}
	}
	fprintf(fp, ">");
	return;
}

//Calls the displaySLLdebug function to show the contents of the SLL
void displayQUEUEdebug(QUEUE *items, FILE *fp){
	displaySLLdebug(items->list, fp);
	return;
}

//Frees the SLL and then frees the queue
void freeQUEUE(QUEUE *items){
	freeSLL(items->list);
	free(items);
	return;
}