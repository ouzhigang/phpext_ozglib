#ifndef __ozgcc__Common__
#define __ozgcc__Common__

#include <stdio.h>

//��׺Ϊndel�ĺ�����Ҫ�ֹ��ͷţ�ndel == not delete
//��׺Ϊnfree�ĺ�����Ҫ�ֹ��ͷţ�nfree == not nfree

char* str_replace(char* src, char* sub, char* dst); //�滻�ַ���
char* str_append_nfree(char* src_str, char* str); //

bool dir_exist(char* dir_path); //����ļ����Ƿ����
void dir_create(char* dir_path); //�����ļ��У���Ŀ¼�����ڻ��Զ�����

#endif // __ozgcc__Common__
