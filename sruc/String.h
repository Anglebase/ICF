#pragma once
#include<string>

/// @brief 字符串封装
class String
{
public:
	String(std::string str);
	char& operator[](int index);
	operator std::string() const;
	bool include(int& index, std::string& str);
	int size()const;
private:
	std::string instr;
};

