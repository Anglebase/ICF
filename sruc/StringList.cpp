#include "StringList.h"
#include<ostream>

/// @brief 按字符拆分字符串
/// @param str 源字符串
/// @param c 要按照的字符
StringList::StringList(const std::string& str, const char c)
{
	std::string s;
	for (auto& ch : str) {
		if (ch == c)
			this->list.push_back(s), s = "";
		else
			s += ch;
	}
	this->list.push_back(s);
}

/// @brief 按索引取值，包含负数
/// @param index 索引
/// @return 索引对应的值
std::string StringList::operator[](int index)
{
	if (index >= 0)
		return this->list[index];
	else
		return this->list[list.size() + index];
}

/// @brief 判断一个字符串是否包含于此字符串列表
/// @param str 要查询的字符串
/// @return 是否存在(是/否)
bool StringList::include(const std::string& str) const
{
	for (auto& s : this->list)
		if (s == str)return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, StringList ls)
{
	for (auto& str : ls.list) {
		os << "\"" << str << "\" ";
	}
	return os;
}
