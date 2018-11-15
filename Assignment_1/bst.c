#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"

static void displayBSThelper(void (*dis)(void *, FILE *), BSTNODE *root, FILE *fp);
static BSTNODE *findBSThelper(int (*comp)(void *, void *), BSTNODE *root, void *value);
static BSTNODE *insertBSThelper(BST *t, BSTNODE *root, void *value);
static int findMinDepth(BST *t);
static int findMaxDepth(BST *t);

struct bstnode{
	void *value;
	struct bstnode *left;
	struct bstnode *right;
	struct bstnode *parent;
};

BSTNODE *newBSTNODE(void *v){
	BSTNODE *newNode = malloc(sizeof(BSTNODE));
		newNode->value = v;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->parent = NULL;

	return newNode;
}

void *getBSTNODEvalue(BSTNODE *n){
	return n->value;
}

void setBSTNODEvalue(BSTNODE *n, void *value){
	n->value = value;
	return;
}

BSTNODE *getBSTNODEleft(BSTNODE *n){
	return n->left;
}

void setBSTNODEleft(BSTNODE *n, BSTNODE *replacement){
	n->left = replacement;
	return;
}

BSTNODE *getBSTNODEright(BSTNODE *n){
	return n->right;
}

void setBSTNODEright(BSTNODE *n, BSTNODE *replacement){
	n->right = replacement;
	return;
}

BSTNODE *getBSTNODEparent(BSTNODE *n){
	return n->left;
}

void setBSTNODEparent(BSTNODE *n, BSTNODE *replacement){
	n->left = replacement;
	return;
}

void freeBSTNODE(BSTNODE *n, void (*f)(void *)){
	if(f != NULL) f(n->value);
	free(n);
}

void swapBSTNODE(BSTNODE *parent, BSTNODE *child){
	void *temp = parent->value;

	parent->value = child->value;
	child->value = temp;
	
	return;
}

struct bst{
	BSTNODE *root;
	int size;

	void (*display)(void *, FILE *);
	int (*compare)(void *, void *);
	void (*swapper)(BSTNODE *, BSTNODE *);
	void (*free)(void *);
};

BST *newBST(void (*d)(void *, FILE *), int (*c)(void *, void *), 
	void (*s)(BSTNODE *, BSTNODE *), void (*f)(void *)){
	BST *tree = malloc(sizeof(BST));
		tree->root = NULL;
		tree->size = 0;

		tree->display = d;
		tree->compare = c;
		if(s == NULL){
			tree->swapper = swapBSTNODE;
		}else{
			tree->swapper = s;
		}
		tree->free    = f;

	return tree;
}

BSTNODE *getBSTroot(BST *t){
	return t->root;
}

void setBSTroot(BST *t, BSTNODE *replacement){
	t->root = replacement;
	return;
}

void setBSTsize(BST *t, int s){
	t->size = s;
	return;
}

BSTNODE *insertBST(BST *t, void *value){
	if(t->root == NULL){
		t->root = newBSTNODE(value);
		(t->size)++;
		return t->root;
	}else{
		return insertBSThelper(t, t->root, value);
	}
	return NULL;
}

BSTNODE *insertBSThelper(BST *t, BSTNODE *root, void *value){
	if(t->compare(value, root->value) < 0){
		if(root->left == NULL){
			root->left = newBSTNODE(value);
			root->left->parent = root;
			(t->size)++;
			return root->left;
		}else{
			return insertBSThelper(t, root->left, value);
		}
	}else{
		if(root->right == NULL){
			root->right = newBSTNODE(value);
			root->right->parent = root;
			(t->size)++;
			return root->right;
		}else{
			return insertBSThelper(t, root->right, value);
		}
	}
	return NULL;
}

BSTNODE *findBST(BST *t, void *value){
	if(t->root == NULL){
		return NULL;
	}else if(t->compare(value, t->root->value) == 0){
		return t->root;
	}else if(t->compare(value, t->root->value) < 0){
		return findBSThelper(t->compare, t->root->left, value);
	}else if(t->compare(value, t->root->value) > 0){
		return findBSThelper(t->compare, t->root->right, value);
	}
	return NULL;
}

BSTNODE *findBSThelper(int (*comp)(void *, void *), BSTNODE *root, void *value){
	if(root == NULL){
		return NULL;
	}else if(comp(value, root->value) == 0){
		return root;
	}else if(comp(value, root->value) < 0){
		return findBSThelper(comp, root->left, value);
	}else if(comp(value, root->value) > 0){
		return findBSThelper(comp, root->right, value);
	}
	return NULL;
}

BSTNODE *deleteBST(BST *t, void *value){
	BSTNODE *del = findBST(t, value);
	del = swapToLeafBST(t, del);
	pruneLeafBST(t, del);
	(t->size)--;
	return del;
}

