#include "input_stream.h" 

input_stream_t* create_stream( void* stream, stream_type st ) 
{
	input_stream_t* str = (input_stream_t*)malloc(sizeof(input_stream_t)) ;
	
	if ( str != NULL ){
		if ( st == DEVICE ) str->stream.device = (FILE*)stream ;
		else str->stream.memory = (char*)stream ;
		
		str->pos = 0 ;
		str->buffer_full = false ;
		str->s_type = st ;
	}
	
	return str ;
}

token_t read_next ( input_stream_t* input ) 
{
	if ( input->s_type == DEVICE ) return device_read( input) ;
	else return memory_read( input) ;
}


_Bool putback( input_stream_t* input, token_t read ) 
{
	if ( input->buffer_full ) return false ;
	
	if ( read.ch != input->current ) return false ;
	
	input->buffer = read ;
	input->buffer_full = true ;
	
	return true ;
}

size_t read_and_discard_space( input_stream_t* input ) 
{
	token_t t = read_next(input) ;
	size_t count = 0 ;
	
	while ( t.type == SPACE ){
		++count;
		t = read_next(stream) ;
	}
	putback(stream, t) ;
	return count ;	
}


void free_stream( input_stream_t* input ) 
{
	if ( input->s_type == DEVICE ) fclose( input->stream.device ) ;
	free(input) ;
}


static token_t parse_token( char token ) 
{	
	token_t t ;
	
	if (isalpha(token)) { t.type = ALPHA; } 
	else if ( isdigit(token)) { t.type = DIGIT ;} 
	else if ( token == ' ' || token == '\t' ) { t.type = SPACE ; } 
	else {
		switch(token)
		{
		case '\n' : { t.type = END ; break ;} 
		case '=' : { t.type = ASSIGN ; break ;} 
		case '-' : { t.type = MINUS ; break ;}
		case '*' : { t.type = MULTIPLY;break ; }
		case '+' : { t.type = PLUS ; break ;}
		case '^' : { t.type = EXP ; break ;} 
		case '/' : { t.type = DIVISION ; break ;} 
		case '(': { t.type = BRACKET_LEFT; break ;} 
		case ')': { t.type = BRACKET_RIGHT; break ;} 
		case '.': { t.type = DOT ; break ;} 
		case '_': { t.type = UNDERSCORE ; break ;}
		default: { t.type = UNKNOWN ; }  
		}
	}
	
	t.ch = token ;
	return t ;
}

static token_t memory_read( input_stream_t* ptr ) 
{
	if ( ptr->buffer_full) {
		ptr->buffer_full = false ;
		return ptr->buffer ;
	}
	
	token_t t ;
	ptr->current = *(ptr->stream.memory + ptr->pos ) ;
	++ptr->pos ;
	
	if ( ptr->current == '\0') { t.type = EOT ; t.ch = '\0'; } 
	else return parse_token( ptr->current ) ;

	return t;
}

static token_t device_read( input_stream_t* dev ) 
{
	if ( dev->buffer_full) {
		dev->buffer_full = false ;
		return dev->buffer ;
	}

	token_t t ;
	dev->current = fgetc(dev->stream.device) ;
	
	if ( dev->current == EOF ) { t.type = EOT ; } 
	else if ( ferror(dev->stream.device )) { t.type = EOT ; }
	else t = parse_token(dev->current) ;

	++dev->pos ;
	return t ; 
}

