#include "Common.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdarg>

#include "boost/filesystem.hpp"

#include "IpFinder.h"

using namespace std;
namespace fs = boost::filesystem;

using namespace ozgcc;

//调试变量用的函数
void _ozgtest(const char* test_str)
{
	ofstream f("D:\\ozglib.log", ios::app | ios::out);
	f.seekp(ios::end);
	f << test_str << "\r" << endl;
	f.flush();
	f.close();
}

//递归复制文件
void CopyFiles(const fs::path &src, const fs::path &dst)
{
	if (!fs::exists(dst))
	{
		fs::create_directories(dst);
	}
	for (fs::directory_iterator it(src); it != fs::directory_iterator(); ++it)
	{
		const fs::path newSrc = it->path();
		const fs::path newDst = dst / newSrc.filename();

		if (fs::is_directory(newSrc))
		{
			CopyFiles(newSrc, newDst);
		}
		else if (fs::is_regular_file(newSrc))
		{
			fs::copy_file(newSrc, newDst);
		}
		else
		{
			//_ftprintf(stderr, "Error: unrecognized file - %s", newSrc.string().c_str());
		}
	}
}

char* str_replace(char* src, char* sub, char* dst)
{
	if (strstr(src, sub) == NULL)
		return src;

	int pos = 0;
	int offset = 0;
	int srcLen, subLen, dstLen;
	char* pRet = NULL;

	srcLen = strlen(src);
	subLen = strlen(sub);
	dstLen = strlen(dst);
	pRet = (char*)malloc(srcLen + dstLen - subLen + 1); //(外部是否该空间)if (NULL != pRet)
	{
		pos = strstr(src, sub) - src;
		memcpy(pRet, src, pos);
		offset += pos;
		memcpy(pRet + offset, dst, dstLen);
		offset += dstLen;
		memcpy(pRet + offset, src + pos + subLen, srcLen - pos - subLen);
		offset += srcLen - pos - subLen;
		*(pRet + offset) = '\0';
	}
	return pRet;
}

char* str_append_nfree(char* src_str, char* str)
{
	string src_str_s(src_str);
	string str_s(str);

	src_str_s.append(str);

	char* res = (char*)malloc(src_str_s.size());
	strcpy(res, src_str_s.c_str());

	return res;
}

void dir_create(char* dir_path)
{
	if (!file_exist(dir_path))
	{
		fs::create_directories(dir_path);
	}	
}

void dir_copy(char* from_path, char* to_path, bool over_write)
{
	if (file_exist(from_path))
	{
		if (file_exist(to_path))
		{
			if (!over_write)
				return;
			else
				file_remove(to_path);
		}

		fs::path from_path_p(from_path);
		fs::path to_path_p(to_path);
		if (!file_exist((char*)to_path_p.parent_path().string().c_str()))
		{
			dir_create((char*)to_path_p.parent_path().string().c_str());
		}
		CopyFiles(from_path_p, to_path_p);
	}
}

bool file_exist(char* file_path)
{
	fs::path file_path_p(file_path);
	return fs::exists(file_path_p);
}

void file_move(char* old_path, char* new_path, bool over_write)
{
	if (file_exist(old_path))
	{
		fs::path old_path_p(old_path);
		fs::path new_path_p(new_path);
		
		if (file_exist(new_path))
		{
			if (!over_write)			
				return;
		}

		//_ozgtest(new_path_p.parent_path().string().c_str());
		if (!file_exist((char*)new_path_p.parent_path().string().c_str()))
		{
			dir_create((char*)new_path_p.parent_path().string().c_str());
		}
		
		fs::rename(old_path_p, new_path_p);
	}
}

void file_remove(char* file_path)
{
	if (file_exist(file_path))
	{
		fs::path file_path_p(file_path);
		fs::remove_all(file_path_p);
	}
}

void file_copy(char* from_path, char* to_path, bool over_write)
{
	if (file_exist(from_path))
	{
		if (file_exist(to_path))
		{
			if (!over_write)
				return;
		}

		fs::path from_path_p(from_path);
		fs::path to_path_p(to_path);

		if (!file_exist((char*)to_path_p.parent_path().string().c_str()))
		{
			dir_create((char*)to_path_p.parent_path().string().c_str());
		}
		fs::copy_file(from_path_p, to_path_p);
	}
	
}

char* ip_to_address_nfree(const char* ip, const char* ip_db_path)
{
	string country, location;

	IpFinder *ip_finder = new IpFinder(ip_db_path);
	ip_finder->GetAddressByIp(ip, country, location);
	
	delete ip_finder;

	string tmp(country);
	tmp.append(" ");
	tmp.append(location);
	//_ozgtest(ret.c_str());

	char* res = (char*)malloc(tmp.size());
	strcpy(res, tmp.c_str());

	return res;
}
