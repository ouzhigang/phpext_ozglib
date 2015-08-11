
#ifndef PHP_OZGLIB_CFG_H
#define PHP_OZGLIB_CFG_H

#ifdef WIN32
//定义flock（使用PHP内部函数）的参数
#define F_LOCK_SH 1 //要取得共享锁定（读取程序）
#define F_LOCK_EX 2 //要取得独占锁定（写入程序）
#define F_LOCK_UN 3 //要释放锁定（无论共享或独占）
#define F_LOCK_NB 4 //如果你不希望 flock() 在锁定时堵塞，则给 operation 加上 LOCK_NB
//定义flock（使用PHP内部函数）的参数 end
#endif

#define F_LOCK_NAME "./lock.tmp" //并发锁的临时文件路径（默认，如果php.ini设置了该路径，这个路径就会被覆盖）

//ozglib_rand_str函数的常量
#define OZGLIB_RAND_STR_LOWER 0x00000001
#define OZGLIB_RAND_STR_UPPER 0x00000002
#define OZGLIB_RAND_STR_NUMBER 0x00000003
#define OZGLIB_RAND_STR_LOWER_UPPER 0x00000004
#define OZGLIB_RAND_STR_UPPER_NUMBER 0x00000005
#define OZGLIB_RAND_STR_LOWER_NUMBER 0x00000006
#define OZGLIB_RAND_STR_ALL 0x00000007

#endif
