#include "calc_error.h"

calc_error* init_error(void) 
{
	c_error = (calc_error*)malloc(sizeof(calc_error)) ;
	return c_error ;
}

void set_error( int err, char* err_msg, input_stream_t* stream ) 
{
	c_error->error_number = err ;
	c_error->error_offset = stream->pos ;
	c_error->error_line = 1 ;
	c_error->error_level = get_error_level(err) ;
	sprintf(c_error->error_msg, "%ld:%s:%s\n", stream->pos, error_msg(err), err_msg) ; 
}


void clear_error(void) 
{
	c_error->error_number = 0 ;
	c_error->error_offset = 0 ;
	c_error->error_level = DEFAULT ;
	memset(c_error->error_msg, 0, sizeof(c_error->error_msg)) ;
}

const char* error_msg( int error_code) 
{
	switch(error_code){

	case 0: return "" ;
	case OVERFLOW : return "Overflow" ;
	case UNDERFLOW: return "Underflow" ;
	case UNMATCHED_PARENTHESIS: return "Unmatched parenthesis" ;
	case UNCLOSED_PARENTHESIS : return "Unclosed parenthesis" ;
	case INVALID_OPERAND : return "Invalid operand" ;
	case EOL_WHILE_PARSING : return "EOL while processing" ;
	case DIVISION_BY_ZERO: return "Division by zero" ;
	case UNDEFINED_VARIABLE: return "Undefined Variable";
	default: return "Unknow error";

	}
}


void print_message ( calc_error* err, FILE* file ) 
{
	fprintf(file, "%s", err->error_msg) ;
}

_Noreturn void die_with_message( void ) 
{
	print_message(c_error, stdout) ;
	exit(c_error->error_number) ;
}

error_level get_error_level(int err_code) 
{
	switch( err_code)
	{
	case OVERFLOW: 
	case UNDERFLOW:
		return WARNING ;
	
	default: return CRITICAL ;	
	}
}
