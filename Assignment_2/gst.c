#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gst.h"
#include "bst.h"
#include "queue.h"

typedef struct gstval{
	void *value;
	int count;

	void (*display)(void *, FILE *);
	int  (*compare)(void *, void *);
	void (*free)(void *);
} GSTVAL;

GSTVAL *newGSTVAL(void *v, void (*d)(void *,FILE *), int (*c)(void *,void *), void (*f)(void *)){
	GSTVAL *newVal = malloc(sizeof(GSTVAL));

	newVal->value = v;
	newVal->count = 1;

	newVal->display = d;
	newVal->compare = c;
	newVal->free = f;

	return newVal;
}

void displayGSTVAL(void *v, FILE *fp){
	if(!v) return;
	GSTVAL *val = v;

	val->display(val->value, fp);
	if(val->count > 1) fprintf(fp, "[%d]", val->count);
	return;
}

int compareGSTVAL(void *a, void *b){
	if(!a || !b) return 0; //New
	GSTVAL *x = a;
	GSTVAL *y = b;

	return x->compare(x->value, y->value);
}

void freeGSTVAL(void *v){
	if(v == NULL) return; //New
	GSTVAL *val = v;

	val->free(val->value);
	free(val);
	return;
}

struct gst{
	BST *tree;
	int size;

	void (*display)(void *, FILE *);
	int (*compare)(void *,void *);
	void (*free)(void *);
};

GST *newGST(void (*d)(void *,FILE *), int (*c)(void *,void *), void (*f)(void *)){
	GST *gst = malloc(sizeof(GST));

	gst->tree = newBST(displayGSTVAL, compareGSTVAL, NULL, freeGSTVAL);

	gst->size = 0;

	gst->display = d;
	gst->compare = c;
	gst->free = f;

	return gst;
}

void insertGST(GST *g, void *v){
	GSTVAL *val = newGSTVAL(v, g->display, g->compare, g->free);
	GSTVAL *existentValue = NULL;
	BSTNODE *found = findBST(g->tree, val);

	if(found != NULL){
		existentValue = getBSTNODEvalue(found);
		(existentValue->count)++;
		(g->size)++;
		freeGSTVAL(val);
		return;
	}

	insertBST(g->tree, val);
	setBSTNODEparent(getBSTroot(g->tree), getBSTroot(g->tree));
	(g->size)++;

	return;
}

int findGSTcount(GST *g, void *v){
	GSTVAL *value = newGSTVAL(v, g->display, g->compare, g->free);
	BSTNODE *bstNode = findBST(g->tree, value);
	free(value);

	if(!bstNode){
		return 0;
	}
	return ((GSTVAL*)getBSTNODEvalue(bstNode))->count;
}

void *findGST(GST *g, void *v){
	GSTVAL *value = newGSTVAL(v, g->display, g->compare, g->free);
	GSTVAL *found = NULL;
	BSTNODE *node = findBST(g->tree, value);

	if(node != NULL){
		found = getBSTNODEvalue(node);
	}

	free(value);
	if(found == NULL) return NULL;
	void *val = found->value;

	return val;
}

void *deleteGST(GST *g, void *v){
	GSTVAL *value = newGSTVAL(v, g->display, g->compare, g->free);
	BSTNODE *bstNode = findBST(g->tree, value);
	free(value);

	if(bstNode == NULL) return NULL;

	GSTVAL *gstValue = getBSTNODEvalue(bstNode);

	if(gstValue->count > 1){
		(gstValue->count)--;
		(g->size)--;
		return NULL;
	}

	bstNode = deleteBST(g->tree, gstValue);
	GSTVAL *deletedVal = getBSTNODEvalue(bstNode);
	void *returnedVal = deletedVal->value;
	free(bstNode);
	free(deletedVal);
	(g->size)--;
	setBSTNODEparent(getBSTroot(g->tree), getBSTroot(g->tree));
	return returnedVal;
}

int sizeGST(GST *g){
	return sizeBST(g->tree);
}

int duplicates(GST *g){
	return g->size - sizeBST(g->tree);
}

void statisticsGST(GST *g, FILE *fp){
	printf("Duplicates: %d", duplicates(g));
	printf("\n");
	statisticsBST(g->tree, fp);
	return;
}

void displayGST(GST *g, FILE *fp){
	displayBSTdecorated(g->tree, fp);
	return;
}

void displayGSTdebug(GST *g, FILE *fp){
	displayBST(g->tree, fp);
	return;
}

void freeGST(GST *g){
	freeBST(g->tree);
	free(g);
	return;
}