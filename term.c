#include "term.h"

calc_t term(void)
{
	read_and_discard_space(stream) ;
	calc_t lhs = primary() ;
	
	read_and_discard_space(stream) ;
	token_t t = read_next(stream) ;

	for(;;){
	if ( t.type == MULTIPLY ){
		read_and_discard_space(stream) ;
		calc_t rhs = primary() ;
		if ( isnan(rhs) ){
			set_error(EOL_WHILE_PARSING, "expected an expression after *", stream) ;
			die_with_message() ;
		}
		read_and_discard_space(stream) ;
		lhs *= rhs ;
	}
	else if ( t.type == DIVISION){
		read_and_discard_space(stream) ;
		calc_t rhs = primary() ;
		read_and_discard_space(stream) ;
		if ( rhs == (calc_t)0){
			set_error(DIVISION_BY_ZERO, "math error", stream) ;
			die_with_message() ;
		}
		if ( isnan(rhs)){
			set_error(EOL_WHILE_PARSING, "expected an expression after /", stream) ;
			die_with_message() ;
		}
		else lhs /= rhs ;
	}
	else{
		putback(stream, t) ;
		return lhs ;
	}
	}
}
