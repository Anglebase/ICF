#include "String.h"

String::String(std::string str) :instr(str) {}

/// @brief 按索引取值
/// @param index 允许负数
/// @return 索引对应的值
char& String::operator[](int index)
{
	if (index >= 0)
		return this->instr[index];
	else
		return this->instr[this->instr.length() + index];
}

/// @brief 强制类型转换
String::operator std::string() const
{
	return this->instr;
}

/// @brief 判断该字符串中指定位置是否与传入字符串完全相同
/// @param index 指定位置
/// @param str 传入字符串
/// @return 是/否
bool String::include(int& index, std::string& str)
{
	for (auto& ch : str) {
		if (ch != this->instr[index])
			return false;
		else
			index++;
	}
	return true;
}

/// @brief 获取字符串长度
/// @return 字符串长度
int String::size() const
{
	return this->instr.size();
}


