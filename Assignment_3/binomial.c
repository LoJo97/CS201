#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "binomial.h"
#include "queue.h"
#include "dll.h"

typedef struct binNode{
	void *value;
	struct binNode *parent;
	DLL *children;
	void *owner;

	void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*update)(void *,void *);
    void (*free)(void *);
}BHNODE;

BHNODE *newBHNODE(void *value, void (*d)(void *,FILE *), int (*c)(void *,void *), void (*u)(void *,void *), void (*f)(void *)){
	BHNODE *newNode = malloc(sizeof(BHNODE));
		newNode->value = value;
		newNode->parent = NULL;
		newNode->children = NULL;
		newNode->owner = NULL;

		newNode->display = d;
		newNode->compare = c;
		newNode->update = u;
		newNode->free = f;

	return newNode;
}

void displayBHNODE(void *value, FILE *fp){
	if(!value) return;

	BHNODE *node = value;

	return node->display(node->value, fp);
}

int compareBHNODE(void *a, void *b){
	if(!a || !b) return 0;

	BHNODE *x = a;
	BHNODE *y = b;

	return x->compare(x->value, y->value);
}

void freeBHNODE(void *value){
	BHNODE *node = value;

	if(!node) return;

	node->free(node->value);
	free(node);

	return;
}

struct binomial{
	DLL *rootList;
	BHNODE *extreme;
	int size;

	void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*update)(void *,void *);
    void (*free)(void *);
};

void consolidate(BINOMIAL *b);
void updateConsolidationArray(BHNODE **array, BHNODE *spot);
BHNODE *combine(BHNODE *x, BHNODE *y);
void decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value);
void *extractBINOMIAL(BINOMIAL *b);
BHNODE *bubbleUp(BINOMIAL *b, BHNODE* n);
void freeBHNODEchildren(BHNODE *parent);

BINOMIAL *newBINOMIAL(void (*d)(void *,FILE *), int (*c)(void *,void *), void (*u)(void *,void *), void (*f)(void *)){
	BINOMIAL *b = malloc(sizeof(BINOMIAL));
		b->rootList = newDLL(displayBHNODE, freeBHNODE);
		b->extreme = NULL;
		b->size = 0;

		b->display = d;
		b->compare = c;
		b->update = u;
		b->free = f;
	
	return b;
}

void *insertBINOMIAL(BINOMIAL *b, void *value){
	BHNODE *node = newBHNODE(value, b->display, b->compare, b->update, b->free);
		node->parent = node;
		node->children = newDLL(displayBHNODE, freeBHNODE);
		node->owner = insertDLL(b->rootList, 0, node);
	b->size++;
	consolidate(b);
	return node;
}

int sizeBINOMIAL(BINOMIAL *b){
	return b->size;
}

void unionBINOMIAL(BINOMIAL *b, BINOMIAL *donor){
	unionDLL(b->rootList, donor->rootList);
	b->size += donor->size;
	donor->rootList = newDLL(displayBHNODE, freeBHNODE);
	donor->size = 0;
	donor->extreme = NULL;
	consolidate(b);
	return;
}

void deleteBINOMIAL(BINOMIAL *b, void *node){
	decreaseKeyBINOMIAL(b, node, NULL);
	extractBINOMIAL(b);
	return;
}

void decreaseKeyBINOMIAL(BINOMIAL *b, void *node, void *value){
	BHNODE *n = node;
	n->value = value;
	BHNODE *newHolder = bubbleUp(b, n);
	if(b->compare(b->extreme->value, value) > 0) b->extreme = newHolder;
	return;
}

BHNODE *bubbleUp(BINOMIAL *b, BHNODE* n){
    BHNODE *p = n->parent;
    if (!compareBHNODE(n, p)) return n;
    if (b->compare(n->value, p->value) >= 0) return n;
    if (b->update) b->update(n->value, p);
    if (b->update) b->update(p->value, n);
    void *temp = n->value;
    n->value = p->value;
    p->value = temp;
    return bubbleUp(b, p);
}

void *peekBINOMIAL(BINOMIAL *b){
	return b->extreme->value;
}

void *extractBINOMIAL(BINOMIAL *b){
	BHNODE *x = removeDLLnode(b->rootList, b->extreme->owner);
	//the children of x are a linked list
	int i;
	for(i = 0; i < sizeDLL(x->children); i++){
		BHNODE *node = getDLL(x->children, i);
		node->parent = node;
	}
	//unionDLL(b->rootList, x->children);
	unionDLL(x->children, b->rootList);
	freeDLL(b->rootList);
	b->rootList = x->children;
	consolidate(b);
	b->size--;
	void *returnedVal = x->value;
	free(x);
	return returnedVal;
}

