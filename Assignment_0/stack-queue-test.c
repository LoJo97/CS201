#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"
#include "integer.h"

static void showQUEUE(QUEUE *items){
	printf("The items are ");
	displayQUEUE(items, stdout);
	printf("\n");
	printf("The items (debugged) are ");
	displayQUEUEdebug(items, stdout);
	printf(".\n");
}

static void fillQUEUE(QUEUE *items, int num){
	int i;
	for(i = 0; i < num; i++){
		enqueue(items, newINTEGER(i));
	}
	return;
}

static void removeQUEUE(QUEUE *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *i = dequeue(items);
		printf("%d removed\n", getINTEGER(i));
		freeINTEGER(i);
	}
	return;
}

static void showSTACK(STACK *items){
	printf("The items are ");
	displaySTACK(items, stdout);
	printf("\n");
	printf("The items (debugged) are ");
	displaySTACKdebug(items, stdout);
	printf(".\n");
}

static void fillSTACK(STACK *items, int num){
	int i;
	for(i = 0; i < num; i++){
		push(items, newINTEGER(i));
	}
	return;
}

static void removeSTACK(STACK *items, int num){
	int i;
	for(i = 0; i < num; i++){
		INTEGER *i = pop(items);
		printf("%d removed\n", getINTEGER(i));
		freeINTEGER(i);
	}
	return;
}

int main(){
	printf("Testing Queue\n\n");

	printf("Creating queue\n");
	QUEUE *q = newQUEUE(displayINTEGER, freeINTEGER);
	showQUEUE(q);

	printf("Filling queue with 10 items\n");
	fillQUEUE(q, 10);
	showQUEUE(q);

	printf("Popping the values from the queue\n");
	removeQUEUE(q, 10);
	showQUEUE(q);

	printf("Filling the queue with 5 items\n");
	fillQUEUE(q, 5);
	showQUEUE(q);

	INTEGER *a = peekQUEUE(q);
	printf("The item at the end of the queue is %d\n", getINTEGER(a));

	freeQUEUE(q);

	printf("Testing Stack\n\n");

	printf("Creating stack\n");
	STACK *s = newSTACK(displayINTEGER, freeINTEGER);
	showSTACK(s);

	printf("Filling stack with 10 items\n");
	fillSTACK(s, 10);
	showSTACK(s);

	printf("Popping the values from the stack\n");
	removeSTACK(s, 10);
	showSTACK(s);

	printf("Filling the stack with 5 items\n");
	fillSTACK(s, 5);
	showSTACK(s);

	INTEGER *b = peekSTACK(s);
	printf("The item at the end of the stack is %d\n", getINTEGER(b));

	freeSTACK(s);
}