#ifndef PHP_INDEXED_ARRAY_H
# define PHP_INDEXED_ARRAY_H

extern zend_module_entry indexed_array_module_entry;
# define phpext_indexed_array_ptr &indexed_array_module_entry

# define PHP_INDEXED_ARRAY_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_INDEXED_ARRAY)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#define PHP_INDEXED_FETCH_FROM(o)	((IndexedArray*) (((char*)o) - XtOffsetOf(IndexedArray, std)))
#define PHP_INDEXED_FETCH(z)		PHP_INDEXED_FETCH_FROM(Z_OBJ_P(z))


zend_class_entry *Indexed_ce;

#endif