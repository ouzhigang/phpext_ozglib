##功能不定期增加中...


## 并发锁实现

windows环境下使用vs2015编译通过。需要设置项目属性：配置属性 - C/C++ - 附加包含目录 - 修改对应的PHP源文件目录，配置属性 - 链接器 - 输入 - 附加依赖项 - 修改对应的Windows版PHP NTS目录（TS版本需要在预处理器的最后加上ZTS）

php.ini可设置锁定文件的路径

[ozglib]

ozglib.lock_name="D:/test.tmp"


~~~~~~~~~~php
<?php
ozg_lock();

echo "update code";

ozg_unlock();
