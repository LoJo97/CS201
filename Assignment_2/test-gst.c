#include <stdio.h>
#include <stdlib.h>
#include "gst.h"
#include "integer.h"

int main(void){
	printf("Testing GST:\n\n");

	GST *gst = newGST(displayINTEGER, compareINTEGER, freeINTEGER);
	displayGST(gst, stdout);
	printf("\n");

	printf("Size: %d\n", sizeGST(gst));

	insertGST(gst, newINTEGER(0));
	displayGST(gst, stdout);
	printf("\n");

	printf("Size: %d\n", sizeGST(gst));

	insertGST(gst, newINTEGER(-5));
	displayGST(gst, stdout);
	printf("\n");

	insertGST(gst, newINTEGER(5));
	displayGST(gst, stdout);
	printf("\n");

	insertGST(gst, newINTEGER(-2));
	displayGST(gst, stdout);
	printf("\n");

	insertGST(gst, newINTEGER(2));
	displayGST(gst, stdout);
	printf("\n");

	insertGST(gst, newINTEGER(10));
	displayGST(gst, stdout);
	printf("\n");

	insertGST(gst, newINTEGER(10));
	displayGST(gst, stdout);
	printf("\n");

	insertGST(gst, newINTEGER(3));
	displayGST(gst, stdout);
	printf("\n");

	displayGSTdebug(gst, stdout);
	printf("\n");

	statisticsGST(gst, stdout);

	printf("Freeing gst\n");
	freeGST(gst);

	return 0;
}