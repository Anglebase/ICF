#pragma once
#include<vector>
#include<string>

/// @brief 字符串列表类
class StringList
{
public:
	StringList() {};
	StringList(const std::string& str,const char c);
	std::string operator[](int index);
	friend std::ostream& operator<<(std::ostream& os, StringList ls);
	bool include(const std::string& str) const;
	const std::vector<std::string>& std() const{ return this->list; }
private:
	std::vector<std::string> list;
};


