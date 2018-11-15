OBJS = integer.o sll.o dll.o queue.o stack.o bst.o heap.o gst.o real.o string.o gst.o avl.o scanner.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : test-gst test-avl

trees : trees.c queue.o queue.h sll.h sll.o string.h string.o bst.h bst.o avl.h avl.o gst.h gst.o scanner.h scanner.o
		gcc $(LOPTS) trees.c avl.o gst.o bst.o queue.o sll.o string.o scanner.o -o trees

test-gst : gst-0-0.c queue.o queue.h sll.h sll.o integer.h integer.o bst.h bst.o gst.h gst.o
		gcc $(LOPTS) gst-0-0.c gst.c bst.o queue.o sll.o integer.o -o test-gst

test-avl : avl-0-0.c queue.o queue.h sll.h sll.o integer.h integer.o real.h real.o string.h string.o bst.h bst.o
		gcc $(LOPTS) avl-0-0.c avl.c bst.o queue.o sll.o integer.o real.o string.o -o test-avl

avl.o : avl.c avl.h bst.h queue.h
		gcc $(OOPTS) avl.c

gst.o : gst.c gst.h bst.h queue.h
		gcc $(OOPTS) gst.c

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
		echo testing gst
		valgrind ./test-gst
		echo
		echo testing avl
		valgrind ./test-avl
		echo

test : all
		echo testing gst
		./test-gst
		echo
		echo testing avl
		./test-avl
		echo
		echo testing trees
		./trees -g t-0-2.data t-0-2.more
		echo

clean    :
		rm -f $(OBJS) test-*.o test-gst test-avl trees.o trees