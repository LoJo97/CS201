# CS201
Projects for Data Structures and Algorithms

These are projects for CS201 - Data Structures and Algorithms with Dr. Lusth from Spring 2018. Below are descriptions of each
project and how to use them.

VERSATILITY:
The data structures from this class can take basically any type of value as the value of a node. This is accomplished through 
the use of wrapper classes such as integer.c, real.c, string.c, etc. (all given to us by Dr. Lusth so that we could be sure to 
comply to his standards).

TESTING:
Makefiles are included in each project folder that show how to compile the modules together (these projects build on each other
so later makefiles get quite long). To run predefined tests on each module, use the command line to 'make', then type 'make run'.

MEMORY MANAGEMENT:
Another requirement was for each module (except for certain executables such as prim.c) to be able to be used without memory leaks.
In order to do this, the memory leak checker Valgrind was used (link: http://valgrind.org/). To run valgrind tests on the modules,
use 'make valgrind' on the command line.

Assignment 0:
This set of modules implements singly and doubly linked lists, as well as a stack and a queue built off of these lists. All of
these structures have the typical functions to add, delete, get, and set values, as well as display the entire list.

Assignment 1:
This assignment builds off the last and implements a binary search tree and a heap. Heap is then used to construct a heapsort
executeable. How the heap is built is based off of the comparator function passed in. For example, INTEGER's regular compare()
function will construct a min-heap, whereas rcompare() will construct a max-heap.

Assignment 2:
Here we take the basic BST module from the last assignment and modify it to create self balancing trees, an AVL and a GST (green
binary search tree).
