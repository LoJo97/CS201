#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
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

int main(void){
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
	return 0;
}