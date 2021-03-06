#include "Common.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

extern "C" {
#include <stdarg.h>
#include <time.h>
}

#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/foreach.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/make_shared.hpp"
#include "boost/random.hpp"
#include "boost/random/uniform_int.hpp"

#include "IpFinder.h"

using namespace std;
using namespace boost;
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

	string src_s(src);
	replace_all(src_s, string(sub), string(dst));

	return (char*)src_s.c_str();
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

char* str_random_nfree(char* str, int len)
{
	boost::random::mt19937_64 gen(time(0));
	boost::uniform_int<> real(0, strlen(str) - 1);

	char* res = (char*)malloc(len + 1);
	for (int i = 0; i < len; i++)
	{
		res[i] = str[real(gen)];
	}
	res[len] = '\0';
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

	//scoped_ptr<IpFinder> ip_finder(new IpFinder(ip_db_path));
	boost::shared_ptr<IpFinder> ip_finder = boost::make_shared<IpFinder>(ip_db_path);
	ip_finder->GetAddressByIp(ip, country, location);
	
	string tmp(country);
	tmp.append(" ");
	tmp.append(location);
	//_ozgtest(ret.c_str());

	char* res = (char*)malloc(tmp.size());
	strcpy(res, tmp.c_str());

	return res;
}
