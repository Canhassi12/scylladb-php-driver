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
BEGIN_EXTERN_C()
zend_class_entry* php_driver_value_ce = NULL;

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_driver_value_methods[] = {
  PHP_ABSTRACT_ME(Value, type, arginfo_none)
  PHP_FE_END
};

void
php_driver_define_Value()
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\Value", php_driver_value_methods);
  php_driver_value_ce = zend_register_internal_class(&ce );
  php_driver_value_ce->ce_flags |= ZEND_ACC_INTERFACE;
}

END_EXTERN_C()