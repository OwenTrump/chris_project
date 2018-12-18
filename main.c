#include "term.h"

input_stream_t* stream = NULL ;
calc_error* c_error = NULL ;
dict_t* symbol_table = NULL ;

static inline void DieWithError( const char* msg )
{
	fprintf(stderr, "%s\n", msg) ; exit(-1) ;
}



const char* str_token(token_t t )
{
	switch(t.type){
	
	case DIGIT: return "DIGIT" ;
	case SPACE: return "SPACE" ;
	case ALPHA: return "ALPHA" ;
	case END: return "END"; 
	case EOT:  return "EOT" ;
	case ASSIGN: return "ASSIGN" ;
	case MINUS: return "MINUS" ;
	case MULTIPLY: return "DIVISION";
	case PLUS: return "PLUS" ;
	case EXP: return "EXP" ;
	case BRACKET_LEFT: return "BRACKET_LEFT" ;
	case BRACKET_RIGHT: return "BRACKET_RIGHT" ;
	case DOT: return "DOT" ;
	case UNKNOWN: return "UNKNOWN";
	default: return "_" ;
	}
}

void print_token( token_t t )
{ printf("{ %c : %s} \n", t.ch, str_token(t)); }
 
size_t val_size ( const void* v )
{
	return sizeof(*((calc_t*)v)) ;
}

size_t key_size ( const void* k )
{
	return strlen(k) + 1 ;
}

int key_cmp( const void* one , const void* two )
{
	return strcmp(one, two) ;
}

int val_cmp( const void* one , const void* two )
{
	const calc_t* o = (calc_t*)one ;
	const calc_t* t = (calc_t*)two ;

	return ( *o == *t ) ? 0 : (*o < *t ) ? -1 : 1 ;
}


int main()
{	
	stream = create_stream( stdin, DEVICE) ;
	c_error = init_error() ;
	symbol_table = new_dict(20, key_cmp, val_cmp, key_size, val_size) ;
	
	calc_t pi = 3.142, e = 0.0023 ;

	d_add_entry(symbol_table, "pi" , &pi) ;
	d_add_entry(symbol_table, "e", &e) ;
	token_t t ;
	calc_t num ;
	while(true){
		calc_t c = expression() ;
		read_and_discard_space(stream) ;
		t = read_next(stream) ;
		if (t.type == END)
			printf("%Lf\n", c) ;	
	}
}
