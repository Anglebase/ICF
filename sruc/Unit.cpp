#include "Unit.h"

/// @brief 获取文档对象
/// @return 文档对象
std::vector<Token>& Unit::getDoc()
{
    return this->doc;
}

/// @brief 按下标取值
/// @param index 为负数时从后向前取值
/// @return 下标对应的值
Token& Unit::operator[](int index)
{
    if (index >= 0)return this->doc[index];
    else return this->doc[this->doc.size() + index];
}
