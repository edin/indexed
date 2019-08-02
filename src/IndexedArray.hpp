#pragma once

extern "C" {
    #include "php.h"
    #include "zend_interfaces.h"
    #include "ext/spl/spl_iterators.h"
}

extern zend_function_entry IndexedArrayMethods[];


class IndexedArray {
    public:
        zval        *data;
        zend_long    size;
        zend_object  std;

        static zend_object_handlers IndexedArrayHandlers;

    public:

    static zend_object* create(zend_class_entry *ce) {
        IndexedArray *instance = (IndexedArray*) ecalloc(1, sizeof(IndexedArray));
        zend_object_std_init(&instance->std, ce);
        object_properties_init(&instance->std, ce);

        instance->std.handlers = &IndexedArrayHandlers;
        return &instance->std;
    }

    static void Destroy(zend_object *object)
    {
        zend_objects_destroy_object(object);
    }

    static IndexedArray* getObject(zval *obj) {
        auto val = Z_OBJ_P(obj);
        return (IndexedArray*)((char*)(val) - XtOffsetOf(IndexedArray, std));
    }

    bool checkIndex(zend_long i) {
        if (i >= this->size) {
            //zend_throw_exception_ex(NULL, 0, "Index %ld is out of bounds", i);
            return false;
        }
        if (i < 0) {
            if ((this->size + i) < 0)  {
                //zend_throw_exception_ex(NULL, 0, "Index %ld (%d) is out of bounds", i, this->size + i);
                return false;
            }
            i = this->size + i;
        }
        return true;
    }

    void construct(zend_long size, HashTable *data) {

        this->size = size;
        this->data = (zval*) ecalloc(sizeof(zval), this->size);

        if (data) {
            setData(data);
        }
    }

    zend_long getCount() {
        return this->size;
    }

    void offsetGet(zend_long index, zval *return_value) {

        //PHP_INDEXED_CHECK(this, index);

        if (Z_TYPE(this->data[index]) != IS_UNDEF) {
            ZVAL_COPY(return_value, &this->data[index]);
        }
    }

    void offsetSet(int index, zval* value) {
        if (Z_TYPE(this->data[index]) != IS_UNDEF) {
            zval_ptr_dtor(&this->data[index]);
        }

        ZVAL_COPY(&this->data[index], value);
    }

    void offsetUnset(int index) {

        //PHP_INDEXED_CHECK(pi, index);

        if (Z_TYPE(this->data[index]) != IS_UNDEF) {
            zval_ptr_dtor(&this->data[index]);
            ZVAL_UNDEF(&this->data[index]);
        }
    }

    void offsetExists(int index, zval* return_value) {

        //PHP_INDEXED_CHECK(this, index);

        RETURN_BOOL(Z_TYPE(this->data[index]) != IS_UNDEF);
    }

    void resize(zend_long resize) {
        while (resize < this->size) {
            if (Z_TYPE(this->data[this->size-1]) != IS_UNDEF) {
                zval_ptr_dtor(&this->data[this->size-1]);
            }
            this->size--;
        }

        this->data = (zval*) erealloc(this->data, sizeof(zval) * resize);

        while (this->size < resize) {
            ZVAL_UNDEF(&this->data[this->size++]);
        }
    }

    void flip(zval *retval) {
        zend_long it;

        object_init_ex(retval, this->std.ce);

        IndexedArray* pf = IndexedArray::getObject(retval);
        pf->size = this->size;
        pf->data = (zval*) ecalloc(pf->size, sizeof(zval));

        for (it = pf->size; it > 0; it--) {
            ZVAL_COPY(&pf->data[(pf->size) - it], &this->data[it - 1]);
        }
    }

    void setData(HashTable *data) {
        zval      *item;
        zval      *items;

        if (!data) {
            return;
        }

        if (this->size < zend_hash_num_elements(data)) {
            this->resize(zend_hash_num_elements(data));
        }

        items = this->data;

        ZEND_HASH_FOREACH_VAL(data, item) {
            ZVAL_COPY(items++, item);
        }
        ZEND_HASH_FOREACH_END();
    }

    static void free(zend_object *o) {

        IndexedArray *instance = PHP_INDEXED_FETCH_FROM(o);

        if (instance->data) {
            zend_long it = 0, end = instance->size;

            for (; it < end; it++) {
                zval_ptr_dtor(&instance->data[it]);
            }
            efree(instance->data);
        }
        zend_object_std_dtor(o);
    }

