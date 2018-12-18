/*
	This is an input stream. It reads input from a file and 
	parses it into tokens 
*/

#ifndef _INPUT_STREAM_H_
#define _INPUT_STREAM_H_

#include<stdio.h>
#include<ctype.h>
#include<stdbool.h>
#include<stdlib.h>


typedef enum { DIGIT, ALPHA, SPACE, END, EOT, ASSIGN, MINUS, MULTIPLY, DIVISION, 
		PLUS, EXP, BRACKET_LEFT, BRACKET_RIGHT, DOT, UNKNOWN } token_type_t ;

typedef enum { DEVICE , MEMORY } stream_type ;

typedef struct {
	
	token_type_t 	type ;
	char 	    	ch ;
	
} token_t ;


/*
	We could read from a file or from the memory.
	If input is from the memory, a valid pointer is assumed.
	Input stream should be terminated with a NULL  byte at the end.
*/
typedef union {

	FILE* device ;
	char* memory ;
	
} inp_stream ;

typedef struct {
	
	inp_stream 	stream ;
	stream_type     s_type ;
	
	char	 	current ;
	size_t  	pos ;

	token_t 	buffer ;
	_Bool 		buffer_full ;
	
} input_stream_t ;

input_stream_t* create_stream( void* stream, stream_type st ) ;

token_t read_next ( input_stream_t* input ) ;

_Bool putback( input_stream_t* input, token_t read ) ;

size_t read_and_discard_space( input_stream_t* input ) ;

void free_stream( input_stream_t* input ) ;

static token_t parse_token( char token ) ;

static token_t memory_read( input_stream_t* ptr ) ;

static token_t device_read( input_stream_t* dev ) ;

#endif

