/* indexed_array extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

extern "C" {
    #include "php.h"
    #include "ext/standard/info.h"
    #include "php_indexed_array.h"
}

#include "src\IndexedArray.hpp"

zend_object_handlers IndexedArray::IndexedArrayHandlers;

#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_METHOD(Indexed, __construct)
{
	zend_long size;
	HashTable *data = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l|H", &size, &data) != SUCCESS) {
		return;
	}

	auto instance = IndexedArray::getObject(getThis());
	instance->construct(size, data);
}

PHP_METHOD(Indexed, count)
{
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}
	auto instance = IndexedArray::getObject(getThis());
	RETURN_LONG(instance->getCount());
}

PHP_METHOD(Indexed, offsetSet)
{
	zend_long index;
	zval *value;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz", &index, &value) != SUCCESS) {
		return;
	}

	auto instance = IndexedArray::getObject(getThis());
	instance->offsetSet(index, value);
}

PHP_METHOD(Indexed, offsetGet)
{
	zend_long index;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	auto instance = IndexedArray::getObject(getThis());
	instance->offsetGet(index, return_value);
}

PHP_METHOD(Indexed, offsetUnset)
{
	zend_long index;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	auto instance = IndexedArray::getObject(getThis());
	instance->offsetUnset(index);
}

PHP_METHOD(Indexed, offsetExists)
{
	zend_long index;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &index) != SUCCESS) {
		return;
	}

	auto instance = IndexedArray::getObject(getThis());
	instance->offsetExists(index, return_value);
}

PHP_METHOD(Indexed, resize)
{
	zend_long resize;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &resize) != SUCCESS) {
		return;
	}
	auto instance = IndexedArray::getObject(getThis());
	instance->resize(resize);
}

PHP_METHOD(Indexed, flip)
{
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	auto instance = IndexedArray::getObject(getThis());
	instance->flip(return_value);
}

ZEND_BEGIN_ARG_INFO_EX(Indexed_construct_arginfo, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(Indexed_count_arginfo, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Indexed_set_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Indexed_get_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Indexed_unset_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Indexed_exists_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Indexed_resize_arginfo, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(Indexed_flip_arginfo, 0, 0, "IndexedArray", 1)
ZEND_END_ARG_INFO()

zend_function_entry IndexedArrayMethods[] = {
	PHP_ME(Indexed, __construct,  Indexed_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, count,        Indexed_count_arginfo,     ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, offsetSet,    Indexed_set_arginfo,	 ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, offsetGet,    Indexed_get_arginfo,	 ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, offsetUnset,  Indexed_unset_arginfo,     ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, offsetExists, Indexed_exists_arginfo,    ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, resize,	      Indexed_resize_arginfo,    ZEND_ACC_PUBLIC)
	PHP_ME(Indexed, flip,	      Indexed_flip_arginfo,      ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_RINIT_FUNCTION(indexed_array)
{
#if defined(ZTS) && defined(COMPILE_DL_INDEXED_ARRAY)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_MINIT_FUNCTION(indexed_array)
{
	#if defined(ZTS) && defined(COMPILE_DL_INDEXED_ARRAY)
		ZEND_TSRMLS_CACHE_UPDATE();
	#endif

   	IndexedArray::Initialize();
	return SUCCESS;
}


PHP_MINFO_FUNCTION(indexed_array) {
	php_info_print_table_start();
	php_info_print_table_header(2, "indexed_array support", "enabled");
	php_info_print_table_end();
}

ZEND_BEGIN_ARG_INFO(arginfo_indexed_array_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_indexed_array_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()


static const zend_function_entry indexed_array_functions[] = {
	PHP_FE_END
};

zend_module_entry indexed_array_module_entry = {
	STANDARD_MODULE_HEADER,
	"indexed_array",				/* Extension name */
	indexed_array_functions,		/* zend_function_entry */
	PHP_MINIT(indexed_array),		/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(indexed_array),		/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(indexed_array),		/* PHP_MINFO - Module info */
	PHP_INDEXED_ARRAY_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_INDEXED_ARRAY
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(indexed_array)
#endif
