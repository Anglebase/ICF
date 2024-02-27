#pragma once
#include"Link.h"

/// @brief 链接所有代码
class LinkAll
{
public:
	LinkAll(const char* filePath);
	std::string getCode() { return this->final_code; }
protected:
	void Format();
private:
	std::vector<std::pair<CodeType, std::string>> code;
	std::string outputText;
	std::string NVICInit;
	std::string INPTDeal;

	std::string final_code;
	std::string after_format;
};