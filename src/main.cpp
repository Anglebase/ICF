#include <iostream>
#include "LinkAll.h"
#include "StringList.h"
#include "error.h"
#include "String.h"
#include "WriteFileObject.h"

void help()
{
	std::cout << "命令语法： icf {源文件路径} [目标文件路径]" << std::endl;
	std::cout << "    源文件路径：要编译的 ICF 文件的路径" << std::endl;
	std::cout << "    目标文件路径：编译后的C源文件目标路径，"
				 "如果目标路径不存在，则会输出到屏幕"
			  << std::endl;
}

void err(std::string s)
{
	std::cout << "错误：" << s << std::endl;
}

int main(int argv, char *argc[])
{
	system("chcp 65001");

	if (argv == 1)// 无参
	{
		std::cout << "版本 V1.1" << std::endl;
		err("参数不能为空");
		help();
	}
	else if (argv == 2) // 单参
	{
		std::cout << LinkAll(argc[1]).getCode();
	}
	else if (argv == 3) // 双参
	{
		auto o = LinkAll(argc[1]);
		WriteFileObject(argc[2], o.getCode());
	}
	else // 其他
	{
		err("参数不正确");
	}
	return 0;
}