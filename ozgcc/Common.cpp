#include "Common.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/convenience.hpp"

using namespace std;
using namespace boost::filesystem;

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

bool dir_exist(char* dir_path)
{
	return boost::filesystem::exists(dir_path);
}

void dir_create(char* dir_path)
{
	boost::filesystem::create_directories(dir_path);
}
