/**
 * Copyright 2015-2017 DataStax, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "php_driver.h"
#include "php_driver_types.h"
#include "util/types.h"
BEGIN_EXTERN_C()
zend_class_entry *php_driver_type_scalar_ce = NULL;

PHP_METHOD(TypeScalar, __construct)
{
  zend_throw_exception_ex(php_driver_logic_exception_ce, 0 ,
    "Instantiation of a " PHP_DRIVER_NAMESPACE "\\Type\\Scalar objects directly is not " \
    "supported, call varchar(), text(), blob(), ascii(), bigint(), " \
    "smallint(), tinyint(), counter(), int(), varint(), boolean(), " \
    "decimal(), double(), float(), inet(), timestamp(), uuid(), timeuuid(), " \
    "map(), collection() or set() on " PHP_DRIVER_NAMESPACE "\\Type statically instead."
  );
  return;
}

PHP_METHOD(TypeScalar, name)
{
  php_driver_type *self;
  const char *name;

  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_TYPE(getThis());
  name = php_driver_scalar_type_name(self->type );
  PHP5TO7_RETVAL_STRING(name);
}

PHP_METHOD(TypeScalar, __toString)
{
  php_driver_type *self;
  const char *name;

  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  self = PHP_DRIVER_GET_TYPE(getThis());
  name = php_driver_scalar_type_name(self->type );
  PHP5TO7_RETVAL_STRING(name);
}

PHP_METHOD(TypeScalar, create)
{
  php_driver_scalar_init(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

#if PHP_VERSION_ID >= 80200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()
#else
#define arginfo_tostring arginfo_none
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_value, 0, ZEND_RETURN_VALUE, 0)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

static zend_function_entry php_driver_type_scalar_methods[] = {
  PHP_ME(TypeScalar, __construct, arginfo_none,  ZEND_ACC_PRIVATE)
  PHP_ME(TypeScalar, name,        arginfo_none,  ZEND_ACC_PUBLIC)
  PHP_ME(TypeScalar, __toString,  arginfo_tostring,  ZEND_ACC_PUBLIC)
  PHP_ME(TypeScalar, create,      arginfo_value, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

static zend_object_handlers php_driver_type_scalar_handlers;

static HashTable *
php_driver_type_scalar_gc(
#if PHP_MAJOR_VERSION >= 8
        zend_object *object,
#else
        zval *object,
#endif
        php5to7_zval_gc table, int *n
)
{
  *table = NULL;
  *n = 0;
  return zend_std_get_properties(object );
}

static HashTable *
php_driver_type_scalar_properties(
#if PHP_MAJOR_VERSION >= 8
        zend_object *object
#else
        zval *object
#endif
)
{
  php5to7_zval name;
#if PHP_MAJOR_VERSION >= 8
  php_driver_type *self  = PHP5TO7_ZEND_OBJECT_GET(type, object);
#else
  php_driver_type *self  = PHP_DRIVER_GET_TYPE(object);
#endif
  HashTable      *props = zend_std_get_properties(object );

  /* Used for comparison and 'text' is just an alias for 'varchar' */
  CassValueType type = self->type == CASS_VALUE_TYPE_TEXT
                     ? CASS_VALUE_TYPE_VARCHAR
                     : self->type;

  PHP5TO7_ZVAL_MAYBE_MAKE(name);
  PHP5TO7_ZVAL_STRING(PHP5TO7_ZVAL_MAYBE_P(name),
                      php_driver_scalar_type_name(type ));
  PHP5TO7_ZEND_HASH_UPDATE(props,
                           "name", sizeof("name"),
                           PHP5TO7_ZVAL_MAYBE_P(name), sizeof(zval));
  return props;
}

static int
php_driver_type_scalar_compare(zval *obj1, zval *obj2 )
{
#if PHP_MAJOR_VERSION >= 8
  ZEND_COMPARE_OBJECTS_FALLBACK(obj1, obj2);
#endif
  php_driver_type* type1 = PHP_DRIVER_GET_TYPE(obj1);
  php_driver_type* type2 = PHP_DRIVER_GET_TYPE(obj2);

  return php_driver_type_compare(type1, type2 );
}

static void
php_driver_type_scalar_free(php5to7_zend_object_free *object )
{
  php_driver_type *self = PHP5TO7_ZEND_OBJECT_GET(type, object);

  if (self->data_type) cass_data_type_free(self->data_type);

  zend_object_std_dtor(&self->zval );
  PHP5TO7_MAYBE_EFREE(self);
}

static php5to7_zend_object
php_driver_type_scalar_new(zend_class_entry *ce )
{
  php_driver_type *self = PHP5TO7_ZEND_OBJECT_ECALLOC(type, ce);

  self->type = CASS_VALUE_TYPE_UNKNOWN;
  self->data_type = NULL;

  PHP5TO7_ZEND_OBJECT_INIT_EX(type, type_scalar, self, ce);
}

void php_driver_define_TypeScalar()
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\Type\\Scalar", php_driver_type_scalar_methods);
  php_driver_type_scalar_ce = php5to7_zend_register_internal_class_ex(&ce, php_driver_type_ce);
  memcpy(&php_driver_type_scalar_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  php_driver_type_scalar_handlers.get_properties  = php_driver_type_scalar_properties;
#if PHP_VERSION_ID >= 50400
  php_driver_type_scalar_handlers.get_gc          = php_driver_type_scalar_gc;
#endif
#if PHP_MAJOR_VERSION >= 8
  php_driver_type_scalar_handlers.compare = php_driver_type_scalar_compare;
#else
  php_driver_type_scalar_handlers.compare_objects = php_driver_type_scalar_compare;
#endif
  php_driver_type_scalar_ce->ce_flags     |= PHP5TO7_ZEND_ACC_FINAL;
  php_driver_type_scalar_ce->create_object = php_driver_type_scalar_new;
}
END_EXTERN_C()