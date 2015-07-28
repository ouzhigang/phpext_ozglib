/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ozglib.h"

/* If you declare any globals in php_ozglib.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ozglib)
*/

/* True global resources - no need for thread safety here */
static int le_ozglib;

/* {{{ ozglib_functions[]
 *
 * Every user visible function must have an entry in ozglib_functions[].
 */
const zend_function_entry ozglib_functions[] = {
	PHP_FE(ozg_lock, NULL)
	PHP_FE(ozg_unlock, NULL)
	PHP_FE_END
};
/* }}} */

const zend_function_entry OzgCache_methods[] = {
	PHP_ME(OzgCache, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgCache, set, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgCache, remove, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry OzgFileCache_methods[] = {
	PHP_ME(OzgFileCache, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgFileCache, set, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgFileCache, remove, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry OzgMemCache_methods[] = {
	PHP_ME(OzgFileCache, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgFileCache, set, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgFileCache, remove, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry OzgRedisCache_methods[] = {
	PHP_ME(OzgFileCache, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgFileCache, set, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(OzgFileCache, remove, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* {{{ ozglib_module_entry
 */
zend_module_entry ozglib_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"ozglib",
	ozglib_functions,
	PHP_MINIT(ozglib),
	PHP_MSHUTDOWN(ozglib),
	PHP_RINIT(ozglib),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(ozglib),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ozglib),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_OZGLIB_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_OZGLIB
ZEND_GET_MODULE(ozglib)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ozglib.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ozglib_globals, ozglib_globals)
    STD_PHP_INI_ENTRY("ozglib.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ozglib_globals, ozglib_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_ozglib_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ozglib_init_globals(zend_ozglib_globals *ozglib_globals)
{
	ozglib_globals->global_value = 0;
	ozglib_globals->global_string = NULL;
}
*/
/* }}} */

zend_class_entry *OzgCache_ce, *OzgFileCache_ce, *OzgMemCache_ce, *OzgRedisCache_ce;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ozglib)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

	zend_class_entry OzgCache, OzgFileCache, OzgMemCache, OzgRedisCache;
	
	//定义OzgCache的interface
	INIT_CLASS_ENTRY(OzgCache, "OzgCache", OzgCache_methods);
	OzgCache_ce = zend_register_internal_interface(&OzgCache TSRMLS_CC);

	//文件缓存
	INIT_CLASS_ENTRY(OzgFileCache, "OzgFileCache", OzgFileCache_methods);
	OzgFileCache_ce = zend_register_internal_class(&OzgFileCache TSRMLS_CC);
	zend_class_implements(OzgFileCache_ce TSRMLS_CC, 1, OzgCache_ce);

	//memcache
	INIT_CLASS_ENTRY(OzgMemCache, "OzgMemCache", OzgMemCache_methods);
	OzgMemCache_ce = zend_register_internal_class(&OzgMemCache TSRMLS_CC);
	zend_class_implements(OzgMemCache_ce TSRMLS_CC, 1, OzgCache_ce);

	//redis
	INIT_CLASS_ENTRY(OzgMemCache, "OzgMemCache", OzgMemCache_methods);
	OzgMemCache_ce = zend_register_internal_class(&OzgMemCache TSRMLS_CC);
	zend_class_implements(OzgMemCache_ce TSRMLS_CC, 1, OzgCache_ce);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ozglib)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ozglib)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ozglib)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ozglib)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ozglib support", "enabled");
	php_info_print_table_row(2, "ozglib version", PHP_OZGLIB_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

PHP_FUNCTION(ozg_lock)
{
	
	RETURN_TRUE;
}

PHP_FUNCTION(ozg_unlock)
{

	RETURN_TRUE;
}

//OzgCache
PHP_METHOD(OzgCache, set)
{

}

PHP_METHOD(OzgCache, get)
{

}

PHP_METHOD(OzgCache, remove)
{

}
//OzgCache end

//OzgFileCache
PHP_METHOD(OzgFileCache, set)
{

}

PHP_METHOD(OzgFileCache, get)
{

}

PHP_METHOD(OzgFileCache, remove)
{

}
//OzgFileCache end

//OzgMemCache
PHP_METHOD(OzgMemCache, set)
{

}

PHP_METHOD(OzgMemCache, get)
{

}

PHP_METHOD(OzgMemCache, remove)
{

}
//OzgMemCache end

//OzgRedisCache
PHP_METHOD(OzgRedisCache, set)
{

}

PHP_METHOD(OzgRedisCache, get)
{

}

PHP_METHOD(OzgRedisCache, remove)
{

}
//OzgRedisCache end

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
