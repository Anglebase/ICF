#pragma once
#include<string>
#include"globals.h"
/// @brief 词素类
class Token
{
public:
	Token();
	Token(int line, int pos);
	void setPos(int line, int pos);
	const std::string& str() const;
	TOKENS& type();
	friend std::ostream& operator<<(std::ostream& os, const Token& token);
	Token& operator<<(char c);
	Token& operator+=(const std::string& s);
	void operator>>(const TOKENS& token);
	int getLine()const;
	int getPos()const;
	void setPath(std::string s);
	std::string getPath();
	void setStr(std::string s) {
		this->s = s;
	}
private:
	int line;
	int pos;
	std::string s;
	TOKENS token;
	std::string filepath;
};

