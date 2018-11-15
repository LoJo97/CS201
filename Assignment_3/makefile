OBJS = integer.o sll.o dll.o queue.o stack.o bst.o heap.o gst.o real.o string.o avl.o scanner.o binomial.o vertex.o edge.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : test-binomial prim

prim : prim.c vertex.o vertex.h edge.o edge.h binomial.o binomial.h avl.o avl.h bst.o bst.h queue.o queue.h sll.o sll.h dll.o dll.h integer.o integer.h scanner.o scanner.h
		gcc $(LOPTS) prim.c vertex.o edge.o binomial.o avl.o bst.o queue.o sll.o dll.o integer.o scanner.o -o prim -lm

test-binomial : binomial-0-9.c binomial.o binomial.h queue.o queue.h sll.o sll.h dll.o dll.h integer.o integer.h real.o real.h string.o string.h
		gcc $(LOPTS) binomial-0-9.c binomial.o queue.o sll.o dll.o integer.o real.o string.o -o test-binomial -lm

test-avl : avl-0-8.c queue.o queue.h sll.h sll.o integer.h integer.o real.h real.o string.h string.o bst.h bst.o
		gcc $(LOPTS) avl-0-8.c avl.c bst.o queue.o sll.o integer.o real.o string.o -o test-avl

binomial.o :  binomial.c binomial.h queue.h dll.h
		gcc $(OOPTS) binomial.c

avl.o : avl.c avl.h bst.h queue.h
		gcc $(OOPTS) avl.c

bst.o : bst.c bst.h queue.h
		gcc $(OOPTS) bst.c

queue.o : queue.c queue.h sll.h
		gcc $(OOPTS) queue.c

stack.o : stack.c stack.h dll.h
		gcc $(OOPTS) stack.c

sll.o : sll.c sll.h
		gcc $(OOPTS) sll.c

dll.o : dll.c dll.h
		gcc $(OOPTS) dll.c

vertex.o : vertex.c vertex.h integer.h dll.h
		gcc $(OOPTS) vertex.c

edge.o : edge.c edge.h
		gcc $(OOPTS) edge.c

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

real.o : real.c real.h
		gcc $(OOPTS) real.c

string.o : string.c string.h
		gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) scanner.c

valgrind  : all
		echo testing binomial
		valgrind ./test-binomial
		echo

test : all
		echo testing binomial
		./test-binomial
		echo
		echo testing prim
		./prim prim.data
		echo

clean    :
		rm -f $(OBJS) test-*.o prim.o prim test-binomial