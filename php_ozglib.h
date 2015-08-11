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

#define PHP_OZGLIB_VERSION "20150810" /* Replace with version number for your extension */

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

//并发锁
PHP_FUNCTION(ozglib_lock);
PHP_FUNCTION(ozglib_unlock);

//公用函数
PHP_FUNCTION(ozglib_page_count);
PHP_FUNCTION(ozglib_rand_str);

//可逆加密
PHP_METHOD(Encrypt, encode);
PHP_METHOD(Encrypt, decode);

//文件操作
PHP_METHOD(FileUtility, createDir);
PHP_METHOD(FileUtility, createFile);
PHP_METHOD(FileUtility, moveDir);
PHP_METHOD(FileUtility, moveFile);
PHP_METHOD(FileUtility, unlinkDir);
PHP_METHOD(FileUtility, unlinkFile);
PHP_METHOD(FileUtility, copyDir);
PHP_METHOD(FileUtility, copyFile);
PHP_METHOD(FileUtility, getDirList);

/*ZEND_BEGIN_MODULE_GLOBALS(ozglib)
	
ZEND_END_MODULE_GLOBALS(ozglib)*/

/* In every utility function you add that needs to use variables 
   in php_ozglib_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as OZGLIB_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#endif	/* PHP_OZGLIB_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */


