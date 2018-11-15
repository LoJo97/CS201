#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "avl.h"
#include "bst.h"
#include "queue.h"

typedef struct avlval{
	void *value;
	int count;

	int leftHeight; //Height of the left subtree
	int rightHeight; //Height of the right subtree
	int height; //Max of leftHeight and rightHeight + 1
	int balance; //Balance factor: leftHeight - rightHeight. Can be -1, 0, or 1

	void (*display)(void *, FILE *);
	int  (*compare)(void *, void *);
	void (*free)(void *);
} AVLVAL;

AVLVAL *newAVLVAL(void *v, void (*d)(void *,FILE *), int (*c)(void *,void *), void (*f)(void *)){
	AVLVAL *newVal = malloc(sizeof(AVLVAL));

	newVal->value = v;
	newVal->count = 1;

	newVal->leftHeight = 0;
	newVal->rightHeight = 0;
	newVal->height = 1;
	newVal->balance = 0;

	newVal->display = d;
	newVal->compare = c;
	newVal->free = f;

	return newVal;
}

AVLVAL *getAVLVAL(BSTNODE *node){
	if(node == NULL) return NULL;
	return (AVLVAL*)getBSTNODEvalue(node);
}

void displayAVLVAL(void *v, FILE *fp){
	if(v == NULL){
		printf("NULL value\n");
		return;
	}

	AVLVAL *val = v;
	val->display(val->value, fp);
	if(val->count > 1) fprintf(fp, "[%d]", val->count);
	if(val->balance > 0) fprintf(fp, "+");
	if(val->balance < 0) fprintf(fp, "-");
	return;
}

int compareAVLVAL(void *a, void *b){
	if(!a || !b) return 0;

	AVLVAL *x = a;
	AVLVAL *y = b;

	return x->compare(x->value, y->value);
}

void swapAVLVAL(BSTNODE *a, BSTNODE *b){
	AVLVAL *aval = getBSTNODEvalue(a);
	AVLVAL *bval = getBSTNODEvalue(b);

	void *vtemp = aval->value;
	aval->value = bval->value;
	bval->value = vtemp;

	int ctemp = aval->count;
	aval->count = bval->count;
	bval->count = ctemp;

	return;
}

void freeAVLVAL(void *v){
	if(v == NULL) return;

	AVLVAL *val = v;

	val->free(val->value);
	free(val);
	return;
}

struct avl{
	BST *tree;
	int size;

	void (*display)(void *, FILE *);
	int (*compare)(void *,void *);
	void (*free)(void *);
};

AVL *newAVL(void (*d)(void *,FILE *), int (*c)(void *,void *), void (*f)(void *)){
	AVL *avl = malloc(sizeof(AVL));

	avl->tree = newBST(displayAVLVAL, compareAVLVAL, swapAVLVAL, freeAVLVAL);

	avl->size = 0;

	avl->display = d;
	avl->compare = c;
	avl->free = f;

	return avl;
}

int isRoot(AVL *a, BSTNODE *node){
	return !compareAVLVAL(getBSTNODEvalue(getBSTroot(a->tree)), getBSTNODEvalue(node));
}

BSTNODE *sibling(BSTNODE *node){
	if(!compareAVLVAL(getBSTNODEvalue(getBSTNODEparent(node)), getBSTNODEvalue(node))) return NULL; //Node is root
	//If node is parent's left child, return the right child
	if(getBSTNODEleft(getBSTNODEparent(node)) && 
	compareAVLVAL(getBSTNODEvalue(node), 
	getBSTNODEvalue(getBSTNODEleft(getBSTNODEparent(node)))) == 0){
		return getBSTNODEright(getBSTNODEparent(node));
	}else if(getBSTNODEright(getBSTNODEparent(node))){ //If node is parent's right child, return the left child
		return getBSTNODEleft(getBSTNODEparent(node));
	}
	return NULL;
}

int linear(BSTNODE *c, BSTNODE *p, BSTNODE *gp){
	return (getBSTNODEleft(gp) && getBSTNODEleft(p) &&
			!compareAVLVAL(getAVLVAL(getBSTNODEleft(gp)), getAVLVAL(p)) && 
			!compareAVLVAL(getAVLVAL(getBSTNODEleft(p)), getAVLVAL(c))) ||
		   (getBSTNODEright(gp) && getBSTNODEright(p) &&
		   	!compareAVLVAL(getAVLVAL(getBSTNODEright(gp)), getAVLVAL(p)) && 
			!compareAVLVAL(getAVLVAL(getBSTNODEright(p)), getAVLVAL(c)));
}

void setBalance(BSTNODE* node){
	if(node == NULL) return;

	AVLVAL *val = getBSTNODEvalue(node);

	//Sets the leftHeight
	if(getBSTNODEleft(node) == NULL) val->leftHeight = 0;
	else val->leftHeight = (getAVLVAL(getBSTNODEleft(node)))->height;

	//Sets the rightHeight
	if(getBSTNODEright(node) == NULL) val->rightHeight = 0;
	else val->rightHeight = (getAVLVAL(getBSTNODEright(node)))->height;

	//Sets the height
	if(val->leftHeight > val->rightHeight) val->height = val->leftHeight + 1;
	else val->height = val->rightHeight + 1;

	val->balance = val->leftHeight - val->rightHeight; //Sets the balance

	return;
}

void rotateRight(AVL *a, BSTNODE *p){
	BSTNODE *c = getBSTNODEleft(p);
	//Move c's right subtree to p's left subtree
	setBSTNODEleft(p, getBSTNODEright(c));
	if(getBSTNODEleft(p)) setBSTNODEparent(getBSTNODEleft(p), p);

	//Set p's parent to be c's parent
	setBSTNODEparent(c, getBSTNODEparent(p));
	if(!isRoot(a, p)){
		if(compareAVLVAL(getAVLVAL(c), getAVLVAL(getBSTNODEparent(c))) < 0){
			setBSTNODEleft(getBSTNODEparent(c), c);
		}else if(compareAVLVAL(getAVLVAL(c), getAVLVAL(getBSTNODEparent(c))) > 0){
			setBSTNODEright(getBSTNODEparent(c), c);
		}
	}else{ //Sets root if p is root
		setBSTroot(a->tree, c);
	}

	//Puts p on c's right
	setBSTNODEright(c, p);
	setBSTNODEparent(p, c);
	
	return;
}

void rotateLeft(AVL *a, BSTNODE *p){
	BSTNODE *c = getBSTNODEright(p);
	//Move c's left subtree to p's right subtree
	setBSTNODEright(p, getBSTNODEleft(c));
	if(getBSTNODEright(p)) setBSTNODEparent(getBSTNODEright(p), p);

	//Set p's parent to be c's parent
	setBSTNODEparent(c, getBSTNODEparent(p));
	if(!isRoot(a, p)){
		if(compareAVLVAL(getAVLVAL(c), getAVLVAL(getBSTNODEparent(c))) < 0){
			setBSTNODEleft(getBSTNODEparent(c), c);
		}else if(compareAVLVAL(getAVLVAL(c), getAVLVAL(getBSTNODEparent(c))) > 0){
			setBSTNODEright(getBSTNODEparent(c), c);
		}
	}else{ //Sets root if p is root
		setBSTroot(a->tree, c);
	}

	//Puts p on c's left
	setBSTNODEleft(c, p);
	setBSTNODEparent(p, c);
	
	return;
}

void insertionFixup(AVL *a, BSTNODE *node){
	BSTNODE *i = node;

	//Loop while index != root
	while(!isRoot(a, i)){
		BSTNODE *p = getBSTNODEparent(i);

		//Sets the fav of p
		BSTNODE *pFav = NULL;
		if(getAVLVAL(p)->balance > 0){
			pFav = getBSTNODEleft(p);
		}else if(getAVLVAL(p)->balance < 0){
			pFav = getBSTNODEright(p);
		}

		//If parent favors sibling
		if(pFav && sibling(i) &&
		   !compareAVLVAL(getAVLVAL(pFav), getAVLVAL(sibling(i)))){
			setBalance(p);
			return;
		}else if(!pFav){ //Parent has no favorite
			setBalance(p);
			i = p;
		}else{//Parent favors index
			BSTNODE *c = NULL;
			if(getAVLVAL(i)->balance > 0){
				c = getBSTNODEleft(i);
			}else if(getAVLVAL(i)->balance < 0){
				c = getBSTNODEright(i);
			}

			int balParent = getAVLVAL(p)->balance;

			if(c && !linear(c, i, p)){
				if(balParent < 0){
					rotateRight(a, i);
					rotateLeft(a, p);

					setBalance(i);
					setBalance(p);
					setBalance(c);
				}else if(balParent > 0){
					rotateLeft(a, i);
					rotateRight(a, p);

					setBalance(i);
					setBalance(p);
					setBalance(c);
				}
			}else{ //Is linear
				if(balParent > 0){
					rotateRight(a, p);

					setBalance(p);
					setBalance(i);
				}else if(balParent < 0){
					rotateLeft(a, p);

					setBalance(p);
					setBalance(i);
				}
			}
			return;
		}
	}
}

