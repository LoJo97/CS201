#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sll.h"

/* Author: Logan Jordan
 * The singly linked list (SLL) class, containing the functions necessary for
 * defining the behavior of the list.
 */

//Defines a node of a singly linked list
typedef struct node{
	void *value;
	struct node *next;
} NODE;

//Defines a singly linked list
struct sll{
	NODE *head;
	NODE *tail;
	int size;

	void (*display)(void *, FILE *);
	void (*free)(void *);
};

//Creates a new node contaning value v
NODE *newSLLNode(void *v){
	NODE *newSLLNode = malloc(sizeof(NODE));
	newSLLNode->value = v;
	newSLLNode->next = NULL;
	return newSLLNode;
}

//Creates a new empty SLL
SLL *newSLL(void (*d)(void *, FILE*), void (*f)(void*)){
	SLL *items = malloc(sizeof(SLL));
		items->head = malloc(sizeof(NODE));
	assert(items != NULL);
	assert(items->head != NULL);

	items->head->value = NULL;
	items->head->next = NULL;

	items->tail = items->head;

	items->size = 0;
	items->display = d;
	items->free = f;
	return items;
}

//Inserts a value into items at the index given
void insertSLL(SLL *items, int index, void *value){
	assert(index <= items->size);
	assert(index >= 0);
	if(index == items->size){ //Insert at end
		NODE *node = newSLLNode(value);
		items->tail->next = node;
		items->tail = items->tail->next;
		(items->size)++;
		return;
	}else{ //Insert into beginning or middle
		NODE *x = items->head;
		NODE *y = items->head->next;
		int i;
		for(i = 0; i < index; i++){
			x = x->next;
			y = y->next;
		}
		NODE *node = newSLLNode(value);
		x->next = node;
		node->next = y;
		(items->size)++;
		return;
	}
	return;
}

//Removes a value from items at the given index
void *removeSLL(SLL *items, int index){
	assert(items->size > 0);
	assert(index >= 0);
	assert(index < items->size);
	//Removes if only 1 item in list
	if(items->size == 1){
		void *valStore = items->tail->value; //Stores the value to be returned

		free(items->tail);

		items->tail = items->head;
		
		(items->size)--;

		return valStore;
	}else if(index == items->size - 1){ //Removes item from end of list
		void *valStore = items->tail->value; //Stores the value to be returned

		NODE* oldTail = items->tail; //Keeps track of the old tail for deletion later

		NODE *curNode = items->head->next;
		int i;
		for(i = 0; i < items->size - 2; i++){ //Finds the second to last node
			curNode = curNode->next;
		}

		curNode->next = NULL; //Makes the second to last node the new tail
		items->tail = curNode;

		free(oldTail); //Frees the old tail

		(items->size)--;

		return valStore;
	}else{ //Removes an item normally
		NODE *deleteNode = items->head->next;
		NODE *prev = items->head;
		int i;
		for(i = 0; i < index; i++){ //Finds both the node to be deleted and the node before it
			deleteNode = deleteNode->next;
			prev = prev->next;
		}

		void *valStore = deleteNode->value; //Stores the value to be returned later

		prev->next = deleteNode->next; //Relinks the list so that the node is removed

		free(deleteNode); //Frees the old node

		(items->size)--;

		return valStore;
	}
}

//Appends donor to recipient, leaving donor empty
//FIXME: LONG VALGRIND DUE TO REPEATED FREE/MALLOC
void unionSLL(SLL *recipient, SLL *donor){
	if((recipient->size == 0 && donor->size == 0) 
		|| donor->size == 0 ) return; //Deals with two empty lists or an empty donor
	else if(recipient->size == 0){ //Deals with an empty recipient list
		//Appends donor to recipient
		recipient->head->next = donor->head->next;
		recipient->tail = donor->tail;
		//Resets donor's tail
		donor->tail = donor->head;
		//Resets size
		recipient->size = (recipient->size + donor->size);
		donor->size = 0;
		return;
	}else{
		//Appends donor to recipient
		recipient->tail->next = donor->head->next;
		recipient->tail = donor->tail;
		//Resets donor's tail
		donor->tail = donor->head;
		//Resets size
		recipient->size = recipient->size + donor->size;
		donor->size = 0;
		return;
	}
	return;
}

//Returns the value in items at the index
//FIXME: BACK RETRIEVALS (test sll-0-14)(DO IN BOTH SLL AND DLL)
void *getSLL(SLL *items, int index){
	assert(index >= 0);
	assert(index < items->size);

	if(index == items->size - 1){
		return items->tail->value;
	}

	NODE *x = items->head->next;
	int i;
	for(i = 0; i < index; i++){
		x = x->next;
	}
	return x->value;
}

//Sets the value in items at the index to value and returns the replaced value
//FIXME: BACK SETS (NOT ADDS)(DO IN BOTH SLL AND DLL)
void *setSLL(SLL *items, int index, void *value){
	assert(index >= 0);
	assert(index <= items->size);
	
	if(index == items->size){ //Set after last item in list
		insertSLL(items, index, value);
		return NULL;
	}else if(index == items->size - 1){
		void *y = items->tail->value;
		items->tail->value = value;
		return y;
	}else{
		NODE *x = items->head->next;
		int i;
		for(i = 0; i < index; i++){
			x = x->next;
		}
		void *y = x->value;
		x->value = value;
		return y;
	}
}

//Returns the size of items
int sizeSLL(SLL *items){
	return items->size;
}

//Displays the contents of items to the place specified by fp 
//in the format {1,2,3,...}
void displaySLL(SLL *items, FILE *fp){
	fprintf(fp, "{");
	NODE *x = items->head->next;
	int i;
	for(i = 0; i < items->size; i++){
		items->display(x->value, fp);
		if(i != items->size - 1){
			fprintf(fp, ",");
		}
		x = x->next;
	}
	fprintf(fp, "}");
	return;
}

//Displays the contents of items to the place specified by fp 
//in the format head->{1,2,3},tail->{3}
void displaySLLdebug(SLL *items, FILE *fp){
	fprintf(fp, "head->");
	displaySLL(items, fp);
	fprintf(fp, ",tail->{");
	if(items->tail->value != NULL){
		items->display(items->tail->value, fp);
	}
	fprintf(fp, "}");
	return;
}

//Walks through the list and frees each value and each node, ending by
//freeing the list itself
void freeSLL(SLL *items){
	if(items->size == 0){
		items->free(items->head->value);
		free(items->head);
	}else if(items->size == 1){ //If tail and head are only nodes
		items->free(items->head->value);
		free(items->head);
		items->free(items->tail->value);
		free(items->tail);
	}else{
		int i;
		for(i = 0; i <= items->size; i++){
			NODE *curNode = items->head;
			items->head = items->head->next;
			if(items->free != NULL){
				items->free(curNode->value);
			}
			free(curNode);
		}
		free(items->head);
	}
	free(items);
	return;
}