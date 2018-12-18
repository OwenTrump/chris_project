#include "config.h"

#ifdef NDEBUG 
	#define chk_putback(x,y) (stream_putback(x,y))
#else 
	#define chk_putback(x,y) (assert(stream_putback(x,y))) 
#endif


typedef enum { NAME, ASSIGN, NUMBER , EOS = EOF, PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/',  L_BRACKET = '(',
		 R_BRACKET = ')',  EXP = '^', END = '\n' } kind_t ;

typedef struct{
	
	kind_t 	kind ;
	union{
		char 	str[NAME_MAX] ;
		char 	ch ;
		calc_t  val ;
	};
	
}token_t ;

typedef enum { DEVICE, MEMORY } stream_type_t ;

typedef union {

	FILE * device ;
	char*  memory ;
	
} stream_t ;

typedef struct {

	stream_t 	_stream ;
	stream_type_t	_s_type ; 
	
	token_t		_current ;
	token_t		_previous ;
	
	char		_buffer ;
	bool		_buffer_full ;
	
	size_t		_pos ;
	
} token_stream_t ;

extern token_stream_t* 	input_stream ;

// create a new stream and returns a pointer to it 
// it initializes most of the stream 
// some members still remain uninitialized after the call 
token_stream_t* new_stream( void* stream, stream_type_t type ) ;

// extracts the next token from the stream
// skips all while space characters 
// it updates _pos with the number if charcters extracted so far 
// on first extraction, _current == _previous  
token_t* stream_read( token_stream_t* pStream ) ;

bool stream_putback(token_stream_t* pStream, char ch ) ;

token_t* stream_current(token_stream_t* pStream) ;

void free_stream(token_stream_t* pStream ) ;

static const char* get_str(token_stream_t* pStream ) ;

static calc_t get_number( token_stream_t* pStream ) ;

static char stream_get ( token_stream_t* pStream ) ;

