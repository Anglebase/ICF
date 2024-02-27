#pragma once
#include<vector>
#include<string>
#include"StringList.h"

/// @brief 全局静态变量池
class signSet
{
public:
	static bool enable_rcc_GPIOA;
	static bool enable_rcc_GPIOB;
	static bool enable_rcc_GPIOC;
	static bool enable_rcc_GPIOD;
	static bool enable_rcc_GPIOE;
	static bool enable_rcc_GPIOF;
	static bool enable_rcc_GPIOG;
	static bool is_haven_GPIOInit;
	static bool is_haven_EXTIInit;
	static bool is_haven_NVICInit;
	static int signPos;
	static bool enable_AFIO;
	static std::vector<std::pair<std::pair<char, int>, std::pair<int, int>>> priority;
	static std::vector<std::pair<std::pair<char, int>, std::string>> interrupt;

	static StringList ls;
	static bool ls_is_empty;
};


