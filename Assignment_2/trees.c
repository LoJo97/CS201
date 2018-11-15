#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "string.h"
#include "avl.h"
#include "gst.h"

/* options */
int avl = 1;    /* option -r */
int gst = 0;    /* option -g */
int v   = 0;    /* option -v */

static int processOptions(int,char **);
void Fatal(char *,...);
void cleanString(char* s);

int main(int argc,char **argv){
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    if(v == 1){
        printf("Logan Jordan\n");
        return 0;
    }

    if(avl == 1){
        AVL *avl;
        avl = newAVL(displaySTRING, compareSTRING, freeSTRING);

        FILE *corpus = fopen(argv[argIndex++], "r");

        char *x;
        if(stringPending(corpus)){
            if(!feof(corpus)) x = readString(corpus);
        }else{
            if(!feof(corpus)) x = readToken(corpus);
        }

        while(!feof(corpus)){
            cleanString(x);
            if(strcmp(x, "") != 0) insertAVL(avl, newSTRING(x));
            if(stringPending(corpus)){
                if(!feof(corpus)) x = readString(corpus);
            }else{
                if(!feof(corpus)) x = readToken(corpus);
            }
        }

        fclose(corpus);

        FILE *commands = fopen(argv[argIndex], "r");

        if(stringPending(commands)){
            if(!feof(commands)) x = readString(commands);
        }else{
            if(!feof(commands)) x = readToken(commands);
        }

        while(!feof(commands)){
            cleanString(x);
            if(strcmp(x, "i") == 0){
                if(stringPending(commands)){
                    if(!feof(commands)) x = readString(commands);
                }else{
                    if(!feof(commands)) x = readToken(commands);
                }
                cleanString(x);
                if(strcmp(x, "") != 0) insertAVL(avl, newSTRING(x));
            }else if(strcmp(x, "d") == 0){
                if(stringPending(commands)){
                    if(!feof(commands)) x = readString(commands);
                }else{
                    if(!feof(commands)) x = readToken(commands);
                }
                cleanString(x);
                if(strcmp(x, "") != 0){
                    if(findAVL(avl, newSTRING(x))) deleteAVL(avl, newSTRING(x));
                    else printf("Value %s not found.\n", x);
                }
            }
            else if(strcmp(x, "f") == 0){
                if(stringPending(commands)){
                    if(!feof(commands)) x = readString(commands);
                }else{
                    if(!feof(commands)) x = readToken(commands);
                }
                cleanString(x);
                if(strcmp(x, "") != 0) printf("Frequency of %s: %d\n", x, findAVLcount(avl, newSTRING(x)));
            }else if(strcmp(x, "s") == 0){
                displayAVL(avl, stdout);
            }else if(strcmp(x, "r") == 0){
                statisticsAVL(avl, stdout);
            }

            if(stringPending(commands)){
                if(!feof(commands)) x = readString(commands);
            }else{
                if(!feof(commands)) x = readToken(commands);
            }
        }

        fclose(commands);

        return 0;
    }


    if(gst == 1){
        GST *gst;
        gst = newGST(displaySTRING, compareSTRING, freeSTRING);

        FILE *corpus = fopen(argv[argIndex++], "r");

        char *x;
        if(stringPending(corpus)){
            if(!feof(corpus)) x = readString(corpus);
        }else{
            if(!feof(corpus)) x = readToken(corpus);
        }

        while(!feof(corpus)){
            cleanString(x);
            if(strcmp(x, "") != 0) insertGST(gst, newSTRING(x));
            if(stringPending(corpus)){
                if(!feof(corpus)) x = readString(corpus);
            }else{
                if(!feof(corpus)) x = readToken(corpus);
            }
        }

        fclose(corpus);
        FILE *commands = fopen(argv[argIndex], "r");
        if(stringPending(commands)){
            if(!feof(commands)) x = readString(commands);
        }else{
            if(!feof(commands)) x = readToken(commands);
        }

        while(!feof(commands)){
            cleanString(x);
            if(strcmp(x, "i") == 0){
                if(stringPending(commands)){
                    if(!feof(commands)) x = readString(commands);
                }else{
                    if(!feof(commands)) x = readToken(commands);
                }
                cleanString(x);
                if(strcmp(x, "") != 0) insertGST(gst, newSTRING(x));
            }else if(strcmp(x, "d") == 0){
                if(stringPending(commands)){
                    if(!feof(commands)) x = readString(commands);
                }else{
                    if(!feof(commands)) x = readToken(commands);
                }
                cleanString(x);
                if(strcmp(x, "") != 0){
                    if(findGST(gst, newSTRING(x))) deleteGST(gst, newSTRING(x));
                    else printf("Value %s not found.\n", x);
                }
            }
            else if(strcmp(x, "f") == 0){
                if(stringPending(commands)){
                    if(!feof(commands)) x = readString(commands);
                }else{
                    if(!feof(commands)) x = readToken(commands);
                }
                cleanString(x);
                if(strcmp(x, "") != 0) printf("Frequency of %s: %d\n", x, findGSTcount(gst, newSTRING(x)));
            }else if(strcmp(x, "s") == 0){
                displayGST(gst, stdout);
            }else if(strcmp(x, "r") == 0){
                statisticsGST(gst, stdout);
            }

            if(stringPending(commands)){
                if(!feof(commands)) x = readString(commands);
            }else{
                if(!feof(commands)) x = readToken(commands);
            }
        }

        fclose(commands);

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

            case 'r':
                avl = 1;
                gst = 0;
                v = 0;
                break;
            case 'g':
                gst = 1;
                avl = 0;
                v = 0;
                break;
            case 'v':
                v = 1;
                avl = 0;
                gst = 0;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }

        if (separateArg && argUsed) ++argIndex;

        ++argIndex;
    }

    return argIndex;
}

void cleanString(char* s){
    int strLen = strlen(s);
    int i = 0, j = 0;
    char newString[strLen];
    int isSpace = 1;
    int leadingSpace = 1;
    while(i < strLen){
        if(isSpace){
            if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')){
                if(leadingSpace == 1){
                    leadingSpace = 0;
                }else{
                    newString[j++] = ' ';
                }
                newString[j] = tolower(s[i]);
                j++;
                isSpace = 0;
            }
        }else{
            if(s[i] == ' ') isSpace = 1;
            else if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')){
                newString[j] = tolower(s[i]);
                j++;
            }
        }
        i++;
    }
    newString[j] = '\0';
    strcpy(s, newString);
    return;
}