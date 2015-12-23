#ifndef PHP_CASSANDRA_H
#define PHP_CASSANDRA_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gmp.h>
#include <cassandra.h>

/* Ensure Visual Studio 2010 does not load MSVC++ stdint definitions */
#ifdef _WIN32
#  ifdef DISABLE_MSVC_STDINT
#    pragma once
#    ifndef _STDINT
#      define _STDINT
#    endif
#  endif
#endif

#include <php.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#if PHP_VERSION_ID < 50304
#  error PHP 5.3.4 or later is required in order to build the driver
#endif

#if HAVE_SPL
#  include <ext/spl/spl_iterators.h>
#  include <ext/spl/spl_exceptions.h>
#else
#  error SPL must be enabled in order to build the driver
#endif

#include "version.h"

/* Resources */
#define PHP_CASSANDRA_CLUSTER_RES_NAME    "Cassandra Cluster"
#define PHP_CASSANDRA_SESSION_RES_NAME    "Cassandra Session"

extern zend_module_entry cassandra_module_entry;
#define phpext_cassandra_ptr &cassandra_module_entry

#ifdef PHP_WIN32
#  define PHP_CASSANDRA_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_CASSANDRA_API __attribute__ ((visibility("default")))
#else
#  define PHP_CASSANDRA_API
#endif

#ifndef ZEND_MOD_END
#  define ZEND_MOD_END {NULL, NULL, NULL}
#endif

#ifndef PHP_FE_END
#  define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

