#ifndef _DICT_H_
#define _DICT_H_

#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct {

	void* key;
	void* value ;

} entry_t ;

typedef int (CmpFunc_t) ( const void* , const void* ) ;
typedef size_t (ObjSize_t)(const void*) ;


typedef struct {

	size_t 	_size ;
	size_t _pos ;
	
	
	CmpFunc_t* _key_cmp ;
	CmpFunc_t* _val_cmp ;
	ObjSize_t* _key_size ;
	ObjSize_t* _val_size ;	

	entry_t* _start[] ;	

} dict_t ;


extern dict_t* symbol_table ;


dict_t* new_dict ( size_t size ,CmpFunc_t* key, CmpFunc_t* val, ObjSize_t* k_size , ObjSize_t* v_size) ;

entry_t* d_find_key( dict_t* dict , const void* key) ;

_Bool d_contains_key( dict_t* dict, const void* key ) ;

size_t d_size(dict_t* dict) ;

_Bool d_full( dict_t* dict ) ;
// if a key already exists with the defined key, it is replaced 
entry_t* d_add_entry( dict_t* dict, const void* key, const void* val ) ;

entry_t* d_update_key( dict_t* dict, const void* key, const void* val ) ;

const void* d_get_value( dict_t* dict, const void* key ) ;

void d_iterators( dict_t* dict, entry_t** begin, entry_t** end ) ;

void destroy_dict ( dict_t* dict ) ;

int default_cmp( const void* key1, const void* key2 ) ;
size_t default_obj_size ( const void* obj) ;


#endif 

