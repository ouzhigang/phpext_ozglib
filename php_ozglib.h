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

#ifndef PHP_OZGLIB_H
#define PHP_OZGLIB_H

extern zend_module_entry ozglib_module_entry;
#define phpext_ozglib_ptr &ozglib_module_entry

#define PHP_OZGLIB_VERSION "20150728" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_OZGLIB_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_OZGLIB_API __attribute__ ((visibility("default")))
#else
#	define PHP_OZGLIB_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(ozglib);
PHP_MSHUTDOWN_FUNCTION(ozglib);
PHP_RINIT_FUNCTION(ozglib);
PHP_RSHUTDOWN_FUNCTION(ozglib);
PHP_MINFO_FUNCTION(ozglib);

//²¢·¢Ëø
PHP_FUNCTION(ozg_lock);
PHP_FUNCTION(ozg_unlock);

PHP_METHOD(OzgCache, get);
PHP_METHOD(OzgCache, set);
PHP_METHOD(OzgCache, remove);

PHP_METHOD(OzgFileCache, get);
PHP_METHOD(OzgFileCache, set);
PHP_METHOD(OzgFileCache, remove);

PHP_METHOD(OzgMemCache, get);
PHP_METHOD(OzgMemCache, set);
PHP_METHOD(OzgMemCache, remove);

PHP_METHOD(OzgRedisCache, get);
PHP_METHOD(OzgRedisCache, set);
PHP_METHOD(OzgRedisCache, remove);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(ozglib)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(ozglib)
*/

/* In every utility function you add that needs to use variables 
   in php_ozglib_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as OZGLIB_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define OZGLIB_G(v) TSRMG(ozglib_globals_id, zend_ozglib_globals *, v)
#else
#define OZGLIB_G(v) (ozglib_globals.v)
#endif

#endif	/* PHP_OZGLIB_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */


