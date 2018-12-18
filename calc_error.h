#ifndef _CALC_ERROR_H_
#define _CALC_ERROR_H_

#include <errno.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#include "input_stream.h"


#define OVERFLOW	1 
#define UNDERFLOW 	2
#define DIVISION_BY_ZERO 3
#define UNMATCHED_PARENTHESIS 4
#define UNCLOSED_PARENTHESIS 5
#define INVALID_OPERAND 6
#define EOL_WHILE_PARSING 7
#define UNDEFINED_VARIABLE 8 

typedef enum { CRITICAL, WARNING, IGNORE, DEFAULT } error_level ;

typedef struct{
	
	error_level error_level ;
	int error_number ;
	int error_line ;
	int error_offset ;
	char error_msg[512] ;

} calc_error ;

extern calc_error* c_error ;

calc_error* init_error(void) ;

void set_error( int err, char* error_msg, input_stream_t* stream ) ;

void clear_error(void) ;

const char* error_msg( int ) ;

void print_message ( calc_error*, FILE* file ) ;

error_level get_error_level(int err_code) ;

_Noreturn void die_with_message( void ) ;


#endif

