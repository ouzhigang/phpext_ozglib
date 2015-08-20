#ifndef __ozgcc__Common__
#define __ozgcc__Common__

#include <stdio.h>

void _ozgtest(const char* test_str);

//后缀为nfree的函数需要手工释放，nfree == not nfree

char* str_replace(char* src, char* sub, char* dst); //替换字符串
char* str_append_nfree(char* src_str, char* str); //字符串追加

void dir_create(char* dir_path); //生成文件夹，父目录不存在会自动建立
void dir_copy(char* from_path, char* to_path, bool over_write); //复制文件夹

bool file_exist(char* file_path); //检查文件、文件夹是否存在
void file_move(char* old_path, char* new_path, bool over_write); //移动文件、文件夹
void file_remove(char* file_path); //删除文件、文件夹
void file_copy(char* from_path, char* to_path, bool over_write); //复制文件

char* ip_to_address_nfree(const char* ip, const char* ip_db_path); //ip地址转换为地理位置

#endif // __ozgcc__Common__
