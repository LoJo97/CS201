#include <stdio.h>
#include <stdlib.h>
#include "sll.h"
#include "dll.h"
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

static void showDLL(DLL *items){
	printf("The items are ");
	displayDLL(items, stdout);
	printf("\n");
	printf("The items (debugged) are ");
	displayDLLdebug(items, stdout);
	printf(".\n");
}

static void fillDLLAtBack(DLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		insertDLL(items, sizeDLL(items), newINTEGER(i));
	}
	return;
}

static void fillDLLAtMid(DLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		insertDLL(items, sizeDLL(items) / 2, newINTEGER(i));
	}
	return;
}

static void fillDLLAtFront(DLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		insertDLL(items, 0, newINTEGER(i));
	}
	return;
}

static void deleteDLLAtBack(DLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *x = removeDLL(items, sizeDLL(items) - 1);
		freeINTEGER(x);
	}
	return;
}

static void deleteDLLAtMid(DLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *x = removeDLL(items, sizeDLL(items) / 2);
		freeINTEGER(x);
	}
	return;
}

static void deleteDLLAtFront(DLL *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *x = removeDLL(items, 0);
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

	printf("Testing DLL:\n\n");

	printf("Creating dll\n");
	DLL *dll = newDLL(displayINTEGER, freeINTEGER);
	showDLL(dll);

	printf("Filling list with 15 values: 5 front, 5 mid, 5 back\n");
	fillDLLAtFront(dll, 5);
	fillDLLAtMid(dll, 5);
	fillDLLAtBack(dll, 5);
	showDLL(dll);

	printf("Deleting all items in list at front\n");
	deleteDLLAtFront(dll, 15);
	showDLL(dll);

	printf("Filling list with 15 values: 5 mid, 5 back, 5 front\n");
	fillDLLAtMid(dll, 5);
	fillDLLAtBack(dll, 5);
	fillDLLAtFront(dll, 5);
	showDLL(dll);

	printf("Deleting all items in list at mid\n");
	deleteDLLAtMid(dll, 15);
	showDLL(dll);

	printf("Filling list with 15 values: 5 back, 5 front, 5 mid\n");
	fillDLLAtBack(dll, 5);
	fillDLLAtFront(dll, 5);
	fillDLLAtMid(dll, 5);
	showDLL(dll);

	printf("Deleting all items in list at back\n");
	deleteDLLAtBack(dll, 15);
	showDLL(dll);

	printf("Creating donor2\n");
	DLL *donor2 = newDLL(displayINTEGER, freeINTEGER);
	showDLL(donor2);

	printf("Filling sll with 5 values at front:\n");
	fillDLLAtFront(dll, 5);
	showDLL(dll);

	printf("Filling donor2 with 5 values at back:\n");
	fillDLLAtBack(donor2, 5);
	showDLL(donor2);

	printf("Appending donor2 to dll:\n");
	unionDLL(dll, donor2);
	printf("dll:\n");
	showDLL(dll);
	printf("donor2:\n");
	showDLL(donor2);
	printf("Freeing donor2\n");
	freeDLL(donor2);

	printf("Setting first, third, and last values to 0:\n");
	INTEGER *d = setDLL(dll, 0, newINTEGER(0));
	printf("%d from slot 0 replaced with 0\n", getINTEGER(d));
	freeINTEGER(d);
	INTEGER *e = setDLL(dll, 2, newINTEGER(0));
	printf("%d from slot 2 replaced with 0\n", getINTEGER(e));
	freeINTEGER(e);
	INTEGER *f = setDLL(dll, sizeDLL(dll), newINTEGER(0));
	printf("0 added to end of list\n");
	freeINTEGER(f);

	showDLL(dll);

	INTEGER *g = getDLL(dll, 0);
	printf("Got %d from slot 0\n", getINTEGER(g));
	INTEGER *h = getDLL(dll, 5);
	printf("Got %d from slot 5\n", getINTEGER(h));
	INTEGER *i = getDLL(dll, sizeDLL(dll) - 1);
	printf("Got %d from last slot\n", getINTEGER(i));

	printf("Freeing lists\n");
	freeDLL(dll);

	printf("Testing unions\n");
	DLL *r2 = newDLL(displayINTEGER, freeINTEGER);
	DLL *don2 = newDLL(displayINTEGER, freeINTEGER);

	unionDLL(r2, don2);
	showDLL(r2);
	showDLL(don2);

	fillDLLAtBack(don2, 5);
	showDLL(don2);
	unionDLL(r2, don2);
	showDLL(r2);
	showDLL(don2);

	freeDLL(r2);
	freeDLL(don2);
	
	return 0;
}