OBJS = integer.o sll.o dll.o stack.o queue.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : test-sll test-dll test-stack test-queue

test-sll : test-sll.c sll.h integer.h integer.o
		gcc $(LOPTS) test-sll.c sll.c integer.o  -o test-sll

test-dll : test-dll.c sll.h integer.h integer.o
		gcc $(LOPTS) test-dll.c dll.c integer.o -o test-dll

test-stack : test-stack.c stack.h dll.h integer.h integer.o
		gcc $(LOPTS) test-stack.c stack.c dll.c integer.o -o test-stack

test-queue : test-queue.c queue.h sll.h integer.h integer.o
		gcc $(LOPTS) test-queue.c queue.c sll.c integer.o -o test-queue

stack.o : stack.c stack.h sll.h
		gcc $(OOPTS) stack.c

queue.o : queue.c queue.h dll.h
		gcc $(OOPTS) queue.c

sll.o : sll.c sll.h
		gcc $(OOPTS) sll.c

dll.o : dll.c dll.h
		gcc $(OOPTS) dll.c

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

valgrind  : all
		echo testing singly-linked list
		valgrind ./test-sll
		echo
		echo testing doubly-linked list
		valgrind ./test-dll
		echo
		echo testing stack
		valgrind ./test-stack
		echo
		echo testing queue
		valgrind ./test-queue
		echo

test : all
		echo testing singly-linked list
		./test-sll
		echo
		echo testing doubly-linked list
		./test-dll
		echo
		echo testing stack
		./test-stack
		echo
		echo testing queue
		./test-queue
		echo

clean    :
		rm -f $(OBJS) test-*.o test-stack test-queue test-sll test-dll