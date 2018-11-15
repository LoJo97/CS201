#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "heap.h"
#include "scanner.h"
#include "integer.h"
#include "real.h"
#include "string.h"

/* options */
int view    = 0;    /* option -v */
int integer = 1;    /* option -i */
int real    = 0;    /* option -r */
int string  = 0;    /* option -s */
int inc     = 1;    /* option -I */
int dec     = 0;    /* option -D */

static int processOptions(int,char **);
void Fatal(char *,...);

int main(int argc,char **argv){
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    if(view == 1){
        printf("Written by Logan Jordan\n");
        printf("My heapsort works in n(log n) time because buildHeap runs\n");
        printf("heapify (a log n operation) n times, yielding a n(log n)\n");
        printf("time for buildHeap. ExtractHEAP, which will be run n times\n");
        printf("runs heapify onces each time, yeilding another n(log n)\n");
        printf("runtime. Thus, heapsort has a θ(2n(log n)) runtime, which\n");
        printf("is equivalent to θ(n(log n)).\n");
        return 0;
    }

    if(integer == 1){
        HEAP *heap;
        if(dec == 1){
            heap = newHEAP(displayINTEGER, rcompareINTEGER, freeINTEGER);
        }else{
            heap = newHEAP(displayINTEGER, compareINTEGER, freeINTEGER);
        }

        FILE *fp = fopen(argv[argIndex], "r");
        
        int x = readInt(fp);

        while(!feof(fp)){
            insertHEAP(heap, newINTEGER(x));
            x = readInt(fp);
        }

        buildHEAP(heap);
        
        INTEGER *y;
        while(sizeHEAP(heap) != 0){
            y = extractHEAP(heap);
            printf("%d", getINTEGER(y));
            if(sizeHEAP(heap) != 0){
                printf(" ");
            }
            freeINTEGER(y);
        }
        printf("\n");
        freeHEAP(heap);
        fclose(fp);
        return 0;
    }

    if(real == 1){
        HEAP *heap;
        if(dec == 1){
            heap = newHEAP(displayREAL, rcompareREAL, freeREAL);
        }else{
            heap = newHEAP(displayREAL, compareREAL, freeREAL);
        }

        FILE *fp = fopen(argv[argIndex], "r");
        
        double x = readReal(fp);

        while(!feof(fp)){
            insertHEAP(heap, newREAL(x));
            x = readReal(fp);
        }

        buildHEAP(heap);
        
        REAL *y;
        while(sizeHEAP(heap) != 0){
            y = extractHEAP(heap);
            printf("%f", getREAL(y));
            if(sizeHEAP(heap) != 0){
                printf(" ");
            }
            freeREAL(y);
        }
        freeHEAP(heap);
        printf("\n");
        fclose(fp);
        return 0;
    }

    if(string == 1){
        HEAP *heap;
        if(dec == 1){
            heap = newHEAP(displaySTRING, rcompareSTRING, freeSTRING);
        }else{
            heap = newHEAP(displaySTRING, compareSTRING, freeSTRING);
        }

        FILE *fp = fopen(argv[argIndex], "r");

        char *x;
        if(stringPending(fp)){
            if(!feof(fp)) x = readString(fp);
        }else{
            if(!feof(fp)) x = readToken(fp);
        }

        while(!feof(fp)){
            insertHEAP(heap, newSTRING(x));
            if(stringPending(fp)){
                if(!feof(fp)) x = readString(fp);
            }else{
                if(!feof(fp)) x = readToken(fp);
            }
        }

        buildHEAP(heap);
        
        STRING *y;
        while(sizeHEAP(heap) != 0){
            y = extractHEAP(heap);
            printf("%s", getSTRING(y));
            if(sizeHEAP(heap) != 0){
                printf(" ");
            }
            freeSTRING(y);
        }
        printf("\n");
        freeHEAP(heap);
        fclose(fp);
        return 0;
    }
    return 0;
}

void
Fatal(char *fmt, ...)
    {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
    }

/* only -oXXX  or -o XXX options */

static int
processOptions(int argc, char **argv)
    {
    int argIndex;
    int argUsed;
    int separateArg;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-'){
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;
            

        switch (argv[argIndex][1]){
            /*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argUsed = 1;
             *         break;
             *
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

            case 'v':
                view = 1;
                break;
            case 'D':
                dec = 1;
                inc = 0;
                break;
            case 'I':
                break;
            case 'i':
                break;
            case 'r':
                integer = 0;
                real = 1;
                break;
            case 's':
                integer = 0;
                string = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }

        if (separateArg && argUsed) ++argIndex;

        ++argIndex;
    }

    return argIndex;
    }
