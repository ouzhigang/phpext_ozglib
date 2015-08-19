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
#include "ozglib_cfg.h"

#include "ozgcc/AES.h"
#include "ozgcc/Common.h"

#ifndef WIN32
#include <sys/file.h>

#endif

//直接调用PHP函数
struct phpfun_return_status
{
	bool is_success;
	zval *retval;
};
phpfun_return_status phpfun(const char* fun_name, zend_uint argc, zval* args[])
{
	zval *func;
	zval *retval;

	MAKE_STD_ZVAL(func);
	MAKE_STD_ZVAL(retval);
	
	ZVAL_STRING(func, fun_name, 1);
	
	phpfun_return_status return_status = { false, NULL };
	if (call_user_function(EG(function_table), NULL, func, retval, argc, args TSRMLS_CC) == SUCCESS)
	{
		return_status.is_success = true;
		return_status.retval = retval;
	}

	return return_status;
}
//直接调用PHP函数 end

//获取服务器路径
static char* get_server_path()
{
	zval **server_pp, **doc_root_pp;
	char *server_key = "_SERVER", *doc_root = "DOCUMENT_ROOT";
	if (zend_hash_find(&EG(symbol_table), server_key, strlen(server_key) + 1, (void**)&server_pp) == SUCCESS)
	{
		HashTable *server_ht = Z_ARRVAL_PP(server_pp);
		if (zend_hash_find(server_ht, doc_root, strlen(doc_root) + 1, (void**)&doc_root_pp) == SUCCESS)		
			return Z_STRVAL_PP(doc_root_pp);		
	}
	return NULL;
}
//获取服务器路径 end

//字符串分隔
zval* str_split(char* separator, char* str)
{
	zval* args[2];
	MAKE_STD_ZVAL(args[0]);
	MAKE_STD_ZVAL(args[1]);
	ZVAL_STRING(args[0], separator, 1);
	ZVAL_STRING(args[1], str, 1);
	phpfun_return_status return_status = phpfun("explode", 2, args);
	if (return_status.is_success)
		return return_status.retval;
	
	return NULL;
}
//字符串分隔 end

/* True global resources - no need for thread safety here */
static int le_ozglib;

/* {{{ ozglib_functions[]
 *
 * Every user visible function must have an entry in ozglib_functions[].
 */
const zend_function_entry ozglib_functions[] = {
	PHP_FE(ozglib_test, NULL)
	PHP_FE(ozglib_lock, NULL)
	PHP_FE(ozglib_unlock, NULL)
	PHP_FE(ozglib_page_count, NULL)
	PHP_FE(ozglib_rand_str, NULL)
	PHP_FE_END
};
/* }}} */

