#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_indexed_object.h"

typedef struct _php_indexed_iterator_t {
	zend_object_iterator it;
	zval indexed;
	zend_long pos;
} php_indexed_iterator_t;

static php_indexed_iterator_dtor(php_indexed_iterator_t *pi) {
	zval_ptr_dtor(&pi->indexed);
}

static int php_indexed_iterator_validate(php_indexed_iterator_t *pi) {
	php_indexed_t *pl = PHP_INDEXED_FETCH_FROM(Z_OBJ(pi->indexed));

	return (pi->pos < pl->size) ? SUCCESS: FAILURE;
}

static zval* php_indexed_iterator_current_data(php_indexed_iterator_t *pi) {
	php_indexed_t *pl = PHP_INDEXED_FETCH_FROM(Z_OBJ(pi->indexed));

	return &pl->data[pi->pos];
}

static void php_indexed_iterator_current_key(php_indexed_iterator_t *pi, zval *key) {
	ZVAL_LONG(key, pi->pos);
}

static void php_indexed_iterator_move_forward(php_indexed_iterator_t *pi) {
	pi->pos++;
}

static zend_object_iterator_funcs php_indexed_iterator_funcs = {
	(void (*)(zend_object_iterator *))		php_indexed_iterator_dtor,
	(int (*)(zend_object_iterator *))		php_indexed_iterator_validate,
	(zval* (*)(zend_object_iterator *))		php_indexed_iterator_current_data,
	(void (*)(zend_object_iterator *, zval *))	php_indexed_iterator_current_key,
	(void (*)(zend_object_iterator *))		php_indexed_iterator_move_forward
};

zend_object_iterator* php_indexed_iterator(zend_class_entry *ce, zval *indexed, int by_ref) {
	php_indexed_iterator_t *pi = (php_indexed_iterator_t*) emalloc(sizeof(php_indexed_iterator_t));

	zend_iterator_init((zend_object_iterator*) pi);

	pi->pos = 0;
	pi->it.funcs = &php_indexed_iterator_funcs;

	ZVAL_COPY(&pi->indexed, indexed);

	return (zend_object_iterator*) pi;
}