#include "Token.h"
#include"globals.h"
/// @brief 默认构造
Token::Token()
{
	this->line = -1;
	this->pos = -1;
}

/// @brief 通过所在行列构造
/// @param line 所在行
/// @param pos 所在列
Token::Token(int line, int pos)
{
	this->line = line;
	this->pos = pos;
}

/// @brief 重设所在行列位置
/// @param line 所在行
/// @param pos 所在列
void Token::setPos(int line, int pos)
{
	this->line = line;
	this->pos = pos;
}

/// @brief 返回它所对应的一串字符
/// @return 字符串
const std::string& Token::str()const
{
	return this->s;
}

TOKENS& Token::type()
{
	return this->token;
}

/// @brief 格式化输出
/// @param os 
/// @param token 
/// @return 
std::ostream& operator<<(std::ostream& os, const Token& token)
{
#ifdef DEBUG
	os << "(" << token.line << ", " << token.pos << ")   \t "
		<< token.token << "   \t" << [=]()->std::string {
		if (token.s == "\n")return "'\\n'";
		if (token.s == "\t")return "'\\t'";
		if (token.s == " ")return "' '";
		return token.s;
	}();
#else
	os << "所在行：" << token.line << "，所在列：" << token.pos;
#endif
	return os;
}

Token& Token::operator<<(char c)
{
	this->s += c;
	return (*this);
}

Token& Token::operator+=(const std::string& s)
{
	this->s += s;
	return (*this);
}

void Token::operator>>(const TOKENS& token)
{
	this->token = token;
}

/// @brief 获取行
/// @return 所在行
int Token::getLine() const
{
	return this->line;
}

/// @brief 获取列
/// @return 所在列
int Token::getPos() const
{
	return this->pos;
}

/// @brief 设置所在文件路径
/// @param s 文件路径
void Token::setPath(std::string s)
{
	this->filepath = s;
}

/// @brief 获取所在文件路径
/// @return 文件路径
std::string Token::getPath()
{
	return this->filepath;
}
