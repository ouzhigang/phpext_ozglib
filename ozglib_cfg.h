
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

#define F_LOCK_NAME "D:\\1.txt" //����������ʱ�ļ�·��

#endif