const zend_function_entry encrypt_AES_methods[] = {
	PHP_ME(encrypt_AES, encode, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(encrypt_AES, decode, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry FileUtility_methods[] = {
	PHP_ME(FileUtility, createDir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(FileUtility, moveFile, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)	
	PHP_ME(FileUtility, unlinkFile, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(FileUtility, copyDir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_ME(FileUtility, copyFile, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry db_IDBHelper_methods[] = {
	PHP_ABSTRACT_ME(db_IDBHelper, getResults, NULL)
	PHP_ABSTRACT_ME(db_IDBHelper, getRow, NULL)
	PHP_ABSTRACT_ME(db_IDBHelper, getVar, NULL)
	PHP_ABSTRACT_ME(db_IDBHelper, query, NULL)
	PHP_FE_END
};

const zend_function_entry db_PDOHelper_methods[] = {
	PHP_ME(db_PDOHelper, getResults, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(db_PDOHelper, getRow, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(db_PDOHelper, getVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(db_PDOHelper, query, NULL, ZEND_ACC_PUBLIC)
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

PHP_INI_BEGIN()
	PHP_INI_ENTRY("ozglib.lock_name", F_LOCK_NAME, PHP_INI_ALL, NULL)
    //STD_PHP_INI_ENTRY("ozglib.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ozglib_globals, ozglib_globals)
    //STD_PHP_INI_ENTRY("ozglib.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ozglib_globals, ozglib_globals)
PHP_INI_END()

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

zend_class_entry *encrypt_AES_ce, *FileUtility_ce, *db_IDBHelper_ce, *db_PDOHelper_ce;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ozglib)
{
	REGISTER_INI_ENTRIES();

	zend_class_entry encrypt_AES, FileUtility, db_IDBHelper, db_PDOHelper;

//php5.3以上支持namespace
#if PHP_VERSION_ID >= 50300
	//支持namespace
	INIT_NS_CLASS_ENTRY(encrypt_AES, "ozglib\\encrypt", "AES", encrypt_AES_methods);
	INIT_NS_CLASS_ENTRY(FileUtility, "ozglib", "FileUtility", FileUtility_methods);

	INIT_NS_CLASS_ENTRY(db_IDBHelper, "ozglib\\db", "IDBHelper", db_IDBHelper_methods);
	INIT_NS_CLASS_ENTRY(db_PDOHelper, "ozglib\\db", "PDOHelper", db_PDOHelper_methods);
#else
	//不支持namespace，加上Ozg的前缀
	INIT_CLASS_ENTRY(AES, "OzgLibEncryptAES", encrypt_AES_methods); //定义OzgLibEncryptAES类
	INIT_CLASS_ENTRY(FileUtility, "OzgLibFileUtility", FileUtility_methods); //定义OzgLibFileUtility类

	INIT_CLASS_ENTRY(db_IDBHelper, "OzgLibDbIDBHelper", db_IDBHelper_methods);
	INIT_CLASS_ENTRY(db_PDOHelper, "OzgLibDbPDOHelper", db_PDOHelper_methods);
#endif

	encrypt_AES_ce = zend_register_internal_class(&encrypt_AES TSRMLS_CC);
	FileUtility_ce = zend_register_internal_class(&FileUtility TSRMLS_CC);
	
	db_IDBHelper_ce = zend_register_internal_interface(&db_IDBHelper TSRMLS_CC);
	db_PDOHelper_ce = zend_register_internal_class(&db_PDOHelper TSRMLS_CC);
	zend_class_implements(db_PDOHelper_ce TSRMLS_CC, 1, db_IDBHelper_ce);

	//注册本模块的常量
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_LOWER", OZGLIB_RAND_STR_LOWER, CONST_PERSISTENT | CONST_CS);
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_UPPER", OZGLIB_RAND_STR_UPPER, CONST_PERSISTENT | CONST_CS);
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_NUMBER", OZGLIB_RAND_STR_NUMBER, CONST_PERSISTENT | CONST_CS);
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_LOWER_UPPER", OZGLIB_RAND_STR_LOWER_UPPER, CONST_PERSISTENT | CONST_CS);
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_UPPER_NUMBER", OZGLIB_RAND_STR_UPPER_NUMBER, CONST_PERSISTENT | CONST_CS);
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_LOWER_NUMBER", OZGLIB_RAND_STR_LOWER_NUMBER, CONST_PERSISTENT | CONST_CS);
	REGISTER_MAIN_LONG_CONSTANT("OZGLIB_RAND_STR_ALL", OZGLIB_RAND_STR_ALL, CONST_PERSISTENT | CONST_CS);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ozglib)
{
	UNREGISTER_INI_ENTRIES();

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

PHP_FUNCTION(ozglib_test)
{
	//测试代码

	RETURN_TRUE;
}

#ifdef WIN32
long lock_fp = 0;
#else
int lock_fp = -1;
#endif

PHP_FUNCTION(ozglib_lock)
{
	char* f_lock_name = INI_STR("ozglib.lock_name");

#ifdef WIN32
	//win32无flock函数，使用php内置函数来实现

	zval* args[2];
	MAKE_STD_ZVAL(args[0]);
	MAKE_STD_ZVAL(args[1]);
	ZVAL_STRING(args[0], f_lock_name, 1);
	ZVAL_STRING(args[1], "a+", 1);
	phpfun_return_status return_status = phpfun("fopen", 2, args);
	if (return_status.is_success)
	{
		lock_fp = Z_RESVAL_P(return_status.retval);

		zval* args2[2];
		MAKE_STD_ZVAL(args2[0]);
		MAKE_STD_ZVAL(args2[1]);
		ZVAL_RESOURCE(args2[0], lock_fp);
		ZVAL_LONG(args2[1], F_LOCK_EX);
		phpfun_return_status return_status2 = phpfun("flock", 2, args2);
		if (return_status2.is_success)
		{
			zval* args3[2];
			MAKE_STD_ZVAL(args3[0]);
			MAKE_STD_ZVAL(args3[1]);
			ZVAL_RESOURCE(args3[0], lock_fp);
			ZVAL_STRING(args3[1], "ok\r\n", 1);
			phpfun_return_status return_status3 = phpfun("fwrite", 2, args3);
			if (return_status3.is_success)
			{
				zval* args4[1];
				MAKE_STD_ZVAL(args4[0]);
				ZVAL_RESOURCE(args4[0], lock_fp);
				phpfun_return_status return_status4 = phpfun("fflush", 1, args4);
				if (return_status4.is_success)
				{
					zval_dtor(return_status4.retval);
					RETURN_TRUE;
				}

				zval_dtor(return_status3.retval);				
			}

			zval_dtor(return_status2.retval);
		}

		zval_dtor(return_status.retval);
	}		
#else
	//非win32下
	const char* tmp = "ok\r\n";
	lock_fp = open(f_lock_name, O_RDWR | O_CREAT, S_IRUSR);
	if (lock_fp != -1)
	{
		int res = flock(lock_fp, LOCK_EX);
		if (res == 0)
		{
			write(lock_fp, tmp, sizeof(tmp));
			RETURN_TRUE;
		}		
	}	
#endif

	RETURN_FALSE;
}

PHP_FUNCTION(ozglib_unlock)
{
	char* f_lock_name = INI_STR("ozglib.lock_name");

#ifdef WIN32
	if (lock_fp != 0)
	{
		//win32无flock函数，使用php内置函数来实现

		zval* args[2];
		MAKE_STD_ZVAL(args[0]);
		MAKE_STD_ZVAL(args[1]);
		ZVAL_RESOURCE(args[0], lock_fp);
		ZVAL_LONG(args[1], F_LOCK_UN);
		phpfun_return_status return_status = phpfun("flock", 2, args);
		if (return_status.is_success)
		{
			zval* args2[1];
			MAKE_STD_ZVAL(args2[0]);
			ZVAL_RESOURCE(args2[0], lock_fp);
			phpfun_return_status return_status2 = phpfun("fclose", 1, args2);
			if (return_status2.is_success)
			{
				zval_dtor(return_status2.retval);
				remove(f_lock_name);
			}

			zval_dtor(return_status.retval);
		}
	}
	lock_fp = 0;
#else
	//非win32下
	if (lock_fp != -1)
	{
		int res = flock(lock_fp, LOCK_UN);
		if (res == 0)
		{
			close(lock_fp);
			remove(f_lock_name);
		}
	}
	lock_fp = -1;
#endif
	
	RETURN_TRUE;
}

PHP_FUNCTION(ozglib_page_count)
{
	long count = NULL, page_size = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &count, &page_size) == FAILURE)
		RETURN_FALSE;
	
	if (count <= 0 || page_size <= 0)
	{
		RETURN_LONG(0);
	}

	if (count % page_size == 0)
	{
		RETURN_LONG(count / page_size);
	}
	else
	{
		RETURN_LONG((long)floor(count / page_size) + 1);
	}
}

PHP_FUNCTION(ozglib_rand_str)
{
	long type = NULL, len = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &type, &len) == FAILURE)
		RETURN_FALSE;

	const char* str1 = "abcdefghijklmnopqrstuvwxyz";
	const char* str2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* str3 = "0123456789";

	char str[128];

	switch (type)
	{
		case OZGLIB_RAND_STR_LOWER:
			sprintf(str, "%s", str1);
			break;
		case OZGLIB_RAND_STR_UPPER:
			sprintf(str, "%s", str2);
			break;
		case OZGLIB_RAND_STR_NUMBER:
			sprintf(str, "%s", str3);
			break;
		case OZGLIB_RAND_STR_LOWER_UPPER:
			sprintf(str, "%s%s", str1, str2);
			break;
		case OZGLIB_RAND_STR_UPPER_NUMBER:
			sprintf(str, "%s%s", str2, str3);
			break;
		case OZGLIB_RAND_STR_LOWER_NUMBER:
			sprintf(str, "%s%s", str1, str3);
			break;
		case OZGLIB_RAND_STR_ALL:
			sprintf(str, "%s%s%s", str1, str2, str3);
			break;
		default:
			RETURN_FALSE;
			break;
	}
	
	char *output = (char*)emalloc(len + 1);
	for (long i = 0; i < len; i++)
	{
		int j = rand() % strlen(str);
		output[i] = str[j];
	}

	output[len] = '\0';
	RETURN_STRING(output, 0);
}

//encrypt\AES
PHP_METHOD(encrypt_AES, encode)
{
	char *text;
	int text_len;
	char *key = ENCRYPT_AES_KEY; //默认key
	int key_len = sizeof(ENCRYPT_AES_KEY) - 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &text, &text_len, &key, &key_len) == FAILURE)
		RETURN_FALSE;

	char output[1024]; //暂定这个大小
	ozgcc::AES aes((unsigned char*)key);
	aes.Cipher(text, output); //加密
	
	RETURN_STRING(output, 1);
}

PHP_METHOD(encrypt_AES, decode)
{
	char *encode_str;
	int encode_str_len;
	char *key = ENCRYPT_AES_KEY; //默认key
	int key_len = sizeof(ENCRYPT_AES_KEY) - 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &encode_str, &encode_str_len, &key, &key_len) == FAILURE)
		RETURN_FALSE;

	char output[1024]; //暂定这个大小
	ozgcc::AES aes((unsigned char*)key);
	aes.InvCipher(encode_str, output); //解密

	RETURN_STRING(output, 1);
}
//encrypt\AES end

//FileUtility
PHP_METHOD(FileUtility, createDir)
{
	char *dir_path;
	int dir_path_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &dir_path, &dir_path_len) == FAILURE)
		RETURN_FALSE;
	
	dir_path = str_replace(dir_path, "\\", "/");
	
	char* tmp1 = str_append_nfree(get_server_path(), "/");
	char* tmp2 = str_append_nfree(tmp1, dir_path);
	char* all_dir_path = estrndup(tmp2, strlen(tmp2));

	//释放内存
	free(tmp1);
	free(tmp2);

	//PHPWRITE(all_dir_path, strlen(all_dir_path));

	dir_create(all_dir_path);
	
	RETURN_TRUE;
}

PHP_METHOD(FileUtility, moveFile)
{
	char *old_path;
	int old_path_len;
	char *new_path;
	int new_path_len;
	zend_bool over_write = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s|b", &old_path, &old_path_len, &new_path, &new_path_len, &over_write) == FAILURE)
		RETURN_FALSE;

	old_path = str_replace(old_path, "\\", "/");
	new_path = str_replace(new_path, "\\", "/");

	char* server_path = get_server_path();

	char* tmp1 = str_append_nfree(server_path, "/");
	char* tmp2 = str_append_nfree(tmp1, old_path);
	char* all_old_path = estrndup(tmp2, strlen(tmp2));
		
	char* tmp3 = str_append_nfree(server_path, "/");
	char* tmp4 = str_append_nfree(tmp3, new_path);
	char* all_new_path = estrndup(tmp4, strlen(tmp4));

	//释放内存
	free(tmp1);
	free(tmp2);
	free(tmp3);
	free(tmp4);

	bool over_write_b = over_write ? true : false;
	file_move(all_old_path, all_new_path, over_write_b);

	RETURN_TRUE;
}

PHP_METHOD(FileUtility, unlinkFile)
{
	char *file_path;
	int file_path_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &file_path_len) == FAILURE)
		RETURN_FALSE;

	file_path = str_replace(file_path, "\\", "/");

	char* tmp1 = str_append_nfree(get_server_path(), "/");
	char* tmp2 = str_append_nfree(tmp1, file_path);
	char* all_file_path = estrndup(tmp2, strlen(tmp2));

	//释放内存
	free(tmp1);
	free(tmp2);
	
	file_remove(all_file_path);

	RETURN_TRUE;
}

