#include <stdio.h>
#include <stdlib.h>
#include "sll.h"
#include "integer.h"

static void showSLL(SLL *items){
	printf("The items are ");
	displaySLL(items, stdout);
	printf("\n");
	printf("The items (debugged) are ");
	displaySLLdebug(items, stdout);
	printf(".\n");
}

static void fillSLLAtBack(SLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		insertSLL(items, sizeSLL(items), newINTEGER(i));
	}
	return;
}

static void fillSLLAtMid(SLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		insertSLL(items, sizeSLL(items) / 2, newINTEGER(i));
	}
	return;
}

static void fillSLLAtFront(SLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		insertSLL(items, 0, newINTEGER(i));
	}
	return;
}

static void deleteSLLAtBack(SLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *x = removeSLL(items, sizeSLL(items) - 1);
		freeINTEGER(x);
	}
	return;
}

static void deleteSLLAtMid(SLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *x = removeSLL(items, sizeSLL(items) / 2);
		freeINTEGER(x);
	}
	return;
}

static void deleteSLLAtFront(SLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *x = removeSLL(items, 0);
		freeINTEGER(x);
	}
	return;
}

int main(void){
	printf("Testing SLL:\n\n");

	printf("Creating sll\n");
	SLL *sll = newSLL(displayINTEGER, freeINTEGER);
	showSLL(sll);

	printf("Filling list with 15 values: 5 front, 5 mid, 5 back\n");
	fillSLLAtFront(sll, 5);
	fillSLLAtMid(sll, 5);
	fillSLLAtBack(sll, 5);
	showSLL(sll);

	printf("Deleting all items in list at front\n");
	deleteSLLAtFront(sll, 15);
	showSLL(sll);

	printf("Filling list with 15 values: 5 mid, 5 back, 5 front\n");
	fillSLLAtMid(sll, 5);
	fillSLLAtBack(sll, 5);
	fillSLLAtFront(sll, 5);
	showSLL(sll);

	printf("Deleting all items in list at mid\n");
	deleteSLLAtMid(sll, 15);
	showSLL(sll);

	printf("Filling list with 15 values: 5 back, 5 front, 5 mid\n");
	fillSLLAtBack(sll, 5);
	fillSLLAtFront(sll, 5);
	fillSLLAtMid(sll, 5);
	showSLL(sll);

	printf("Deleting all items in list at back\n");
	deleteSLLAtBack(sll, 15);
	showSLL(sll);

	printf("Creating donor\n");
	SLL *donor = newSLL(displayINTEGER, freeINTEGER);
	showSLL(donor);

	printf("Filling sll with 5 values at front:\n");
	fillSLLAtFront(sll, 5);
	showSLL(sll);

	printf("Filling donor with 5 values at back:\n");
	fillSLLAtBack(donor, 5);
	showSLL(donor);

	printf("Appending donor to sll:\n");
	unionSLL(sll, donor);
	printf("sll:\n");
	showSLL(sll);
	printf("donor:\n");
	showSLL(donor);
	printf("Freeing donor\n");
	freeSLL(donor);

	printf("Setting first, third, and last values to 0:\n");
	INTEGER *x = setSLL(sll, 0, newINTEGER(0));
	printf("%d from slot 0 replaced with 0\n", getINTEGER(x));
	freeINTEGER(x);
	INTEGER *y = setSLL(sll, 2, newINTEGER(0));
	printf("%d from slot 2 replaced with 0\n", getINTEGER(y));
	freeINTEGER(y);
	INTEGER *z = setSLL(sll, sizeSLL(sll), newINTEGER(0));
	printf("0 added to end of list\n");
	freeINTEGER(z);

	showSLL(sll);

	INTEGER *a = getSLL(sll, 0);
	printf("Got %d from slot 0\n", getINTEGER(a));
	INTEGER *b = getSLL(sll, 5);
	printf("Got %d from slot 5\n", getINTEGER(b));
	INTEGER *c = getSLL(sll, sizeSLL(sll) - 1);
	printf("Got %d from last slot\n", getINTEGER(c));

	printf("Freeing lists\n\n");
	freeSLL(sll);

	printf("Testing unions\n");
	SLL *r = newSLL(displayINTEGER, freeINTEGER);
	SLL *don = newSLL(displayINTEGER, freeINTEGER);

	unionSLL(r, don);
	showSLL(r);
	showSLL(don);

	fillSLLAtBack(don, 5);
	showSLL(don);
	unionSLL(r, don);
	showSLL(r);
	showSLL(don);

	freeSLL(r);
	freeSLL(don);
	return 0;
}