    static HashTable* gc(zend_object *indexed, zval **table, int *n) {
        IndexedArray *pi = PHP_INDEXED_FETCH_FROM(indexed);

        *table = pi->data;
        *n = (int)pi->size;

        return NULL;
    }

    static HashTable* dump(zend_object *indexed, int *is_temp) {
        IndexedArray *instance = PHP_INDEXED_FETCH_FROM(indexed);
        HashTable *ht;
        zend_long it;

        ALLOC_HASHTABLE(ht);
        zend_hash_init(ht, (uint32_t)instance->size, NULL, ZVAL_PTR_DTOR, 0);
        *is_temp = 1;

        for (it = 0; it < instance->size; it++) {
            if (Z_TYPE(instance->data[it]) != IS_UNDEF &&
                zend_hash_index_update(ht, it, &instance->data[it])) {
                Z_TRY_ADDREF(instance->data[it]);
            }
        }

        return ht;
    }

    static zend_object* clone(zend_object *object) {
        IndexedArray *pi = PHP_INDEXED_FETCH_FROM(object);
        IndexedArray *cl = (IndexedArray*) ecalloc(1, sizeof(IndexedArray));
        zend_long it;

        zend_object_std_init(&cl->std, pi->std.ce);

        cl->std.handlers = &IndexedArrayHandlers;

        cl->data = (zval*) ecalloc(pi->size, sizeof(zval));

        for (it = 0; it < pi->size; it++) {
            ZVAL_COPY(&cl->data[it], &pi->data[it]);
        }

        cl->size = pi->size;
        return &cl->std;
    }

    static int castTo(zend_object *indexed, zval *retval, int type) {
        IndexedArray *instance = PHP_INDEXED_FETCH_FROM(indexed);
        zend_long it;

        if (type != IS_ARRAY) {
            return FAILURE;
        }

        array_init(retval);

        for (it = 0; it < instance->size; it++) {
            if (Z_TYPE(instance->data[it]) != IS_UNDEF &&
                add_next_index_zval(retval, &instance->data[it])) {
                Z_TRY_ADDREF(instance->data[it]);
            }
        }

        return SUCCESS;
    }

    // Disable dynamic properties
    static void noProperties() {
        //zend_throw_exception_ex(NULL, 0, "Properties on IndexedArray are not allowed");
    }

    static zval* propertyRead(zend_object *object, zend_string *member, int type, void **cache_slot, zval *rv) {
        noProperties();
        return &EG(uninitialized_zval);
    }

    static zval* propertyWrite(zend_object *object, zend_string *member, zval *value, void **cache_slot) {
        noProperties();
        return &EG(uninitialized_zval);
    }

    static int propertyExists(zend_object *object, zend_string *member, int has_set_exists, void **cache_slot) {
        noProperties();
        return 0;
    }

    static void propertyUnset(zend_object *object, zend_string *member, void **cache_slot) {
        noProperties();
    }

    static void Initialize() {
        zend_class_entry ce;


        INIT_CLASS_ENTRY(ce, "IndexedArray", IndexedArrayMethods);
        Indexed_ce = zend_register_internal_class(&ce);
        Indexed_ce->create_object = IndexedArray::create;
        //Indexed_ce->get_iterator = php_indexed_iterator;
        Indexed_ce->ce_flags |= ZEND_ACC_FINAL;

        zend_class_implements(Indexed_ce, 2, spl_ce_ArrayAccess, spl_ce_Countable);

        memcpy(&IndexedArrayHandlers, zend_get_std_object_handlers(), sizeof(IndexedArrayHandlers));

        IndexedArrayHandlers.free_obj = IndexedArray::free;
        IndexedArrayHandlers.dtor_obj = IndexedArray::Destroy;

        IndexedArrayHandlers.get_gc = IndexedArray::gc;
        IndexedArrayHandlers.get_debug_info = IndexedArray::dump;
        IndexedArrayHandlers.clone_obj = IndexedArray::clone;
        IndexedArrayHandlers.cast_object = IndexedArray::castTo;

        IndexedArrayHandlers.read_property = IndexedArray::propertyRead;
        IndexedArrayHandlers.write_property = IndexedArray::propertyWrite;
        IndexedArrayHandlers.has_property = IndexedArray::propertyExists;
        IndexedArrayHandlers.unset_property = IndexedArray::propertyUnset;

        IndexedArrayHandlers.get_properties = NULL;

        IndexedArrayHandlers.offset = XtOffsetOf(IndexedArray, std);
    }
};