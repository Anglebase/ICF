#include "LinkAll.h"
#include<iostream>

LinkAll::LinkAll(const char* filePath)
{
	Link obj(filePath);
	this->code = obj.code;
	this->INPTDeal = obj.INPTDeal;
	this->NVICInit = obj.NVICInit;
	this->after_format = "";
	// 目标代码生成
	this->final_code = "#include \"stm32f10x.h\"\n";
	for (auto& s : this->code) {
		if (s.first == CodeType::GLOBAL) {
			this->final_code << s.second;
		}
	}
	this->final_code << "\nint main(void){\n";
	for (auto& s : this->code) {
		if (s.first == CodeType::GPIOINIT||s.first==CodeType::GPIOINPT) {
			this->final_code << s.second;
		}
	}
	this->final_code << this->NVICInit;
	this->final_code << "\nwhile(1){\n";
	for (auto& s : this->code) {
		if (s.first == CodeType::MAIN) {
			this->final_code << s.second;
		}
	}
	this->final_code << "\n}\n";
	this->final_code << "}\n";
	this->final_code << this->INPTDeal;
#ifdef DEBUG
	std::cout << this->final_code << std::endl;
#endif // DEBUG
}

// 移除字符串的空白符
std::string strip(std::string s) {
	std::string res;
	bool is_space = true;
	for (auto& c : s) {
		if (c == ' ' || c == '\t')is_space = false;
		if (!is_space) {
			res += c;
		}
	}
	return res;
}

// 程序内代码格式化(尚未实现)
void LinkAll::Format()
{
	std::vector<std::string> ls = StringList(this->final_code, '\n').std();
	for (auto& s : ls) {
		s = strip(s);
	}
}

