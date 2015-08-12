##功能不定期增加中...

windows环境下使用vs2015编译通过。

需要设置项目属性：

1、配置属性 - C/C++ - 附加包含目录 - 修改对应的PHP源文件目录。

2、配置属性 - 链接器 - 输入 - 附加依赖项 - 修改对应的Windows版PHP NTS目录（TS版本需要在预处理器的最后加上ZTS）。

3、将PHP源代码\win32\build\config.w32.h.in复制到E:\php-5.6.9-src\main里，然后去掉后面的”.in“，打开此文件然后在顶部加入#define PHP_COMPILER_ID "VC11"。

4、PHP源代码\Zend\zend_config.w32.h找到#define snprintf _snprintf，然后注释掉（vs2015需要执行这步）。

php.ini可设置锁定文件的路径

[ozglib]

ozglib.lock_name="D:/test.tmp"

~~~~~~~~~~php
<?php
//并发锁
if(ozglib_lock()) {
	echo "update code";
}
ozglib_unlock();

//获取总页数，参数1为数据总数，参数2为每页显示数
$page_count = ozglib_page_count(11, 5);

//获取随机字符串，参数1为字符类型，参数2为返回字符数
//字符类型:
//OZGLIB_RAND_STR_LOWER
//OZGLIB_RAND_STR_UPPER
//OZGLIB_RAND_STR_NUMBER
//OZGLIB_RAND_STR_LOWER_UPPER
//OZGLIB_RAND_STR_UPPER_NUMBER
//OZGLIB_RAND_STR_LOWER_NUMBER
//OZGLIB_RAND_STR_ALL
$rand_str = ozglib_rand_str(OZGLIB_RAND_STR_ALL, 10);

//AES加密
//Encrypt::encode为加密方法，参数1为加密前的字符串，参数2为key（默认值在ozglib_cfg.h里面设置）
//Encrypt::decode为解密方法，参数1为加密后的字符串，参数2为key（默认值在ozglib_cfg.h里面设置）
use \ozglib\Encrypt;
$str = "ozglib";
$str_encode = Encrypt::encode($str);
$str_decode = Encrypt::decode($str_encode);
var_dump($str_encode);
var_dump($str_decode);
