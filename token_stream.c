#include "token_stream.h"

token_stream_t* new_stream( void* stream, stream_type_t type ) 
{
	token_stream_t * st = (token_stream_t*)malloc(sizeof(token_stream_t)) ;
	
	if ( stream != NULL ){

		st->_s_type = type ;
		if ( type == DEVICE ) st->_stream.device = (FILE*)stream ;
		else st->_stream.memory = (char*)stream ;

		st->_pos = 0 ;
		st->_buffer_full = false ;
	}
	
	return st ;
}

token_t* stream_read( token_stream_t* pStream ) 
{
	char ch = stream_get(pStream) ;
	
	if (isblank(ch)){
		
		while (isblank(ch = stream_get(pStream)))
			pStream->_pos++ ;
	}
	
	pStream->_previous = pStream->_current ;

	if(isalpha(ch)){
	
		chk_putback(pStream, ch ) ;
		const char* n = get_str(pStream) ;
		strcpy(pStream->_current.str, n ) ;
		free((char*)n) ;
		pStream->_current.kind = NAME ;
		return &pStream->_current;
	}
	
	switch(ch){

	case 1 : case 2: case 3: case 4: case 5 : case 6: case 7: case 8: case 9: case 0:
	{ 
		chk_putback(pStream, ch ) ;
		pStream->_current.val = get_number(pStream) ; 
		pStream->_current.kind = NUMBER ;
		return &pStream->_current; 	
	}
	case '-': case '*': case '+': case '=': case '^': case '/': case ')': case '(': case '\n': case EOF:
	{
		pStream->_current.ch = ch ;
		pStream->_current.kind = (kind_t)ch ;
		return &pStream->_current ;
	}
	default:
	{
		pStream->_current.kind = EOS ;
		return &pStream->_current ;
	}
	
	
	}
}


static char stream_get ( token_stream_t* pStream )
{
	if (pStream->_buffer_full){
		pStream->_buffer_full = false ;
		return pStream->_buffer ;
	}

	if (pStream->_s_type == MEMORY){
		char ch = *(pStream->_stream.memory + pStream->_pos++ );
		return ch == '\0' ? EOF : ch ; 
	}
	else {	
		char ch = fgetc(pStream->_stream.device) ;
		++pStream->_pos ;
		return ch ;
	}
}


bool stream_putback(token_stream_t* pStream, char ch ) 
{
	if ( pStream->_buffer_full ) return false ;
	
	pStream->_buffer  = ch ;
	pStream->_buffer_full = true ;	
	
	return true ;
}

token_t* stream_current(token_stream_t* pStream) 
{
	return &pStream->_current ;
}

static const char* get_str(token_stream_t* pStream ) 
{
	char buffer[NAME_MAX] ;
	int res = fscanf(pStream->_stream.device, "%[1234567890_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]", buffer) ;
	if ( res < 1 ) return NULL ;
	return strcpy( (char*)malloc(strlen(buffer)+1), buffer) ;
}

static calc_t get_number( token_stream_t* pStream ) 
{
	calc_t val ;
	int num ;
	int res = fscanf(pStream->_stream.device, "%Lf %n", &val, &num) ;
	if ( res < 1 ) return NAN ;
	pStream->_pos += num ;
	return val ;
}

void free_stream(token_stream_t* pStream ) 
{
	free(pStream) ;
}