BSTNODE *swapToLeafBST(BST *t, BSTNODE *node){
	if(node->right != NULL){
		BSTNODE *temp = node->right;

		while(temp->left != NULL){
			temp = temp->left;
		}

		t->swapper(node, temp);

		return swapToLeafBST(t, temp);
	}else if(node->left != NULL){
		BSTNODE *temp = node->left;

		while(temp->right != NULL){
			temp = temp->right;
		}

		t->swapper(node, temp);

		return swapToLeafBST(t, temp);
	}else{
		return node;
	}
	return node;
}

void pruneLeafBST(BST *t, BSTNODE *leaf){
	if(leaf->parent == NULL){
		t->root = NULL;
		return;
	}else if(leaf->parent->left != NULL && t->compare(leaf->parent->left->value, leaf->value) == 0){
		leaf->parent->left = NULL;
		leaf->parent = NULL;
	}else if(leaf->parent->right != NULL && t->compare(leaf->parent->right->value, leaf->value) == 0){
		leaf->parent->right = NULL;
		leaf->parent = NULL;
	}
	return;
}

int sizeBST(BST *t){
	return t->size;
}

void statisticsBST(BST *t, FILE *fp){
	fprintf(fp, "Nodes: %d\n", t->size);
	fprintf(fp, "Minimum depth: %d\n", findMinDepth(t));
	fprintf(fp, "Maximum depth: %d\n", findMaxDepth(t));
	return;
}

int findMinDepth(BST *t){
	int minDepth = -1;

	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, t->root);
	enqueue(queue, NULL);

	while(peekQUEUE(queue) != NULL){
		while(peekQUEUE(queue) != NULL){
			BSTNODE *temp = dequeue(queue);

			if(temp->left == NULL || temp->right == NULL){
				freeQUEUE(queue);
				minDepth++;
				return minDepth;
			}

			if(temp->left != NULL){
				enqueue(queue, temp->left);
			}

			if(temp->right != NULL){
				enqueue(queue, temp->right);
			}
		}

		dequeue(queue);
		enqueue(queue, NULL);
		minDepth++;
	}
	freeQUEUE(queue);
	return minDepth;
}

int findMaxDepth(BST *t){
	int maxDepth = -1;

	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, t->root);
	enqueue(queue, NULL);

	while(peekQUEUE(queue) != NULL){
		while(peekQUEUE(queue) != NULL){
			BSTNODE *temp = dequeue(queue);

			if(temp->left != NULL){
				enqueue(queue, temp->left);
			}

			if(temp->right != NULL){
				enqueue(queue, temp->right);
			}
		}

		dequeue(queue);
		enqueue(queue, NULL);
		maxDepth++;
	}
	freeQUEUE(queue);
	return maxDepth;
}

void displayBST(BST *t, FILE *fp){
	fprintf(fp, "[");
	if(t->root != NULL){
		t->display(t->root->value, fp);
		displayBSThelper(t->display, t->root->left, fp);
		displayBSThelper(t->display, t->root->right, fp);
	}
	fprintf(fp, "]");
	return;
}

void displayBSThelper(void (*dis)(void *, FILE *), BSTNODE *root, FILE *fp){
	if(root == NULL){
		return;
	}
	fprintf(fp, " [");
	dis(root->value, fp);
	displayBSThelper(dis, root->left, fp);
	displayBSThelper(dis, root->right, fp);
	fprintf(fp, "]");
	return;
}

void displayBSTdebug(BST *t, FILE *fp){
	if(t->root == NULL){
		return;
	}

	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, t->root);
	enqueue(queue, NULL);

	while(peekQUEUE(queue) != NULL){
		while(peekQUEUE(queue) != NULL){
			BSTNODE *temp = dequeue(queue);
			t->display(temp->value, fp);

			if(temp->left != NULL){
				enqueue(queue, temp->left);
			}
			if(temp->right != NULL){
				enqueue(queue, temp->right);
			}
			if(peekQUEUE(queue) != NULL){
				printf(" ");
			}
		}
		dequeue(queue);
		enqueue(queue, NULL);
		printf("\n");
	}
	freeQUEUE(queue);
	return;
}

void freeBST(BST *t){
	if(t->root == NULL){
		freeBSTNODE(t->root, NULL);
		free(t);
		return;
	}

	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, t->root);
	enqueue(queue, NULL);

	while(peekQUEUE(queue) != NULL){
		while(peekQUEUE(queue) != NULL){
			BSTNODE *temp = dequeue(queue);

			if(temp->left != NULL){
				enqueue(queue, temp->left);
			}
			if(temp->right != NULL){
				enqueue(queue, temp->right);
			}
			freeBSTNODE(temp, t->free);
		}
		dequeue(queue);
		enqueue(queue, NULL);
	}
	freeQUEUE(queue);
	free(t);
	return;
}