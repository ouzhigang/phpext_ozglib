##功能不定期增加中...


## 并发锁实现

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
ozglib_lock();

echo "update code";

ozglib_unlock();
