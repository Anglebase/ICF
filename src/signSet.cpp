#include "signSet.h"

// 对全局静态变量池的初始化
bool signSet::enable_rcc_GPIOA = false;
bool signSet::enable_rcc_GPIOB = false;
bool signSet::enable_rcc_GPIOC = false;
bool signSet::enable_rcc_GPIOD = false;
bool signSet::enable_rcc_GPIOE = false;
bool signSet::enable_rcc_GPIOF = false;
bool signSet::enable_rcc_GPIOG = false;
bool signSet::is_haven_GPIOInit = false;
bool signSet::is_haven_NVICInit = false;
bool signSet::enable_AFIO = false;
bool signSet::is_haven_EXTIInit = false;
std::vector<std::pair<std::pair<char, int>, std::pair<int, int>>> signSet::priority;
std::vector<std::pair<std::pair<char, int>, std::string>> signSet::interrupt;

StringList signSet::ls;
bool signSet::ls_is_empty = true;
