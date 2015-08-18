#ifndef __ozgcc__Common__
#define __ozgcc__Common__

#include <stdio.h>

//后缀为ndel的函数需要手工释放，ndel == not delete
//后缀为nfree的函数需要手工释放，nfree == not nfree

char* str_replace(char* src, char* sub, char* dst); //替换字符串
char* str_append_nfree(char* src_str, char* str); //

bool dir_exist(char* dir_path); //检查文件夹是否存在
void dir_create(char* dir_path); //生成文件夹，父目录不存在会自动建立

#endif // __ozgcc__Common__
