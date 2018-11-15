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

//Defines a BSTNODE
struct bstnode{
	void *value;
	struct bstnode *left;
	struct bstnode *right;
	struct bstnode *parent;
};

//Creates a new BSTNODE
BSTNODE *newBSTNODE(void *v){
	BSTNODE *newNode = malloc(sizeof(BSTNODE));
		newNode->value = v;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->parent = NULL;

	return newNode;
}

//Returns the value of a BSTNODE
void *getBSTNODEvalue(BSTNODE *n){
	if(n == NULL) return NULL;
	return n->value;
}

//Sets a BSTNODE's value
void setBSTNODEvalue(BSTNODE *n, void *value){
	n->value = value;
	return;
}

//Gets the left child of a BSTNODE
BSTNODE *getBSTNODEleft(BSTNODE *n){
	return n->left;
}

//Sets the left child of a BSTNODE
void setBSTNODEleft(BSTNODE *n, BSTNODE *replacement){
	n->left = replacement;
	return;
}

//Gets the right child of a BSTNODE
BSTNODE *getBSTNODEright(BSTNODE *n){
	return n->right;
}

//Sets the right child of a BSTNODE
void setBSTNODEright(BSTNODE *n, BSTNODE *replacement){
	n->right = replacement;
	return;
}

//Gets the parent of a BSTNODE
BSTNODE *getBSTNODEparent(BSTNODE *n){
	return n->parent;
}

//Sets the parent of a BSTNODE
void setBSTNODEparent(BSTNODE *n, BSTNODE *replacement){
	n->parent = replacement;
	return;
}

//Frees a BSTNODE and its value
void freeBSTNODE(BSTNODE *n, void (*f)(void *)){
	if(f != NULL) f(n->value);
	free(n);
	return;
}

//Swaps the values of two BSTNODEs
void swapBSTNODE(BSTNODE *parent, BSTNODE *child){
	void *temp = parent->value;

	parent->value = child->value;
	child->value = temp;
	
	return;
}

//Defines a BST
struct bst{
	BSTNODE *root;
	int size;

	void (*display)(void *, FILE *);
	int  (*compare)(void *, void *);
	void (*swapper)(BSTNODE *, BSTNODE *);
	void (*free)(void *);
};

//Creates a new BST
BST *newBST(void (*d)(void *, FILE *), int (*c)(void *, void *), 
	void (*s)(BSTNODE *, BSTNODE *), void (*f)(void *)){
	BST *tree = malloc(sizeof(BST));
		tree->root = NULL;
		tree->size = 0;

		//Sets the display, compare, swapper, and free functions of a BST
		//to those of the data structure being stored in the BST
		tree->display = d;
		tree->compare = c;
		if(s == NULL){
			//If the swapper is NULL, use the default swapper
			tree->swapper = swapBSTNODE;
		}else{
			tree->swapper = s;
		}
		tree->free = f;

	return tree;
}

//Gets the root of the BST
BSTNODE *getBSTroot(BST *t){
	return t->root;
}

//Sets the root of the BST
void setBSTroot(BST *t, BSTNODE *replacement){
	t->root = replacement;
	setBSTNODEparent(t->root, t->root);
	return;
}

//Sets the size of the BST
void setBSTsize(BST *t, int s){
	t->size = s;
	return;
}

//Inserts a value into the BST
BSTNODE *insertBST(BST *t, void *value){
	//If the root is NULL, set the root to be the new value
	if(t->root == NULL){
		t->root = newBSTNODE(value);
		t->root->parent = t->root; //NEW CODE
		(t->size)++;
		return t->root;
	}else{
		//Call the BST helper function for recursion
		return insertBSThelper(t, t->root, value);
	}
	return NULL;
}

