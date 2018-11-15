#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dll.h"

/* Author: Logan Jordan
 * The doubly linked list (DLL) class, containing the functions necessary for
 * defining the behavior of the list.
 */

//Defines a node of a singly linked list
typedef struct node{
	void *value;
	struct node *next;
	struct node *prev;
} NODE;

//Defines a doubly linked list
struct dll{
	NODE *head;
	NODE *tail;
	int size;

	void (*display)(void *, FILE *);
	void (*free)(void *);
};

//Creates a new node contaning value v
NODE *newDLLNode(void *v){
	NODE *newDLLNode = malloc(sizeof(NODE));
	newDLLNode->value = v;
	newDLLNode->next = NULL;
	newDLLNode->prev = NULL;
	return newDLLNode;
}

//Creates a new empty DLL
DLL *newDLL(void (*d)(void *, FILE*), void (*f)(void*)){
	DLL *items = malloc(sizeof(DLL));
		items->head = malloc(sizeof(NODE));
	assert(items != NULL);

	items->head->value = NULL;
	items->head->next = NULL;
	items->head->prev = NULL;

	items->tail = items->head;

	items->size = 0;
	items->display = d;
	items->free = f;

	return items;
}

//Inserts a value into items at the index given
void insertDLL(DLL *items, int index, void *value){
	assert(index <= items->size);
	assert(index >= 0);
	if(index == items->size){ //Insert at back
		NODE *node = newDLLNode(value);
		node->prev = items->tail;
		items->tail->next = node;
		items->tail = node;
		(items->size)++;
		return;
	}else if(index > (items->size) / 2){ //Insert in back half
		int i;
		NODE *x = items->tail;

		for(i = items->size - 1; i > index; i--){
			x = x->prev;
		}

		NODE *node = newDLLNode(value);

		x->prev->next = node;
		node->prev = x->prev;
		node->next = x;
		x->prev = node;
		(items->size)++;
		return;
	}else{ //Insert in front half
		int i;
		NODE *x = items->head;
		NODE *y = items->head->next;

		for(i = 0; i < index; i++){
			x = x->next;
			y = y->next;
		}

		NODE *node = newDLLNode(value);

		x->next = node;
		node->prev = x;
		node->next = y;
		y->prev = node;
		(items->size)++;
		return;
	}
	return;
}

//Removes a value from items at the given index
void *removeDLL(DLL *items, int index){
	assert(items->size > 0);
	assert(index >= 0);
	assert(index < items->size);
	//Removes if only 1 item is in the list
	if(items->size == 1){
		void *valStore = items->tail->value; //Stores the value to be returned
		
		free(items->tail);

		items->tail = items->head;

		(items->size)--;

		return valStore;
	}else if(index == items->size - 1){ //Removes item at end of list
		void *valStore = items->tail->value; //Stores the value to be returned
		
		NODE* oldTail = items->tail; //Keeps track of the old tail for deletion later
		items->tail = items->tail->prev; //Makes the second to last node the new tail
		items->tail->next = NULL;

		free(oldTail); //Frees the old tail
		(items->size)--;

		return valStore;
	}else if(index > (items->size) / 2){
		int i;
		NODE *deleteNode = items->tail;
		for(i = items->size - 1; i > index; i--){
			deleteNode = deleteNode->prev;
		}

		void *valStore = deleteNode->value;

		deleteNode->next->prev = deleteNode->prev;
		deleteNode->prev->next = deleteNode->next;

		free(deleteNode);

		(items->size)--;

		return valStore;
	}else{ //Removes an item normally
		int i;
		NODE *deleteNode = items->head->next;
		for(i = 0; i < index; i++){ //Finds both the node to be deleted and the node before it
			deleteNode = deleteNode->next;
		}

		void *valStore = deleteNode->value; //Stores the value to be returned later

		deleteNode->next->prev = deleteNode->prev;
		deleteNode->prev->next = deleteNode->next; //Relinks the list so that the node is removed

		free(deleteNode); //Frees the old node

		(items->size)--;

		return valStore;
	}
}

//Appends donor to recipient, leaving donor empty
void unionDLL(DLL *recipient, DLL *donor){
	if((recipient->size == 0 && donor->size == 0)
		|| donor->size == 0) return; //Deals with two empty lists
	else if(recipient->size == 0){ //Deals with an empty recipient
		//Appends donor to recipient
		recipient->head->next = donor->head->next;
		donor->head->next->prev = recipient->head;
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
		donor->head->next->prev = recipient->tail;
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
void *getDLL(DLL *items, int index){
	assert(index >= 0);
	assert(index <items->size);

	if(index > (items->size) / 2){ //Insert in back half
		int i;
		NODE *x = items->tail;
		for(i = items->size - 1; i > index; i--){
			x = x->prev;
		}
		return x->value;
	}else{
		int i;
		NODE *x = items->head->next;
		for(i = 0; i < index; i++){
			x = x->next;
		}
		return x->value;
	}
	return NULL;
}

//Sets the value in items at the index to value and returns the replaced value
void *setDLL(DLL *items, int index, void *value){
	assert(index >= 0);
	assert(index <= items->size);

	if(index == items->size){ //Set after last item in list
		insertDLL(items, items->size, value);
		return NULL;
	}else if(index > (items->size) / 2){ //Set in back half
		int i;
		NODE *x = items->tail;
		for(i = items->size - 1; i > index; i--){
			x = x->prev;
		}
		void *y = x->value;
		x->value = value;
		return y;
	}else{ //Set in front half
		int i;
		NODE *x = items->head->next;
		for(i = 0; i < index; i++){
			x = x->next;
		}
		void *y = x->value;
		x->value = value;
		return y;
	}
}

//Returns the size of items
int sizeDLL(DLL *items){
	return items->size;
}

//Displays the contents of items to the place specified by fp 
//in the format {{1,2,3,...}}
void displayDLL(DLL *items, FILE *fp){
	fprintf(fp, "{{");
	NODE *x = items->head->next;
	int i;
	for(i = 0; i < items->size; i++){
		items->display(x->value, fp);
		if(i != items->size - 1){
			fprintf(fp, ",");
		}
		x = x->next;
	}
	fprintf(fp, "}}");
}

//Displays the contents of items to the place specified by fp 
//in the format head->{{1,2,3}},tail->{{3}}
void displayDLLdebug(DLL *items, FILE *fp){
	fprintf(fp, "head->");
	displayDLL(items, fp);
	fprintf(fp, ",tail->{{");
	if(items->tail->value != NULL){
		items->display(items->tail->value, fp);
	}
	fprintf(fp, "}}");
	return;
}

//Walks through the list and frees each value and each node, ending by
//freeing the list itself
void freeDLL(DLL *items){
	if(items->size == 0){
		if(items->free != NULL) items->free(items->head->value);
		free(items->head);
	}else if(items->size <= 1){ //If tail and head are only nodes
		if(items->free != NULL) items->free(items->head->value);
		free(items->head);
		if(items->free != NULL) items->free(items->tail->value);
		free(items->tail);
	}else{
		int i;

		for(i = 0; i <= items->size; i++){
			NODE *curNode = items->head;
			curNode = items->head;
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