void insertAVL(AVL *a, void *v){
	AVLVAL *val = newAVLVAL(v, a->display, a->compare, a->free);
	AVLVAL *existentValue = NULL;
	BSTNODE *found = findBST(a->tree, val);

	if(found != NULL){
		existentValue = getBSTNODEvalue(found);
		(existentValue->count)++;
		(a->size)++;
		free(val);
		return;
	}

	BSTNODE *newNode = insertBST(a->tree, val);
	insertionFixup(a, newNode);

	setBSTNODEparent(getBSTroot(a->tree), getBSTroot(a->tree));
	(a->size)++;
	return;
}

int findAVLcount(AVL *a, void *v){
	AVLVAL *value = newAVLVAL(v, a->display, a->compare, a->free);
	BSTNODE *bstNode = findBST(a->tree, value);
	free(value);

	if(!bstNode){
		return 0;
	}
	return getAVLVAL(bstNode)->count;
}

void *findAVL(AVL *a, void *v){
	AVLVAL *value = newAVLVAL(v, a->display, a->compare, a->free);
	AVLVAL *found = NULL;
	BSTNODE *node = findBST(a->tree, value);

	if(node != NULL){
		found = getBSTNODEvalue(node);
	}

	free(value);
	if(found == NULL) return NULL;
	void *val = found->value;

	return val;
}

void deleteFixup(AVL *a, BSTNODE* node){
	getAVLVAL(node)->height = 0; //Sets height to 0 of node to be pruned

	BSTNODE *i = node;

	while(!isRoot(a, i)){
		BSTNODE *p = getBSTNODEparent(i);
		
		//Sets the fav of p
		BSTNODE *pFav = NULL;
		if(getAVLVAL(p)->balance > 0){
			pFav = getBSTNODEleft(p);
		}else if(getAVLVAL(p)->balance < 0){
			pFav = getBSTNODEright(p);
		}

		//Favors i
		if(pFav &&
		   !compareAVLVAL(getBSTNODEvalue(pFav), getBSTNODEvalue(i))){
			setBalance(p);
			i = p;
		}else if(!pFav){ //Favors none
			setBalance(p);
			return;
		}else{ //Favors sibling
			BSTNODE *s = sibling(i);
			BSTNODE *c = NULL;
			if(getAVLVAL(s)->balance > 0){
				c = getBSTNODEleft(s);
			}else if(getAVLVAL(s)->balance < 0){
				c = getBSTNODEright(s);
			}

			int balParent = getAVLVAL(p)->balance;

			if(c && !linear(c, s, p)){
				if(balParent > 0){
					rotateLeft(a, s);
					rotateRight(a, p);

					setBalance(s);
					setBalance(p);
					setBalance(c);
					i = c;
				}else if(balParent < 0){
					rotateRight(a, s);
					rotateLeft(a, p);

					setBalance(s);
					setBalance(p);
					setBalance(c);
					i = c;
				}
			}else{
				if(balParent > 0){
					rotateRight(a, p);
					
					setBalance(p);
					setBalance(s);
					if(c == NULL) return;
					i = s;
				}else if(balParent < 0){
					rotateLeft(a, p);
					
					setBalance(p);
					setBalance(s);
					if(c == NULL) return;
					i = s;
				}
			}
		}
	}
}

void *deleteAVL(AVL *a, void *v){
	AVLVAL *value = newAVLVAL(v, a->display, a->compare, a->free);
	BSTNODE *bstNode = findBST(a->tree, value);
	free(value);

	if(bstNode == NULL) return NULL;

	AVLVAL *avlValue = getBSTNODEvalue(bstNode);

	if(avlValue->count > 1){
		(avlValue->count)--;
		(a->size)--;
		return NULL;
	}

	bstNode = swapToLeafBST(a->tree, bstNode);
	avlValue = getBSTNODEvalue(bstNode);
	deleteFixup(a, bstNode);
	pruneLeafBST(a->tree, bstNode);
	(a->size)--;
	setBSTsize(a->tree, sizeBST(a->tree) - 1);
	void *deletedVal = avlValue->value;
	free(bstNode);
	free(avlValue);
	setBSTNODEparent(getBSTroot(a->tree), getBSTroot(a->tree));
	return deletedVal;
}

int sizeAVL(AVL *a){
	return sizeBST(a->tree);
}

int duplicatesAVL(AVL *a){
	return a->size - sizeBST(a->tree);
}

void statisticsAVL(AVL *a, FILE *fp){
	fprintf(fp, "Duplicates: %d\n", duplicatesAVL(a));
	statisticsBST(a->tree, fp);
	return;
}

void displayAVL(AVL *a, FILE *fp){
	displayBSTdecorated(a->tree, fp);
	return;
}

void displayAVLdebug(AVL *a, FILE *fp){
	displayBST(a->tree, fp);
	return;
}

void freeAVL(AVL *a){
	freeBST(a->tree);
	free(a);
	return;
}