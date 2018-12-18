// primary reads a primary figure and parses it 

#ifndef _PRIM_H_
#define _PRIM_H_

#include<float.h>
#include<math.h>
#include "calc_error.h"
#include "input_stream.h"
#include "dict.h"

#define NAME_MAX 512 

typedef long double calc_t ;

extern calc_t expression(void) ;

calc_t number(void) ;

char* name(void) ;

calc_t primary(void) ;

#endif

