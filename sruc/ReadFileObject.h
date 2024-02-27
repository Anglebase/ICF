#pragma once
#include<fstream>
#include"globals.h"
// 只读文件类对象
class ReadFileObject
{
public:
	ReadFileObject(const char* filepath);
	std::string getContent();
private:
	std::string filepath;
	std::string content;
};