void consolidate(BINOMIAL *b){
	//create the consolidation array
	int size = log(b->size * 1.0)/log(2.0) + 1.0;
	BHNODE *consolidationArray[size];
	int i;
	for(i = 0; i < size; i++) consolidationArray[i] = NULL;

	//place all the subheaps in the array, combining as necessary
	while(sizeDLL(b->rootList)){
		BHNODE *spot = removeDLL(b->rootList, 0);
		updateConsolidationArray(consolidationArray, spot);
	}

	//transfer the array back to the heap, keeping track of the extreme value
	b->extreme = NULL;
	for(i = 0; i < size; i++){
		if(consolidationArray[i]){
			consolidationArray[i]->owner = insertDLL(b->rootList, sizeDLL(b->rootList), consolidationArray[i]);
			if(!b->extreme || compareBHNODE(b->extreme, consolidationArray[i]) > 0)
				b->extreme = consolidationArray[i];
		}
	}
	return;
}

void updateConsolidationArray(BHNODE **array, BHNODE *spot){
	int degree = sizeDLL(spot->children);
	while(array[degree]){
		spot = combine(array[degree], spot);
		array[degree] = NULL;
		degree++;
	}
	array[degree] = spot;
	return;
}

BHNODE *combine(BHNODE *x, BHNODE *y){
	if(compareBHNODE(x, y) <= 0){
		y->owner = insertDLL(x->children, sizeDLL(x->children), y);
		y->parent = x;
		return x;
	}else{
		x->owner = insertDLL(y->children, sizeDLL(y->children), x);
		x->parent = y;
		return y;
	}
	return NULL;
}

void statisticsBINOMIAL(BINOMIAL *b, FILE *fp){
	fprintf(fp, "size: %d\n", sizeBINOMIAL(b));
	fprintf(fp, "rootlist size: %d\n", sizeDLL(b->rootList));
	fprintf(fp, "extreme: ");
	displayBHNODE(b->extreme, fp);
	fprintf(fp, "\n");
	return;
}

void displayBINOMIAL(BINOMIAL *b, FILE *fp){
	printf("rootlist: ");
	int lastDegree = -1;
	int index = 0;
	while(index < sizeDLL(b->rootList)){
		BHNODE *node = getDLL(b->rootList, index);
		int degree = sizeDLL(node->children);
		if(degree - lastDegree > 1){
			int j = 1;
			while(j < degree - lastDegree){
				fprintf(fp, "NULL ");
				j++;
			}
		}
		displayBHNODE(node, fp);
		if(b->extreme == node) fprintf(fp, "*");
		if(index != sizeDLL(b->rootList) - 1) fprintf(fp, " ");
		index++;
		lastDegree = degree;
	}
	fprintf(fp, "\n");

	return;
}

void displayBINOMIALdebug(BINOMIAL *b, FILE *fp){
	if(!b->rootList) return;
	
	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, b->rootList);
	enqueue(queue, NULL);

	while(peekQUEUE(queue)){
		while(peekQUEUE(queue)){
			DLL *tempList = dequeue(queue);
			displayDLL(tempList, fp);
			int i = 0;
			while(i < sizeDLL(tempList)){
				if(sizeDLL(((BHNODE*)getDLL(tempList, i))->children))
					enqueue(queue, ((BHNODE*)getDLL(tempList, i))->children);
				i++;
			}
		}
		dequeue(queue);
		enqueue(queue, NULL);
		fprintf(fp, "\n");
	}
	freeQUEUE(queue);
	return;
}

/*
void freeBINOMIAL(BINOMIAL *b){
	//if(!b->rootList) return;
	
	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, b->rootList);
	enqueue(queue, NULL);

	while(peekQUEUE(queue)){
		while(peekQUEUE(queue)){
			DLL *tempList = dequeue(queue);
			int i = 0;
			while(i < sizeDLL(tempList)){
				if(((BHNODE*)getDLL(tempList, i))->children)
				enqueue(queue, ((BHNODE*)getDLL(tempList, i))->children);
				i++;
			}
			freeDLL(tempList);
		}
		dequeue(queue);
		enqueue(queue, NULL);
	}
	freeQUEUE(queue);
	free(b);
	return;
}
*/

void freeBINOMIAL(BINOMIAL *b){
	int i;
	for(i = 0; i < sizeDLL(b->rootList); i++){
		freeBHNODEchildren(getDLL(b->rootList, i));
	}
	freeDLL(b->rootList);
	free(b);
	return;
}

void freeBHNODEchildren(BHNODE *parent){
	if(!sizeDLL(parent->children)){
		freeDLL(parent->children);
		return;
	}
	int i = 0;
	while(i < sizeDLL(parent->children)){
		BHNODE *x = getDLL(parent->children, i++);
		freeBHNODEchildren(x);
	}
	freeDLL(parent->children);
	return;
}
