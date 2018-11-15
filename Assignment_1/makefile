OBJS = integer.o sll.o dll.o queue.o stack.o bst.o heap.o scanner.o real.o string.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : test-bst test-heap heapsort

heapsort : heapsort.c queue.c queue.h stack.c stack.h sll.h sll.o dll.h dll.o integer.h integer.o real.o real.h string.o string.h bst.h bst.o heap.h heap.o scanner.h scanner.o
		gcc $(LOPTS) heapsort.c heap.c bst.c queue.c stack.c sll.c dll.c scanner.c integer.o real.o string.o -o heapsort

test-bst : test-bst.c queue.o queue.h sll.h sll.o integer.h integer.o bst.h bst.o
		gcc $(LOPTS) test-bst.c bst.c queue.c sll.c integer.o -o test-bst

test-heap : test-heap.c queue.c queue.h stack.c stack.h sll.h sll.o dll.h dll.o integer.h integer.o bst.h bst.o heap.h heap.o
		gcc $(LOPTS) test-heap.c heap.c bst.c queue.c stack.c sll.c dll.c integer.o -o test-heap

heap.o : heap.c heap.h queue.h
		gcc $(OOPTS) heap.c

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

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

real.o : real.c real.h
		gcc $(OOPTS) real.c

string.o : string.c string.h
		gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) scanner.c

valgrind  : all
		echo testing bst
		valgrind ./test-bst
		echo
		echo testing heap
		valgrind ./test-heap
		echo

test : all
		echo testing bst
		./test-bst
		echo
		echo testing heap
		./test-heap
		echo

clean    :
		rm -f $(OBJS) test-*.o test-heap test-bst heapsort