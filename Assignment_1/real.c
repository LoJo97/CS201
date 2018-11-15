#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "real.h"

struct REAL
    {
    double value;
    };

REAL *
newREAL(double x)
    {
    REAL *p = malloc(sizeof(REAL));
    assert(p != 0);
    p->value = x;
    return p;
    }

double 
getREAL(REAL *v)
    {
    return v->value;
    }

double 
setREAL(REAL *v,double x)
    {
    double old = v->value;
    v->value = x;
    return old;
    }

void 
displayREAL(void *v,FILE *fp)
    {
    fprintf(fp,"%g",getREAL((REAL *) v));
    }

int 
compareREAL(void *v,void *w)
    {
    return (int)getREAL(v) - (int)getREAL(w);
    }

int 
rcompareREAL(void *v,void *w)
    {
    return (int)getREAL(w) - (int)getREAL(v);
    }

void
freeREAL(void *v)
    {
    free((REAL *) v);
    }
