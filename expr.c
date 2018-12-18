#include "expr.h"

calc_t expression(void)
{
	
	read_and_discard_space(stream) ;
	calc_t lhs = term() ;
	read_and_discard_space(stream);
	token_t t = read_next(stream);
	for(;;){
	if ( t.type == PLUS ){

		read_and_discard_space(stream) ;
		calc_t rhs = term() ;
		read_and_discard_space(stream) ;
		lhs += rhs ;
	}
	else if ( t.type == MINUS ){
		
		read_and_discard_space(stream) ;
		calc_t rhs = term() ;
		read_and_discard_space(stream) ;
		lhs -= rhs ;	
	}
	else{
		putback(stream, t) ;
		return lhs ;
	}
	}
}

