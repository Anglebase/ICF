#pragma once
#include<fstream>
#include<iostream>
class WriteFileObject
{
public:
	/// @brief 只写文件类对象
	/// @param filepath 文件所在路径
	/// @param s 要写入文件的内容
	WriteFileObject(const char* filepath,std::string s) {
		std::ofstream ofs;
		ofs.open(filepath,std::ios::trunc|std::ios::out);
		if (!ofs.is_open()) {
			std::cout << "异常：写入文件时失败" << std::endl;
		}
		ofs << s;
		ofs.close();
	}
};

