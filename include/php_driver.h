#pragma once

#include <cassandra.h>
#include <gmp.h>
#include <version.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <math.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include <php.h>

#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_types.h>

#ifdef _WIN32
#define LL_FORMAT "%I64d"
#else
#define LL_FORMAT "%lld"
#endif

#if PHP_VERSION_ID < 80000
#error PHP 8.0.0 or later is required in order to build the driver
#endif


#include <ext/spl/spl_exceptions.h>
#include <ext/spl/spl_iterators.h>

#if defined(__GNUC__) && __GNUC__ >= 4
#define PHP_DRIVER_API __attribute__((visibility("default")))
#else
#define PHP_DRIVER_API
#endif

#define PHP_DRIVER_NAMESPACE "Cassandra"

#define PHP_DRIVER_NAMESPACE_ZEND_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null)                               \
    ZEND_ARG_OBJ_INFO(pass_by_ref, name, Cassandra\\classname, allow_null)

#define PHP_DRIVER_CORE_METHOD(name) PHP_METHOD(Cassandra, name)

#define PHP_DRIVER_CORE_ME(name, arg_info, flags) PHP_ME(Cassandra, name, arg_info, flags)

#define SAFE_STR(a) ((a) ? a : "")

#define SAFE_ZEND_STRING(a) ((a != NULL) ? ZSTR_VAL(a) : "")

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define PHP_DRIVER_G(v) TSRMG(php_driver_globals_id, zend_php_driver_globals *, v)
#else
#define PHP_DRIVER_G(v) (php_driver_globals.v)
#endif

#define CPP_DRIVER_VERSION(major, minor, patch) (((major) << 16) + ((minor) << 8) + (patch))

#define CURRENT_CPP_DRIVER_VERSION CPP_DRIVER_VERSION(CASS_VERSION_MAJOR, CASS_VERSION_MINOR, CASS_VERSION_PATCH)

#define php5to7_zend_register_internal_class_ex(ce, parent_ce)                                                         \
    zend_register_internal_class_ex((ce), (parent_ce));

    typedef zval php5to7_zval;
    typedef zval *php5to7_zval_args;
    typedef zend_string *php5to7_string;
    typedef zend_long php5to7_long;
    typedef zend_ulong php5to7_ulong;
    typedef zval php5to7_zend_resource_le;
    typedef zend_resource *php5to7_zend_resource;
    typedef zend_object *php5to7_zend_object;
    typedef zend_object php5to7_zend_object_free;
    typedef zval **php5to7_zval_gc;
    typedef zval *php5to7_dtor;
    typedef size_t php5to7_size;
    typedef unsigned long ulong;

    static inline int php5to7_string_compare(php5to7_string s1, php5to7_string s2)
    {
        if (s1->len != s2->len)
        {
            return s1->len < s2->len ? -1 : 1;
        }
        return memcmp(s1->val, s2->val, s1->len);
    }

#define PHP5TO7_ZEND_OBJECT_GET(type_name, object) php_driver_##type_name##_object_fetch(object)

#define PHP5TO7_SMART_STR_INIT                                                                                         \
    {                                                                                                                  \
        NULL, 0                                                                                                        \
    }
#define PHP5TO7_SMART_STR_VAL(ss) ((ss).s ? (ss).s->val : NULL)
#define PHP5TO7_SMART_STR_LEN(ss) ((ss).s ? (ss).s->len : 0)

#define PHP5TO7_STRCMP(s, c) strcmp((s)->val, (c))
#define PHP5TO7_STRVAL(s) ((s)->val)

#define PHP5TO7_ZEND_ACC_FINAL ZEND_ACC_FINAL

