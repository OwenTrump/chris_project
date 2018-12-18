#include "prim.h"
#include "expr.h"

calc_t number(void) 
{
	token_t t ;
	char buffer[128] ;
	size_t pos = 0 ;

	_Bool continue_loop = true ;
	

	for ( t = read_next(stream) ; continue_loop ; t = read_next(stream) ){
		
		switch(t.type){
		
		case DIGIT:  
		case DOT:  
			{ buffer[pos] = t.ch; break ; }
		case ALPHA:
		{
			if ( t.ch == 'E' || t.ch == 'e' ){
				buffer[pos] = t.ch ; break ;
			}
			else{
				putback(stream, t ) ;
				break ;
			}
			break ;
		}
		case PLUS:
		case MINUS:
		{
			if ( tolower(buffer[pos-1]) == 'e'){
				buffer[pos] = t.ch ;
				break ;
			}
			else if ( pos == 0 ){
				buffer[pos] = t.ch ;
				break ;
			}
			else {
				// binary operand 
				putback(stream, t) ;
				continue_loop = false ;
				break ;
			}
			break ;
		}
		
		default:
		{
			continue_loop = false ;
			putback(stream, t ) ;
			break ;
			
		}
		
		}
		if (!continue_loop) break ;
		if ( pos == 128) break ;
		++pos ;
	}
	
	buffer[pos] = '\0' ;
	errno = 0 ;
	char *endptr = NULL;

	calc_t ret = strtold(buffer, &endptr ) ;
	if ( endptr == buffer ) return NAN ;
	if ( ret == (calc_t)0 && errno != 0 ) return NAN ;
	else return ret ;
}


char* name(void) 
{
	char buffer[NAME_MAX] ;
	size_t pos = 0 ;

	token_t t = read_next(stream) ;

	while ( t.type == ALPHA || t.type == UNDERSCORE ){
		buffer[pos++] = t.ch  ;
		t = read_next(stream) ;
	}
	putback(stream, t) ;
	buffer[pos] = '\0' ;
	char* n = malloc(strlen(buffer) + 1 ) ;
	strcpy( n, buffer ) ;
	
	return n ;
	
}

calc_t primary(void) 
{
	token_t t ;
	calc_t accumulator = 0.0 ;
	char* n ;
	calc_t value ;
	
	while(true){
	
		read_and_discard_space(stream) ;
		t = read_next(stream) ;
		
		switch(t.type){
		
		case MINUS:
		case PLUS:
		case DIGIT:
		{
			putback(stream, t) ;
			read_and_discard_space(stream) ;
			accumulator = number() ;
			read_and_discard_space(stream) ;
			t = read_next(stream) ;
			if ( t.type == EXP ){
				read_and_discard_space(stream) ;
				calc_t ex = number() ;
				return powl(accumulator, ex) ;
			}
			else{ 
				putback(stream, t);
			}
			return accumulator ;
			break ;
		}
		
		case ALPHA:
		case UNDERSCORE:
		{
			putback(stream, t) ;
			char* n = name() ;
			read_and_discard_space(stream) ;
			t = read_next(stream) ;
			if ( t.type == ASSIGN){
				read_and_discard_space(stream) ;
				value = expression() ; // just for test ;
				d_add_entry(symbol_table, n, &value); 
				return primary();
			}
			else{
				const void* res = d_get_value(symbol_table, n ) ;
				if ( res == NULL ){
					char message[512] ;
					strcat(message, n) ;
					strcat( message, " is undefined ") ;
					set_error(UNDEFINED_VARIABLE, message, stream ) ;
					die_with_message() ;
				}
				accumulator = *((calc_t*)res) ;
				putback(stream, t) ;
				return primary() ;	
			}
			
			break ;
		}
		case BRACKET_LEFT:
		{
			accumulator = expression() ;
			read_and_discard_space(stream) ;
			t = read_next(stream) ;
			
			if ( t.type != BRACKET_RIGHT){
				set_error(UNMATCHED_PARENTHESIS, "expected a ) ", stream) ;
				die_with_message() ;
			}
			else return accumulator ;
			
			break ;
		}
		default:
		{
			putback( stream, t) ;
			return 0.0 ;
		}
					
		}
		
	}
}



