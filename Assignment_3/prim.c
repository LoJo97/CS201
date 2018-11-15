#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "binomial.h"
#include "avl.h"
#include "queue.h"
#include "dll.h"
#include "vertex.h"
#include "edge.h"
#include "integer.h"

#define INFINITY -1

EDGE *readEdge(FILE *fp){
	if(feof(fp)) return NULL;
	int x, y;
	int w = 1;

	x = readInt(fp);
	y = readInt(fp);

	char *str = readToken(fp);
	if(strcmp(str, ";")){
		w = atoi(str);
		readToken(fp);
		stringPending(fp);
	}

	EDGE *e = newEDGE(x, y, w);
	return e;
}

void update(void *v,void *n){ //v is a vertex, n is a binomial heap node
	VERTEX *p = v;
	setVERTEXowner(p, n);
	return;
}

void displayVERTEXprim(VERTEX *v){
	displayVERTEX(v, stdout);
	printf("(");
	displayVERTEX(getVERTEXpred(v), stdout);
	printf(")");
	printf("%d", getVERTEXkey(v));
	return;
}

int compareSort(void *a, void *b){
	VERTEX *x = a;
	VERTEX *y = b;

	return getVERTEXnumber(x) - getVERTEXnumber(y);
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Incorrect number of arguments\n");
		return 0;
	}
	if(!strcmp(argv[1], "-v")){
		printf("Author: Logan Jordan\n");
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");
	if(!fp){
		printf("Failed to open file %s\n", argv[1]);
	}

	int firstInsert = 1;
	VERTEX *firstVertex;

	//Creates vertex and edge storage
	AVL *vertices = newAVL(displayVERTEX, compareVERTEX, freeVERTEX);
	AVL *edges = newAVL(displayEDGE, compareEDGE, freeVERTEX);

	//Creates the binomial heap to be used as a priority queue
	BINOMIAL *heap = newBINOMIAL(displayVERTEX, compareVERTEX, update, freeVERTEX);

	while(!feof(fp)){
		//Creates new copies of the edge
		EDGE *newEdge1 = readEdge(fp);
		EDGE *newEdge2 = newEDGE(getEDGEv2(newEdge1), getEDGEv1(newEdge1), getEDGEweight(newEdge1));
		//Checks if the edge has already been read
		EDGE *found = findAVL(edges, newEdge1);
		if(!found) found = findAVL(edges, newEdge2);

		if(!found){
			//Creates the vertices to be used
			VERTEX *ver1 = newVERTEX(getEDGEv1(newEdge1));
			VERTEX *ver2 = newVERTEX(getEDGEv2(newEdge1));
			if(compareVERTEX(ver1, ver2)){
				//Cashes the weight of the edge
				int weight = getEDGEweight(newEdge1);

				//Checks if verticies are already in the graph
				VERTEX *ver1found = findAVL(vertices, ver1);
				if(ver1found) ver1 = ver1found;
				VERTEX *ver2found = findAVL(vertices, ver2);
				if(ver2found) ver2 = ver2found;

				if(!ver1found){ //If ver1 is not found, add it to the vertex tree and heap
					insertVERTEXneighbor(ver1, ver2);
					insertVERTEXweight(ver1, weight);
					insertAVL(vertices, ver1);
					//displayVERTEX(ver1, stdout);
					//printf("\n");
					//printf("Up to binomial call\n");
					setVERTEXowner(ver1, insertBINOMIAL(heap, ver1));
					//printf("Past binomial call\n");
					if(firstInsert){
						firstVertex = ver1;
						firstInsert = 0;
					}
				}
				//printf("Past !ver1found\n");

				//Checks if vertex 2 is already in the graph
				//VERTEX *ver2found = findAVL(vertices, ver2);

				//Same protocol as above for edge 2
				if(!ver2found){
					insertVERTEXneighbor(ver2, ver1);
					insertVERTEXweight(ver2, weight);
					insertAVL(vertices, ver2);
					setVERTEXowner(ver2, insertBINOMIAL(heap, ver2));
					if(firstInsert){
						firstVertex = ver2;
						firstInsert = 0;
					}
				}

				if(ver1found){ 
					insertVERTEXneighbor(ver1, ver2);
					insertVERTEXweight(ver1, weight);
				}

				if(ver2found){ 
					insertVERTEXneighbor(ver2, ver1);
					insertVERTEXweight(ver2, weight);
				}
			}else{
				VERTEX *ver1found = findAVL(vertices, ver1);

				if(!ver1found){ //If ver1 is not found, add it to the vertex tree and heap
					insertAVL(vertices, ver1);
					setVERTEXowner(ver1, insertBINOMIAL(heap, ver1));
					if(firstInsert){
						firstVertex = ver1;
						firstInsert = 0;
					}
				}
			}
			//Inserts the edge into the edge tree
			insertAVL(edges, newEdge1);
		}
	}
	//printf("Does not seg fault in read\n");
/*
	printf("VERTICES:\n");
	displayAVL(vertices, stdout);
	printf("EDGES:\n");
	displayAVL(edges, stdout);
	printf("BINOMIAL:\n");
	displayBINOMIAL(heap, stdout);
	displayBINOMIALdebug(heap, stdout);
*/
	setVERTEXkey(firstVertex, 0);
	decreaseKeyBINOMIAL(heap, getVERTEXowner(firstVertex), firstVertex);
	//displayBINOMIAL(heap, stdout);
	//displayBINOMIALdebug(heap, stdout);

	VERTEX *v = extractBINOMIAL(heap);
	setVERTEXflag(v, 1);
	VERTEX *MST = v;
	while(sizeBINOMIAL(heap)){ //While the heap is not empty
		DLL *neighbors = getVERTEXneighbors(v); //Gets V's neighbors
		DLL *weights = getVERTEXweights(v); //Gets V's weights
		firstDLL(neighbors); //Iterators
		firstDLL(weights);
		while(moreDLL(neighbors) && moreDLL(weights)){
			VERTEX *curNeighbor = currentDLL(neighbors);
			int w = getINTEGER(currentDLL(weights));
			if(!getVERTEXflag(curNeighbor) && (getVERTEXkey(curNeighbor) == INFINITY || w < getVERTEXkey(curNeighbor))){
				setVERTEXkey(curNeighbor, w);
				decreaseKeyBINOMIAL(heap, getVERTEXowner(curNeighbor), curNeighbor);
				setVERTEXpred(curNeighbor, v);
				//insertVERTEXsuccessor(v, curNeighbor);
			}
			nextDLL(neighbors);
			nextDLL(weights);
		}
		v = extractBINOMIAL(heap);
		setVERTEXflag(v, 1);
		if(getVERTEXpred(v)) insertVERTEXsuccessor(getVERTEXpred(v), v);
	}

	//printf("Does not seg fault in creation of MST\n");

	int totalWeight = 0;
	int level = 0;
	printf("%d: ", level++);
	displayVERTEX(MST, stdout);
	printf("\n");

	QUEUE *queue = newQUEUE(NULL, NULL);

	enqueue(queue, getVERTEXsuccessors(MST));
	enqueue(queue, NULL);

	while(peekQUEUE(queue)){
		printf("%d: ", level++);
		BINOMIAL *heapsort = newBINOMIAL(NULL, compareSort, NULL, NULL);
		while(peekQUEUE(queue)){
			DLL *tempList = dequeue(queue);
			firstDLL(tempList);
			while(moreDLL(tempList)){
				VERTEX *curSuccessor = currentDLL(tempList);
				//displayVERTEXprim(curSuccessor);
				insertBINOMIAL(heapsort, curSuccessor);
				totalWeight += getVERTEXkey(curSuccessor);
				if(sizeDLL(getVERTEXsuccessors(curSuccessor)))
					enqueue(queue, getVERTEXsuccessors(curSuccessor));
				nextDLL(tempList);
			}
		}
		while(sizeBINOMIAL(heapsort)){
			VERTEX *x = extractBINOMIAL(heapsort);
			displayVERTEXprim(x);
			if(sizeBINOMIAL(heapsort)) printf(" ");
		}
		dequeue(queue);
		enqueue(queue, NULL);
		printf("\n");
	}
	freeQUEUE(queue);

	printf("weight: %d\n", totalWeight);

	return 0;
}