PHP_METHOD(FileUtility, copyDir)
{
	char *from_path;
	int from_path_len;
	char *to_path;
	int to_path_len;
	zend_bool over_write = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s|b", &from_path, &from_path_len, &to_path, &to_path_len, &over_write) == FAILURE)
		RETURN_FALSE;

	from_path = str_replace(from_path, "\\", "/");
	to_path = str_replace(to_path, "\\", "/");

	char* server_path = get_server_path();

	char* tmp1 = str_append_nfree(server_path, "/");
	char* tmp2 = str_append_nfree(tmp1, from_path);
	char* all_from_path = estrndup(tmp2, strlen(tmp2));

	char* tmp3 = str_append_nfree(server_path, "/");
	char* tmp4 = str_append_nfree(tmp3, to_path);
	char* all_to_path = estrndup(tmp4, strlen(tmp4));

	//释放内存
	free(tmp1);
	free(tmp2);
	free(tmp3);
	free(tmp4);

	bool over_write_b = over_write ? true : false;
	dir_copy(all_from_path, all_to_path, over_write_b);

	RETURN_TRUE;
}

PHP_METHOD(FileUtility, copyFile)
{
	char *from_path;
	int from_path_len;
	char *to_path;
	int to_path_len;
	zend_bool over_write = 1;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s|b", &from_path, &from_path_len, &to_path, &to_path_len, &over_write) == FAILURE)
		RETURN_FALSE;

	from_path = str_replace(from_path, "\\", "/");
	to_path = str_replace(to_path, "\\", "/");

	char* server_path = get_server_path();

	char* tmp1 = str_append_nfree(server_path, "/");
	char* tmp2 = str_append_nfree(tmp1, from_path);
	char* all_from_path = estrndup(tmp2, strlen(tmp2));

	char* tmp3 = str_append_nfree(server_path, "/");
	char* tmp4 = str_append_nfree(tmp3, to_path);
	char* all_to_path = estrndup(tmp4, strlen(tmp4));

	//释放内存
	free(tmp1);
	free(tmp2);
	free(tmp3);
	free(tmp4);

	bool over_write_b = over_write ? true : false;
	file_copy(all_from_path, all_to_path, over_write_b);

	RETURN_TRUE;
}

//FileUtility end

//db\PDOHelper
PHP_METHOD(db_PDOHelper, getResults)
{

	RETURN_FALSE;
}

PHP_METHOD(db_PDOHelper, getRow)
{

	RETURN_FALSE;
}

PHP_METHOD(db_PDOHelper, getVar)
{

	RETURN_FALSE;
}

PHP_METHOD(db_PDOHelper, query)
{

	RETURN_FALSE;
}
//db\PDOHelper end

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
