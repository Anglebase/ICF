#pragma once
#include<vector>
#include"Token.h"

// 代码解析类基类
class Unit
{
public:
	std::vector<Token>& getDoc();
	Token& operator[](int index);
protected:
	std::vector<Token> doc;
};

