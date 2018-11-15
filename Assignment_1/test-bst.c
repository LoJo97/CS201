#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "integer.h"

int main(void){
	printf("Testing BST:\n\n");

	BST *bst = newBST(displayINTEGER, compareINTEGER, NULL, freeINTEGER);
	displayBST(bst, stdout);
	printf("\n");

	printf("Size: %d\n", sizeBST(bst));

	insertBST(bst, newINTEGER(0));
	displayBST(bst, stdout);
	printf("\n");

	printf("Size: %d\n", sizeBST(bst));

	insertBST(bst, newINTEGER(-5));
	displayBST(bst, stdout);
	printf("\n");

	insertBST(bst, newINTEGER(5));
	displayBST(bst, stdout);\
	printf("\n");

	insertBST(bst, newINTEGER(-2));
	displayBST(bst, stdout);
	printf("\n");

	insertBST(bst, newINTEGER(2));
	displayBST(bst, stdout);
	printf("\n");

	insertBST(bst, newINTEGER(-10));
	displayBST(bst, stdout);
	printf("\n");

	insertBST(bst, newINTEGER(10));
	displayBST(bst, stdout);
	printf("\n");

	insertBST(bst, newINTEGER(3));
	displayBST(bst, stdout);
	printf("\n");

	displayBSTdebug(bst, stdout);

	statisticsBST(bst, stdout);

	INTEGER *x = newINTEGER(0);
	BSTNODE *del = deleteBST(bst, x);
	freeBSTNODE(del, freeINTEGER);

	setINTEGER(x, -5);

	del = deleteBST(bst, x);

	freeBSTNODE(del, freeINTEGER);

	displayBST(bst, stdout);
	displayBSTdebug(bst, stdout);

	printf("Freeing bst\n");
	freeINTEGER(x);
	freeBST(bst);

	return 0;
}