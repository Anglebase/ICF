#pragma once
#include"Explain.h"
#include"signSet.h"

/// @brief 链接中断部分代码
class Link
{
public:
	Link(const char* filePath);
	void CheckNVIC();
	void OutputNVICCode();
	void OutputINPTCode();
	std::vector<std::pair<CodeType, std::string>> code;
	std::string outputText;
	std::string NVICInit;
	std::string INPTDeal;
private:
	int NVICGroup;

	std::vector<std::pair<std::pair<char, int>, std::pair<int, int>>> N1_4;
	std::vector<std::pair<std::pair<char, int>, std::pair<int, int>>> N9_5;
	std::vector<std::pair<std::pair<char, int>, std::pair<int, int>>> N15_10;
	std::vector<std::pair<std::pair<char, int>, std::string>> I1_4;
	std::vector<std::pair<std::pair<char, int>, std::string>> I9_5;
	std::vector<std::pair<std::pair<char, int>, std::string>> I15_10;
};