#if ZEND_MODULE_API_NO < 20100525
#  define object_properties_init(value, class_entry) \
              zend_hash_copy(*value.properties, &class_entry->default_properties, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
#endif

#define SAFE_STR(a) ((a)?a:"")

#ifdef ZTS
#  include "TSRM.h"
#endif

#if PHP_MAJOR_VERSION >= 7
#define php5to7_zend_register_internal_class_ex(ce, parent_ce) zend_register_internal_class_ex((ce), (parent_ce) TSRMLS_CC);

typedef zval php5to7_zval;
typedef zval *php5to7_zval_args;
typedef zval *php5to7_zval_arg;
typedef zend_string *php5to7_string;
typedef zend_ulong php5to7_ulong;
typedef zval php5to7_zend_resource_le;
typedef zend_resource* php5to7_zend_resource;
typedef zend_object *php5to7_zend_object;
typedef zend_object php5to7_zend_object_free;
typedef zval **php5to7_zval_gc;

#define PHP5TO7_SMART_STR_INIT { NULL, 0 }
#define PHP5TO7_SMART_STR_VAL(ss) (ss).s->val

#define PHP5TO7_STRCMP(s, c) strcmp((s)->val, (c))
#define PHP5TO7_STRVAL(s) ((s)->val)

#define PHP5TO7_ZEND_ACC_FINAL ZEND_ACC_FINAL

#define PHP5TO7_ZEND_OBJECT_ECALLOC(type_name, ce) (cassandra_##type_name *) \
  ecalloc(1, sizeof(cassandra_##type_name) + zend_object_properties_size(ce))

#define PHP5TO7_ZEND_OBJECT_INIT(type_name, self, ce) \
  PHP5TO7_ZEND_OBJECT_INIT_EX(type_name, type_name, self, ce)

#define PHP5TO7_ZEND_OBJECT_INIT_EX(type_name, name, self, ce) do {                  \
  zend_object_std_init(&self->zval, ce TSRMLS_CC);                                   \
  cassandra_##name##_handlers.offset = XtOffsetOf(cassandra_##type_name, zval);      \
  cassandra_##name##_handlers.free_obj = php_cassandra_##name##_free;      \
  self->zval.handlers = &cassandra_##name##_handlers;                           \
  return &self->zval;                                                                \
} while(0)

#define PHP5TO7_ZEND_OBJECT_MAYBE_EFREE(self) ((void)0)

#define PHP5TO7_ZEND_HASH_GET_CURRENT_DATA(ht, res) \
  ((res = zend_hash_get_current_data((ht))) != NULL)

#define PHP5TO7_ZEND_HASH_GET_CURRENT_KEY(ht, str_index, num_index) \
  zend_hash_get_current_key((ht), (str_index), (num_index))

#define PHP5TO7_ZEND_HASH_EXISTS(ht, key, len) \
  zend_hash_str_exists((ht), (key), (len))

#define PHP5TO7_ZEND_HASH_FIND(ht, key, len, res) \
  ((res = zend_hash_str_find((ht), (key), (size_t)(len - 1))) != NULL)

#define PHP5TO7_ZEND_HASH_INDEX_FIND(ht, index, res) \
  ((res = zend_hash_index_find((ht), (php5to7_ulong) (index))) != NULL)

#define PHP5TO7_ZEND_HASH_UPDATE(ht, key, len, val, val_size) \
  ((zend_hash_str_update((ht), (key), (size_t)(len - 1), (val))) != NULL)

#define PHP5TO7_ZEND_HASH_ADD(ht, key, len, val, val_size) \
  (zend_hash_str_add((ht), (key), (len), (val)) != NULL)

#define PHP5TO7_ZEND_HASH_DEL(ht, key, len) \
  ((zend_hash_str_del((ht), (key), (size_t)(len - 1))) == SUCCESS)

#define PHP5TO7_ZEND_STRING_VAL(str) (str)->val
#define PHP5TO7_ZEND_STRING_LEN(str) (str)->len

#define PHP5TO7_ZVAL_COPY(zv1, zv2) ZVAL_COPY(zv1, zv2)
#define PHP5TO7_ZVAL_IS_UNDEF(zv) Z_ISUNDEF(zv)

#define PHP5TO7_ZVAL_IS_BOOL_P(zv) \
  (Z_TYPE_P(zv) == IS_TRUE  || Z_TYPE_P(zv) == IS_FALSE)
#define PHP5TO7_ZVAL_IS_FALSE_P(zv) (Z_TYPE_P(zv) == IS_FALSE)
#define PHP5TO7_ZVAL_IS_TRUE_P(zv) (Z_TYPE_P(zv) == IS_TRUE)

#define PHP5TO7_ZVAL_UNDEF(zv) ZVAL_UNDEF(&(zv));
#define PHP5TO7_ZVAL_MAYBE_MAKE(zv) ((void)0)
#define PHP5TO7_ZVAL_MAYBE_DESTROY(zv) do { \
  if (Z_ISUNDEF(zv)) {                      \
    zval_ptr_dtor(&(zv));                   \
    ZVAL_UNDEF(&(zv));                      \
  }                                         \
} while(0)

#define PHP5TO7_ZVAL_STRING(zv, s) ZVAL_STRING(zv, s)
#define PHP5TO7_ZVAL_STRINGL(zv, s, len) ZVAL_STRINGL(zv, s, len)
#define PHP5TO7_RETURN_STRING(s) RETURN_STRING(s)
#define PHP5TO7_RETURN_STRINGL(s, len) RETURN_STRINGL(s, len)

#define PHP5TO7_ZVAL_ARG(zv) &(zv)
#define PHP5TO7_ZVAL_MAYBE_DEREF(zv) (zv)
#define PHP5TO7_ZVAL_MAYBE_ADDR_OF(zv) (zv)
#define PHP5TO7_ZVAL_MAYBE_P(zv) &(zv)
#define PHP5TO7_Z_TYPE_MAYBE_P(zv) Z_TYPE(zv)
#define PHP5TO7_Z_ARRVAL_MAYBE_P(zv) Z_ARRVAL(zv)
#define PHP5TO7_Z_OBJCE_MAYBE_P(zv) Z_OBJCE(zv)
#define PHP5TO7_Z_LVAL_MAYBE_P(zv) Z_LVAL(zv)
#define PHP5TO7_Z_DVAL_MAYBE_P(zv) Z_DVAL(zv)
#define PHP5TO7_Z_STRVAL_MAYBE_P(zv) Z_STRVAL(zv)
#define PHP5TO7_Z_STRLEN_MAYBE_P(zv) Z_STRLEN(zv)

#else
typedef zval *php5to7_zval;
typedef zval ***php5to7_zval_args;
typedef char *php5to7_string;
typedef ulong php5to7_ulong;
typedef zend_rsrc_list_entry php5to7_zend_resource_le;
typedef zend_rsrc_list_entry *php5to7_zend_resource;
typedef zend_object_value php5to7_zend_object;
typedef void php5to7_zend_object_free;
typedef zval ***php5to7_zval_gc;

#define PHP5TO7_SMART_STR_INIT { NULL, 0, 0 }
#define PHP5TO7_SMART_STR_VAL(ss) (ss).c

#define PHP5TO7_STRCMP(s, c) strcmp((s), (c))
#define PHP5TO7_STRVAL(s) (s)

#define PHP5TO7_ZEND_ACC_FINAL ZEND_ACC_FINAL_CLASS

#define PHP5TO7_ZEND_OBJECT_ECALLOC(type_name, ce) (cassandra_##type_name *) \
  ecalloc(1, sizeof(cassandra_##type_name))

#define PHP5TO7_ZEND_OBJECT_INIT(type_name, self, ce) \
  PHP5TO7_ZEND_OBJECT_INIT_EX(type_name, type_name, self, ce)

#define PHP5TO7_ZEND_OBJECT_INIT_EX(type_name, name, self, ce) do {                                 \
  zend_object_value retval;                                                                         \
  zend_object_std_init(&self->zval, ce TSRMLS_CC);                                                  \
  object_properties_init(&self->zval, ce);                                                          \
  retval.handle   = zend_objects_store_put(self,                                                    \
                                           (zend_objects_store_dtor_t) zend_objects_destroy_object, \
                                           php_cassandra_##name##_free, NULL TSRMLS_CC);       \
  retval.handlers = &cassandra_##name##_handlers;                                              \
  return retval;                                                                                    \
} while(0)

#define PHP5TO7_ZEND_OBJECT_MAYBE_EFREE(self) efree((void *)self)


#define PHP5TO7_ZEND_HASH_GET_CURRENT_DATA(ht, res) \
  (zend_hash_get_current_data((ht), (void **) &(res)) == SUCCESS)

#define PHP5TO7_ZEND_HASH_GET_CURRENT_KEY(ht, str_index, num_index) \
  zend_hash_get_current_key((ht), (str_index), (num_index), 0)

#define PHP5TO7_ZEND_HASH_EXISTS(ht, key, len) \
  zend_hash_exists((ht), (key), (len))

#define PHP5TO7_ZEND_HASH_FIND(ht, key, len, res) \
  (zend_hash_find((ht), (key), (uint)(len), (void **)&(res)) == SUCCESS)

#define PHP5TO7_ZEND_HASH_INDEX_FIND(ht, index, res) \
  (zend_hash_index_find((ht), (php5to7_ulong) (index), (void **) &res) == SUCCESS)

#define PHP5TO7_ZEND_HASH_UPDATE(ht, key, len, val, val_size) \
  (zend_hash_update((ht), (key), (uint)(len), (void *)(val), (uint)(val_size), NULL) == SUCCESS)

#define PHP5TO7_ZEND_HASH_ADD(ht, key, len, val, val_size) \
  (zend_hash_add((ht), (key), (len), (void *)(val), (uint)(val_size), NULL) == SUCCESS)

#define PHP5TO7_ZEND_HASH_DEL(ht, key, len) \
  ((zend_hash_del((ht), (key), (uint)(len))) == SUCCESS)


#define php5to7_zend_register_internal_class_ex(ce, parent_ce) zend_register_internal_class_ex((ce), (parent_ce), NULL TSRMLS_CC);

#define PHP5TO7_ZVAL_COPY(zv1, zv2) do { \
  zv1 = zv2;                             \
  Z_ADDREF_P(zv2);                       \
} while(0)

#define PHP5TO7_ZVAL_IS_UNDEF(zv) ((zv) == NULL)
#define PHP5TO7_ZVAL_IS_BOOL_P(zv) (Z_TYPE_P(zv) == IS_BOOL)
#define PHP5TO7_ZVAL_IS_FALSE_P(zv) (Z_TYPE_P(zv) == IS_BOOL && !Z_BVAL_P(zv))
#define PHP5TO7_ZVAL_IS_TRUE_P(zv) (Z_TYPE_P(zv) == IS_BOOL && Z_BVAL_P(zv))

#define PHP5TO7_ZVAL_UNDEF(zv) (zv) = NULL;
#define PHP5TO7_ZVAL_MAYBE_MAKE(zv) MAKE_STD_ZVAL(zv)
#define PHP5TO7_ZVAL_MAYBE_DESTROY(zv) do { \
  if ((zv) != NULL) {                       \
    zval_ptr_dtor(&(zv));                   \
    (zv) = NULL;                            \
  }                                         \
} while(0)

#define Z_RES_P(zv) (zv)
#define Z_RES(zv) (&(zv))

#define PHP5TO7_ZVAL_STRING(zv, s) ZVAL_STRING(zv, s, 1)
#define PHP5TO7_ZVAL_STRINGL(zv, s, len) ZVAL_STRINGL(zv, s, len, 1)
#define PHP5TO7_RETURN_STRING(s) RETURN_STRING(s, 1)
#define PHP5TO7_RETURN_STRINGL(s, len) RETURN_STRINGL(s, len, 1)

#define PHP5TO7_ZVAL_ARG(zv) *(zv)
#define PHP5TO7_ZVAL_MAYBE_DEREF(zv) *(zv)
#define PHP5TO7_ZVAL_MAYBE_ADDR_OF(zv) &(zv)
#define PHP5TO7_ZVAL_MAYBE_P(zv) (zv)
#define PHP5TO7_Z_TYPE_MAYBE_P(zv) Z_TYPE_P(zv)
#define PHP5TO7_Z_ARRVAL_MAYBE_P(zv) Z_ARRVAL_P(zv)
#define PHP5TO7_Z_OBJCE_MAYBE_P(zv) Z_OBJCE_P(zv)
#define PHP5TO7_Z_LVAL_MAYBE_P(zv) Z_LVAL_P(zv)
#define PHP5TO7_Z_DVAL_MAYBE_P(zv) Z_DVAL_P(zv)
#define PHP5TO7_Z_STRVAL_MAYBE_P(zv) Z_STRVAL_P(zv)
#define PHP5TO7_Z_STRLEN_MAYBE_P(zv) Z_STRLEN_P(zv)

#endif


zend_class_entry *exception_class(CassError rc);

void throw_invalid_argument(zval *object,
                            const char *object_name,
                            const char *expected_type TSRMLS_DC);

#define INVALID_ARGUMENT(object, expected) \
{ \
  throw_invalid_argument(object, #object, expected TSRMLS_CC); \
  return; \
}

#define INVALID_ARGUMENT_VALUE(object, expected, failed_value) \
{ \
  throw_invalid_argument(object, #object, expected TSRMLS_CC); \
  return failed_value; \
}

#define ASSERT_SUCCESS_BLOCK(rc, block) \
{ \
  if (rc != CASS_OK) { \
    zend_throw_exception_ex(exception_class(rc), rc TSRMLS_CC, \
                            "%s", cass_error_desc(rc)); \
    block \
  } \
}

#define ASSERT_SUCCESS(rc) ASSERT_SUCCESS_BLOCK(rc, return;)

#define ASSERT_SUCCESS_VALUE(rc, value) ASSERT_SUCCESS_BLOCK(rc, return value;)

#define CPP_DRIVER_VERSION(major, minor, patch) \
  (((major) << 16) + ((minor) << 8) + (patch))

#define CURRENT_CPP_DRIVER_VERSION \
  CPP_DRIVER_VERSION(CASS_VERSION_MAJOR, CASS_VERSION_MINOR, CASS_VERSION_PATCH)

#include "php_cassandra_types.h"

PHP_MINIT_FUNCTION(cassandra);
PHP_MSHUTDOWN_FUNCTION(cassandra);
PHP_RINIT_FUNCTION(cassandra);
PHP_RSHUTDOWN_FUNCTION(cassandra);
PHP_MINFO_FUNCTION(cassandra);

ZEND_BEGIN_MODULE_GLOBALS(cassandra)
  CassUuidGen          *uuid_gen;
  unsigned int          persistent_clusters;
  unsigned int          persistent_sessions;
  php5to7_zval          type_varchar;
  php5to7_zval          type_text;
  php5to7_zval          type_blob;
  php5to7_zval          type_ascii;
  php5to7_zval          type_bigint;
  php5to7_zval          type_counter;
  php5to7_zval          type_int;
  php5to7_zval          type_varint;
  php5to7_zval          type_boolean;
  php5to7_zval          type_decimal;
  php5to7_zval          type_double;
  php5to7_zval          type_float;
  php5to7_zval          type_inet;
  php5to7_zval          type_timestamp;
  php5to7_zval          type_uuid;
  php5to7_zval          type_timeuuid;
ZEND_END_MODULE_GLOBALS(cassandra)

#ifdef ZTS
#  define CASSANDRA_G(v) TSRMG(cassandra_globals_id, zend_cassandra_globals *, v)
#else
#  define CASSANDRA_G(v) (cassandra_globals.v)
#endif

#endif /* PHP_CASSANDRA_H */
