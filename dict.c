#include "dict.h"

dict_t* new_dict ( size_t size , CmpFunc_t* key, CmpFunc_t* val, ObjSize_t* k_size , ObjSize_t* v_size) 
{
	dict_t* d = (dict_t*)malloc(sizeof(dict_t) + sizeof(entry_t*) * size ) ;

	if ( d != NULL ){

		d->_size = size ;
		d->_pos = 0 ;
		d->_key_cmp = ( key == NULL ) ? default_cmp : key ;
		d->_val_cmp = ( val == NULL ) ? default_cmp : val ;
		d->_key_size = ( k_size == NULL) ? default_obj_size : k_size ;
		d->_val_size = ( v_size == NULL ) ? default_obj_size : v_size ;
		
	}

	return d ;	
}


entry_t* d_find_key( dict_t* dict , const void* key) 
{
	for ( int i = 0 ; i < dict->_pos ; ++i ){
		if ( dict->_key_cmp( key , dict->_start[i]->key ) == 0 )
			return dict->_start[i]  ;
	}
	
	return NULL ;
}

_Bool d_contains_key( dict_t* dict, const void* key ) 
{
	return d_find_key(dict, key) != NULL ;
}

// if a key already exists with the defined key, it is replaced 
entry_t* d_add_entry( dict_t* dict, const void* key, const void* val ) 
{
	if ( dict->_pos == dict->_size ) return NULL ;
	
	entry_t* f;

	if ((f = d_find_key(dict, key )) != NULL ){

		memcpy( f->value, val, dict->_val_size(val)) ;
		return f;
	}
	
	entry_t* ent = (entry_t*)malloc(sizeof(entry_t)) ;
	
	if ( ent != NULL ){
		ent->key = malloc(dict->_key_size(key)) ;
		ent->value = malloc(dict->_val_size(val)) ;
		
		if ( ent->key == NULL || ent->value == NULL ){
			free(ent) ;
			return NULL ;

		}
		memcpy(ent->key, key, dict->_key_size(key) ) ;
		memcpy(ent->value, val, dict->_val_size(val) ) ;
		dict->_start[dict->_pos] = ent ;
		dict->_pos++ ;
	}
	
	return ent ;
	
	
}

// returns NULL if entry does not exist

entry_t* d_update_key( dict_t* dict, const void* key, const void* val ) 
{
	entry_t* ent ;
	if ( ( ent = d_find_key( dict, key) )!= NULL )
		memcpy(dict->_start[dict->_pos]->value , val, dict->_val_size(val)) ;
		
	return ent ;  
}

const void* d_get_value( dict_t* dict, const void* key ) 
{
	entry_t* ent ;
	if ((ent = d_find_key(dict, key)) != NULL ) 
		return ent->value ;
		
	return NULL ;
}

void destroy_dict ( dict_t* dict ) 
{
	for ( int i = 0 ; i < dict->_pos; ++i ){
		entry_t* ent = dict->_start[i] ;
		free(ent->key) ;
		free(ent->value);
		free(ent) ;
	}
	free(dict) ;
}

size_t d_size(dict_t* dict) 
{
	return dict->_size;
}

_Bool d_full( dict_t* dict )
{
	return dict->_size == dict->_pos ;
}

int default_cmp( const void* key1, const void* key2 ) 
{
	return ( key1 == key2 ) ? 0 : ( key1 < key2 ) ? -1 : 1 ;
}

size_t default_obj_size ( const void* obj) 
{
	return sizeof(obj) ;
}

void d_iterators( dict_t* dict, entry_t** begin, entry_t** end ) 
{
	entry_t** start = dict->_start ;
	*begin = (entry_t*)start ;
	*start = dict->_start[dict->_pos] ;	
	*end = (entry_t*)start ;
}
