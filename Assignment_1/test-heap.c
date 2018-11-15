#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "integer.h"

void repeatInsert(HEAP *h, int num){
	int i;
	for(i = num; i > 0; i--){
		insertHEAP(h, newINTEGER(i));
	}
	return;
}

int main(void){
	HEAP *heap = newHEAP(displayINTEGER, rcompareINTEGER, freeINTEGER);

	repeatInsert(heap, 100);

	printf("Root: ");
	displayINTEGER(peekHEAP(heap), stdout);
	printf("\n");

	displayHEAP(heap, stdout);
	displayHEAPdebug(heap, stdout);

	printf("Building heap:\n");
	buildHEAP(heap);

	displayHEAP(heap, stdout);
	displayHEAPdebug(heap, stdout);
	
	int i;
	for(i = 0; i < 10; i++){
		printf("Min: ");
		INTEGER *x = extractHEAP(heap);
		displayINTEGER(x, stdout);
		freeINTEGER(x);
		printf("\n");
	}
	
	displayHEAPdebug(heap, stdout);

	freeHEAP(heap);

	return 0;
}