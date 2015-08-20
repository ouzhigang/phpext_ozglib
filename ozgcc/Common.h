#ifndef __ozgcc__Common__
#define __ozgcc__Common__

#include <stdio.h>

void _ozgtest(const char* test_str);

//��׺Ϊnfree�ĺ�����Ҫ�ֹ��ͷţ�nfree == not nfree

char* str_replace(char* src, char* sub, char* dst); //�滻�ַ���
char* str_append_nfree(char* src_str, char* str); //�ַ���׷��

void dir_create(char* dir_path); //�����ļ��У���Ŀ¼�����ڻ��Զ�����
void dir_copy(char* from_path, char* to_path, bool over_write); //�����ļ���

bool file_exist(char* file_path); //����ļ����ļ����Ƿ����
void file_move(char* old_path, char* new_path, bool over_write); //�ƶ��ļ����ļ���
void file_remove(char* file_path); //ɾ���ļ����ļ���
void file_copy(char* from_path, char* to_path, bool over_write); //�����ļ�

char* ip_to_address_nfree(const char* ip, const char* ip_db_path); //ip��ַת��Ϊ����λ��

#endif // __ozgcc__Common__
