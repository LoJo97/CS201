#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include "integer.h"

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
	DLL *donor = newDLL(displayINTEGER, freeINTEGER);
	showDLL(donor);

	printf("Filling sll with 5 values at front:\n");
	fillDLLAtFront(dll, 5);
	showDLL(dll);

	printf("Filling donor with 5 values at back:\n");
	fillDLLAtBack(donor, 5);
	showDLL(donor);

	printf("Appending donor to dll:\n");
	unionDLL(dll, donor);
	printf("dll:\n");
	showDLL(dll);
	printf("donor:\n");
	showDLL(donor);
	printf("Freeing donor\n");
	freeDLL(donor);

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
	DLL *r = newDLL(displayINTEGER, freeINTEGER);
	DLL *don = newDLL(displayINTEGER, freeINTEGER);

	unionDLL(r, don);
	showDLL(r);
	showDLL(don);

	fillDLLAtBack(don, 5);
	showDLL(don);
	unionDLL(r, don);
	showDLL(r);
	showDLL(don);

	freeDLL(r);
	freeDLL(don);
	return 0;
}