#define PHP5TO7_ZEND_OBJECT_ECALLOC(type_name, ce)                                                                     \
    (php_driver_##type_name *)ecalloc(1, sizeof(php_driver_##type_name) + zend_object_properties_size(ce))

#define PHP5TO7_ZEND_OBJECT_INIT(type_name, self, ce) PHP5TO7_ZEND_OBJECT_INIT_EX(type_name, type_name, self, ce)

#define PHP5TO7_ZEND_OBJECT_INIT_EX(type_name, name, self, ce)                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        zend_object_std_init(&self->zval, ce);                                                               \
        ((zend_object_handlers *)&php_driver_##name##_handlers)->offset = XtOffsetOf(php_driver_##type_name, zval);    \
        ((zend_object_handlers *)&php_driver_##name##_handlers)->free_obj = php_driver_##name##_free;                  \
        self->zval.handlers = (zend_object_handlers *)&php_driver_##name##_handlers;                                   \
        return &self->zval;                                                                                            \
    } while (0)

#define PHP5TO7_MAYBE_EFREE(p) ((void)0)

#define PHP5TO7_ADD_ASSOC_ZVAL_EX(zv, key, len, val) add_assoc_zval_ex((zv), (key), (size_t)(len - 1), val)

#define PHP5TO7_ADD_ASSOC_STRINGL_EX(zv, key, key_len, str, str_len)                                                   \
    add_assoc_stringl_ex((zv), (key), (size_t)(key_len - 1), (char *)(str), (size_t)(str_len))

#define PHP5TO7_ADD_NEXT_INDEX_STRING(zv, str) add_next_index_string((zv), (char *)(str));

#define PHP5TO7_ZEND_HASH_FOREACH_VAL(ht, _val) ZEND_HASH_FOREACH_VAL(ht, _val)

#define PHP5TO7_ZEND_HASH_FOREACH_NUM_KEY_VAL(ht, _h, _val) ZEND_HASH_FOREACH_NUM_KEY_VAL(ht, _h, _val)

#if PHP_VERSION_ID >= 80200

#define PHP5TO7_ZEND_HASH_FOREACH_STR_KEY_VAL(ht, _key, _val)                                                          \
    ZEND_HASH_FOREACH(ht, 0);                                                                                          \
    if (__key)                                                                                                         \
    {                                                                                                                  \
        (_key) = ZSTR_VAL(__key);                                                                                      \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        (_key) = NULL;                                                                                                 \
    }                                                                                                                  \
    _val = _z;

#else

#define PHP5TO7_ZEND_HASH_FOREACH_STR_KEY_VAL(ht, _key, _val)                                                          \
    ZEND_HASH_FOREACH(ht, 0);                                                                                          \
    if (_p->key)                                                                                                       \
    {                                                                                                                  \
        (_key) = _p->key->val;                                                                                         \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        (_key) = NULL;                                                                                                 \
    }                                                                                                                  \
    _val = _z;

#endif

#define PHP5TO7_ZEND_HASH_FOREACH_END(ht) ZEND_HASH_FOREACH_END()

#define PHP5TO7_ZEND_HASH_GET_CURRENT_DATA(ht, res) ((res = zend_hash_get_current_data((ht))) != NULL)

#define PHP5TO7_ZEND_HASH_GET_CURRENT_DATA_EX(ht, res, pos) ((res = zend_hash_get_current_data_ex((ht), (pos))) != NULL)

#define PHP5TO7_ZEND_HASH_GET_CURRENT_DATA_EX(ht, res, pos) ((res = zend_hash_get_current_data_ex((ht), (pos))) != NULL)

#define PHP5TO7_ZEND_HASH_GET_CURRENT_KEY(ht, str_index, num_index)                                                    \
    zend_hash_get_current_key((ht), (str_index), (num_index))

#define PHP5TO7_ZEND_HASH_GET_CURRENT_KEY_EX(ht, str_index, num_index, pos)                                            \
    zend_hash_get_current_key_ex((ht), (str_index), (num_index), pos)

#define PHP5TO7_ZEND_HASH_EXISTS(ht, key, len) zend_hash_str_exists((ht), (key), (size_t)(len - 1))

#define PHP5TO7_ZEND_HASH_FIND(ht, key, len, res) ((res = zend_hash_str_find((ht), (key), (size_t)(len - 1))) != NULL)

#define PHP5TO7_ZEND_HASH_INDEX_FIND(ht, index, res)                                                                   \
    ((res = zend_hash_index_find((ht), (php5to7_ulong)(index))) != NULL)

#define PHP5TO7_ZEND_HASH_NEXT_INDEX_INSERT(ht, val, val_size) ((void)zend_hash_next_index_insert((ht), (val)))

#define PHP5TO7_ZEND_HASH_UPDATE(ht, key, len, val, val_size)                                                          \
    ((void)zend_hash_str_update((ht), (key), (size_t)(len - 1), (val)))

#define PHP5TO7_ZEND_HASH_INDEX_UPDATE(ht, index, val, val_size) ((void)zend_hash_index_update((ht), (index), (val)))

#define PHP5TO7_ZEND_HASH_ADD(ht, key, len, val, val_size)                                                             \
    ((void)zend_hash_str_add((ht), (key), (size_t)(len - 1), (val)))

#define PHP5TO7_ZEND_HASH_DEL(ht, key, len) ((zend_hash_str_del((ht), (key), (size_t)(len - 1))) == SUCCESS)

#define PHP5TO7_ZEND_HASH_ZVAL_COPY(dst, src) zend_hash_copy((dst), (src), (copy_ctor_func_t)zval_add_ref);

#define PHP5TO7_ZEND_HASH_SORT(ht, compare_func, renumber) zend_hash_sort(ht, compare_func, renumber)

#define PHP5TO7_ZVAL_COPY(zv1, zv2) ZVAL_COPY(zv1, zv2)
#define PHP5TO7_ZVAL_IS_UNDEF(zv) Z_ISUNDEF(zv)
#define PHP5TO7_ZVAL_IS_UNDEF_P(zv) Z_ISUNDEF_P(zv)

#define PHP5TO7_ZVAL_IS_BOOL_P(zv) (Z_TYPE_P(zv) == IS_TRUE || Z_TYPE_P(zv) == IS_FALSE)
#define PHP5TO7_ZVAL_IS_FALSE_P(zv) (Z_TYPE_P(zv) == IS_FALSE)
#define PHP5TO7_ZVAL_IS_TRUE_P(zv) (Z_TYPE_P(zv) == IS_TRUE)

#define PHP5TO7_ZVAL_UNDEF(zv) ZVAL_UNDEF(&(zv));
#define PHP5TO7_ZVAL_MAYBE_MAKE(zv) ((void)0)
#define PHP5TO7_ZVAL_MAYBE_DESTROY(zv)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!Z_ISUNDEF(zv))                                                                                            \
        {                                                                                                              \
            zval_ptr_dtor(&(zv));                                                                                      \
            ZVAL_UNDEF(&(zv));                                                                                         \
        }                                                                                                              \
    } while (0)

#define PHP5TO7_ZVAL_STRING(zv, s) ZVAL_STRING(zv, s)
#define PHP5TO7_ZVAL_STRINGL(zv, s, len) ZVAL_STRINGL(zv, s, len)
#define PHP5TO7_RETVAL_STRING(s) RETVAL_STRING(s)
#define PHP5TO7_RETVAL_STRINGL(s, len) RETVAL_STRINGL(s, len)
#define PHP5TO7_RETURN_STRINGL(s, len) RETURN_STRINGL(s, len)

#define PHP5TO7_ZVAL_ARG(zv) &(zv)
#define PHP5TO7_ZVAL_MAYBE_DEREF(zv) (zv)
#define PHP5TO7_ZVAL_MAYBE_ADDR_OF(zv) (zv)
#define PHP5TO7_ZVAL_MAYBE_P(zv) &(zv)
#define PHP5TO7_Z_TYPE_MAYBE_P(zv) Z_TYPE(zv)
#define PHP5TO7_Z_ARRVAL_MAYBE_P(zv) Z_ARRVAL(zv)
#define PHP5TO7_Z_OBJCE_MAYBE_P(zv) Z_OBJCE(zv)
#define PHP5TO7_Z_LVAL_MAYBE_P(zv) Z_LVAL(zv)
#define PHP5TO7_Z_STRVAL_MAYBE_P(zv) Z_STRVAL(zv)
#define PHP5TO7_Z_STRLEN_MAYBE_P(zv) Z_STRLEN(zv)

#define PHP5TO7_ZEND_LONG_MAX ZEND_LONG_MAX
#define PHP5TO7_ZEND_LONG_MIN ZEND_LONG_MIN

    extern zend_module_entry php_driver_module_entry;
#define phpext_cassandra_ptr &php_driver_module_entry

    PHP_MINIT_FUNCTION(php_driver);
    PHP_MSHUTDOWN_FUNCTION(php_driver);
    PHP_RINIT_FUNCTION(php_driver);
    PHP_RSHUTDOWN_FUNCTION(php_driver);
    PHP_MINFO_FUNCTION(php_driver);

    zend_class_entry *exception_class(CassError rc);

    void throw_invalid_argument(zval *object, const char *object_name, const char *expected_type);

#define INVALID_ARGUMENT(object, expected)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        throw_invalid_argument(object, #object, expected);                                                   \
        return;                                                                                                        \
    } while (0)

#define INVALID_ARGUMENT_VALUE(object, expected, failed_value)                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        throw_invalid_argument(object, #object, expected);                                                   \
        return failed_value;                                                                                           \
    } while (0)

#define ASSERT_SUCCESS_BLOCK(rc, block)                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        if (rc != CASS_OK)                                                                                             \
        {                                                                                                              \
            zend_throw_exception_ex(exception_class(rc), rc, "%s", cass_error_desc(rc));                     \
            block                                                                                                      \
        }                                                                                                              \
    } while (0)

#define ASSERT_SUCCESS(rc) ASSERT_SUCCESS_BLOCK(rc, return;)

#define ASSERT_SUCCESS_VALUE(rc, value) ASSERT_SUCCESS_BLOCK(rc, return value;)

#define PHP_DRIVER_DEFAULT_CONSISTENCY CASS_CONSISTENCY_LOCAL_ONE

#define PHP_DRIVER_DEFAULT_LOG PHP_DRIVER_NAME ".log"
#define PHP_DRIVER_DEFAULT_LOG_LEVEL "ERROR"

    PHP_INI_MH(OnUpdateLogLevel);
    PHP_INI_MH(OnUpdateLog);

#ifdef __cplusplus
}
#endif
