
#ifndef PHP_OZGLIB_CFG_H
#define PHP_OZGLIB_CFG_H

#ifdef WIN32
//����flock��ʹ��PHP�ڲ��������Ĳ���
#define F_LOCK_SH 1 //Ҫȡ�ù�����������ȡ����
#define F_LOCK_EX 2 //Ҫȡ�ö�ռ������д�����
#define F_LOCK_UN 3 //Ҫ�ͷ����������۹�����ռ��
#define F_LOCK_NB 4 //����㲻ϣ�� flock() ������ʱ��������� operation ���� LOCK_NB
//����flock��ʹ��PHP�ڲ��������Ĳ��� end
#endif

#define F_LOCK_NAME "./lock.tmp" //����������ʱ�ļ�·����Ĭ�ϣ����php.ini�����˸�·�������·���ͻᱻ���ǣ�

//ozglib_rand_str�����ĳ���
#define OZGLIB_RAND_STR_LOWER 0x00000001
#define OZGLIB_RAND_STR_UPPER 0x00000002
#define OZGLIB_RAND_STR_NUMBER 0x00000003
#define OZGLIB_RAND_STR_LOWER_UPPER 0x00000004
#define OZGLIB_RAND_STR_UPPER_NUMBER 0x00000005
#define OZGLIB_RAND_STR_LOWER_NUMBER 0x00000006
#define OZGLIB_RAND_STR_ALL 0x00000007

#endif
