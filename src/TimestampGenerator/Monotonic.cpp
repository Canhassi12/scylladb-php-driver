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
zend_class_entry *php_driver_timestamp_gen_monotonic_ce = NULL;

static zend_function_entry php_driver_timestamp_gen_monotonic_methods[] = {
  PHP_FE_END
};

static zend_object_handlers php_driver_timestamp_gen_monotonic_handlers;

static void
php_driver_timestamp_gen_monotonic_free(php5to7_zend_object_free *object )
{
  php_driver_timestamp_gen *self = PHP5TO7_ZEND_OBJECT_GET(timestamp_gen, object);

  cass_timestamp_gen_free(self->gen);

  zend_object_std_dtor(&self->zval );
  PHP5TO7_MAYBE_EFREE(self);
}

static php5to7_zend_object
php_driver_timestamp_gen_monotonic_new(zend_class_entry *ce )
{
  php_driver_timestamp_gen *self = PHP5TO7_ZEND_OBJECT_ECALLOC(timestamp_gen, ce);

  self->gen = cass_timestamp_gen_monotonic_new();

  PHP5TO7_ZEND_OBJECT_INIT_EX(timestamp_gen, timestamp_gen_monotonic, self, ce);
}

void php_driver_define_TimestampGeneratorMonotonic()
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\TimestampGenerator\\Monotonic", php_driver_timestamp_gen_monotonic_methods);
  php_driver_timestamp_gen_monotonic_ce = zend_register_internal_class(&ce );
  zend_class_implements(php_driver_timestamp_gen_monotonic_ce , 1, php_driver_timestamp_gen_ce);
  php_driver_timestamp_gen_monotonic_ce->ce_flags     |= PHP5TO7_ZEND_ACC_FINAL;
  php_driver_timestamp_gen_monotonic_ce->create_object = php_driver_timestamp_gen_monotonic_new;

  memcpy(&php_driver_timestamp_gen_monotonic_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
}
END_EXTERN_C()