//The helper function that recurs for an insertion
BSTNODE *insertBSThelper(BST *t, BSTNODE *root, void *value){
	//Checks if the new value is less than the current node
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

//Finds the given value, returns NULL if not found
BSTNODE *findBST(BST *t, void *value){
	if(t->root == NULL){
		return NULL;
	}else if(t->compare(value, t->root->value) == 0){
		return t->root;
	}else if(t->compare(value, t->root->value) < 0){
		if(t->root->left == NULL) return NULL;
		return findBSThelper(t->compare, t->root->left, value);
	}else if(t->compare(value, t->root->value) > 0){
		if(t->root->right == NULL) return NULL;
		return findBSThelper(t->compare, t->root->right, value);
	}
	return NULL;
}

//The helper function that recurs for a find
BSTNODE *findBSThelper(int (*comp)(void *, void *), BSTNODE *root, void *value){
	if(root == NULL){
		return NULL;
	}else if(comp(value, root->value) == 0){ //Value is equal, return found value
		return root;
	}else if(comp(value, root->value) < 0){//Value less than current, go left
		if(root->left == NULL) return NULL;
		return findBSThelper(comp, root->left, value);
	}else if(comp(value, root->value) > 0){//Value greater than current, go right
		if(root->right == NULL) return NULL;
		return findBSThelper(comp, root->right, value);
	}
	return NULL;
}

//Deletes a BSTNODE
BSTNODE *deleteBST(BST *t, void *value){
	BSTNODE *del = findBST(t, value); //Checks if the value is in the tree
	if(del == NULL) return NULL;
	del = swapToLeafBST(t, del); //Swaps the found node to the leaf
	t->size--;
	pruneLeafBST(t, del); //Cuts the node from the tree
	return del;
}

//Swaps a value to a leaf
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
	}
	return node;
}

//Cuts the leaf from the BST
void pruneLeafBST(BST *t, BSTNODE *leaf){
	//if(leaf->parent == NULL){
	if(t->compare(leaf->value, t->root->value) == 0){ //NEW CODE
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

//Returns the size of the BST
int sizeBST(BST *t){
	return t->size;
}

//Prints the size, max depth, and min depth of the BST
void statisticsBST(BST *t, FILE *fp){
	fprintf(fp, "Nodes: %d\n", t->size);
	fprintf(fp, "Minimum depth: %d\n", findMinDepth(t));
	fprintf(fp, "Maximum depth: %d\n", findMaxDepth(t));
	return;
}

//Find the minimum depth of the BST, returns -1 if empty
int findMinDepth(BST *t){
	int minDepth = -1;
	if(t->root == NULL){
		return minDepth;
	}

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

//Finds the maximum depth of the BST, returns -1 if empty
int findMaxDepth(BST *t){
	int maxDepth = -1;
	if(t->root == NULL){
		return maxDepth;
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
		}

		dequeue(queue);
		enqueue(queue, NULL);
		maxDepth++;
	}
	freeQUEUE(queue);
	return maxDepth;
}

//Displays the BST
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

//Helper function for displaying the BST as a preorder traversal
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

//Displays the BST in a level order traversal
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
				fprintf(fp, " ");
			}
		}
		dequeue(queue);
		enqueue(queue, NULL);
		fprintf(fp, "\n");
	}
	freeQUEUE(queue);
	return;
}

void displayBSTdecorated(BST *b, FILE *fp){
	BSTNODE *root = b->root;

	if(root == NULL){
		fprintf(fp, "EMPTY\n");
		return;
	}
	int level = 0;
	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, root);
	enqueue(queue, NULL);

	while(peekQUEUE(queue) != NULL){
		printf("%d: ", level);
		while(peekQUEUE(queue) != NULL){
			BSTNODE *temp = dequeue(queue);

			if(temp->left == NULL && temp->right == NULL){
				fprintf(fp, "=");
			}
			b->display(temp->value, fp);

			//Prints the parent of the node being printed
			if(temp->parent == NULL){
				fprintf(fp, "(");
				b->display(temp->value, fp);
				fprintf(fp, ")");
			}else{
				fprintf(fp, "(");
				b->display(temp->parent->value, fp);
				fprintf(fp, ")");
			}

			//Indicates if the node is the root or a left or right child
			if(b->compare(temp->parent->value, temp->value) == 0){
				fprintf(fp, "X");
			}else if(temp->parent->left != NULL && 
				b->compare(temp->parent->left->value, temp->value) == 0){
				printf("L");
			}else{
				printf("R");
			}

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
		level++;
	}
	freeQUEUE(queue);
	return;
}

//Frees the nodes of the BST and their values and then frees the BST itself
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