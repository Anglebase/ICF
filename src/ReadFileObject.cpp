#include "ReadFileObject.h"
#include<string>
#include<iostream>
static int count = 0;

/**
 * @param filepath 要读取的文件所在路径
 * @note 当文件没有正常打开时：
 * 		 在非DEBUG模式：输出异常并退出程序
 * 		 在DEBUG模式：输出导致异常的文件
*/
ReadFileObject::ReadFileObject(const char* filepath) :filepath(filepath) {
	std::string line;
	std::fstream ifs(filepath, std::ios::in);
	if (!ifs.is_open()) {
#ifdef DEBUG
		std::cout << "文件打开失败: " << filepath << std::endl;
#endif
		std::cout << "异常：读取文件时失败" << std::endl;
		exit(0);
	}
	while (!ifs.eof())
	{
		getline(ifs, line);
		this->content += line + '\n';
	}
	ifs.close();
}

/**
 * @return 读取到的文件内容
*/
std::string ReadFileObject::getContent()
{
	return this->content;
}
