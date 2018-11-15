#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include "bst.h"
#include "queue.h"
#include "stack.h"

static void heapify(HEAP *h, BSTNODE *root);

struct heap{
	BST *tree;
	QUEUE *q;
	QUEUE *extractQueue;
	STACK *s;

	int size;

	int (*compare)(void *, void *);
	void (*display)(void *, FILE *);
};

HEAP *newHEAP(void (*d)(void *, FILE *), int (*c)(void *, void *), void (*f)(void *)){
	HEAP *h = malloc(sizeof(HEAP)); 
		h->tree = newBST(d, c, NULL, f);
		h->q = newQUEUE(NULL, NULL);
		h->extractQueue = newQUEUE(NULL, NULL);
		h->s = newSTACK(NULL, NULL);

		h->size = 0;

		h->compare = c;
		h->display = d;

	return h;
}

void insertHEAP(HEAP *h, void *value){
	if(sizeBST(h->tree) == 0){
		setBSTroot(h->tree, newBSTNODE(value));
		setBSTsize(h->tree, 1);
		enqueue(h->q, getBSTroot(h->tree));
		push(h->s, getBSTroot(h->tree));
	}else{
		if(getBSTNODEleft(peekQUEUE(h->q)) == NULL){
			BSTNODE *temp = peekQUEUE(h->q);
			setBSTNODEleft(temp, newBSTNODE(value));
			enqueue(h->q, getBSTNODEleft(temp));
			push(h->s, temp);
		}else{
			BSTNODE *temp = dequeue(h->q);
			setBSTNODEright(temp, newBSTNODE(value));
			enqueue(h->q, getBSTNODEright(temp));
		}
		setBSTsize(h->tree, sizeBST(h->tree) + 1);
	}
	(h->size)++;
	return;
}

void buildHEAP(HEAP *h){
	while(sizeSTACK(h->s) > 0){
		BSTNODE *temp = pop(h->s);
		enqueue(h->extractQueue, temp);
		heapify(h, temp);
	}
	return;
}

void *peekHEAP(HEAP *h){
	return getBSTNODEvalue(getBSTroot(h->tree));
}

void *extractHEAP(HEAP *h){
	if(h->size == 1){
		BSTNODE *root = getBSTroot(h->tree);
		void *value = getBSTNODEvalue(root);
		setBSTroot(h->tree, NULL);
		setBSTsize(h->tree, sizeBST(h->tree) - 1);
		(h->size)--;
		freeBSTNODE(root, NULL);
		return value;
	}

	BSTNODE *root = getBSTroot(h->tree);
	void *value = getBSTNODEvalue(root);
	BSTNODE *leaf;
	if(getBSTNODEright(peekQUEUE(h->extractQueue)) == NULL){
		BSTNODE *parent = dequeue(h->extractQueue);
		leaf = getBSTNODEleft(parent);
		setBSTNODEleft(parent, NULL);
		setBSTNODEparent(leaf, NULL);
	}else{
		BSTNODE *parent = peekQUEUE(h->extractQueue);
		leaf = getBSTNODEright(parent);
		setBSTNODEright(parent, NULL);
		setBSTNODEparent(leaf, NULL);
	}
	setBSTsize(h->tree, sizeBST(h->tree) - 1);
	(h->size)--;

	setBSTNODEvalue(root, getBSTNODEvalue(leaf));

	freeBSTNODE(leaf, NULL);

	heapify(h, root);
	return value;
}

int sizeHEAP(HEAP *h){
	return h->size;
}

void displayHEAP(HEAP *h, FILE *fp){
	displayBST(h->tree, fp);
	return;
}

void displayHEAPdebug(HEAP *h, FILE *fp){
	printf("heap size: %d\n", h->size);
	printf("bst size: %d\n", sizeBST(h->tree));
	displayBSTdebug(h->tree, fp);
	return;
}

void freeHEAP(HEAP *h){
	freeQUEUE(h->q);
	freeQUEUE(h->extractQueue);
	freeSTACK(h->s);
	freeBST(h->tree);
	free(h);
	return;
}

void heapify(HEAP *h, BSTNODE *root){
	if(root == NULL){
		return;
	}

	BSTNODE *lChild = getBSTNODEleft(root);
	BSTNODE *rChild = getBSTNODEright(root);

	if(lChild == NULL && rChild == NULL){
		return;
	}

	if(rChild == NULL){
		if(h->compare(getBSTNODEvalue(root), getBSTNODEvalue(lChild)) > 0){
			void *temp = getBSTNODEvalue(root);

			setBSTNODEvalue(root, getBSTNODEvalue(lChild));
			setBSTNODEvalue(lChild, temp);

			return;
		}
		return;
	}

	if(h->compare(getBSTNODEvalue(rChild), getBSTNODEvalue(lChild)) >= 0){
		if(h->compare(getBSTNODEvalue(root), getBSTNODEvalue(lChild)) >= 0){
			void *temp = getBSTNODEvalue(root);

			setBSTNODEvalue(root, getBSTNODEvalue(lChild));
			setBSTNODEvalue(lChild, temp);

			return heapify(h, lChild);
		}
	}

	if(h->compare(getBSTNODEvalue(lChild), getBSTNODEvalue(rChild)) > 0){
		if(h->compare(getBSTNODEvalue(root), getBSTNODEvalue(rChild)) > 0){
			void *temp = getBSTNODEvalue(root);

			setBSTNODEvalue(root, getBSTNODEvalue(rChild));
			setBSTNODEvalue(rChild, temp);

			return heapify(h, rChild);
		}
	}
	return;
}