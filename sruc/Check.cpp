#include "Check.h"
#include"globals.h"
#include<iostream>

Check::Check(const char* filepath)
    :ls(StringList(ReadFileObject(filepath).getContent(), '\n')),err_obj(error(filepath)) {
    this->doc = Mark(filepath).getDoc();

    // 流水线式数据处理
    this->checkPair();
    this->remarkInnerStruct();
    this->removeWhiteSpace();
    this->resetCode();
    this->checkSyntax();
    this->removeNote();
}
extern std::string copy(std::string src, int first = 0, int end = -1);
extern bool isNumber(std::string s);

/// @brief 解析GPIO端口名
/// @param name 要解析的字符串
/// @return 解析后的组+号
std::pair<char, int> exp(Token name) {
    if (name.str()[0] == 'P' &&
        // 解析
        (name.str()[1] >= 'A' && name.str()[1] <= 'G') &&
        isNumber(copy(name.str(), 2))) {
        return std::pair<char, int>(name.str()[1], atoi(copy(name.str(), 2).c_str()));
    }
    else {
        // 报错
#ifdef DEBUG
        std::cout << "不存在的选择器目标 " << name << std::endl;
#else
        error(name.getPath().c_str()).out(name, "不存在的选择器目标");
#endif // DEBUG
    }
    return std::pair<char, int>(); // 屏蔽可能无返回值警告
}

/// @brief 检查括号是否匹配
void Check::checkPair()
{
    for (auto& tok : this->doc) {
        // 遇到左括号入栈
        if (tok.type() == BigLeft ||
            tok.type() == MidLeft ||
            tok.type() == SmallLeft) {
            this->st.push(tok);
        }
        // 遇到右大括号尝试出栈
        if (tok.type() == BigRight) {
            if (!st.empty() && st.top().type() == BigLeft) {
                st.pop();
            }
            else {
                // 未能出栈
                err_obj.out(tok, "找不到与之匹配的前括号");
            }
        }
        // 遇到右中括号尝试出栈
        if (tok.type() == MidRight) {
            if (!st.empty() && st.top().type() == MidLeft) {
                st.pop();
            }
            else {
                // 未能出栈
                err_obj.out(tok, "找不到与之匹配的前括号");
            }
        }
        // 遇到右小括号尝试出栈
        if (tok.type() == SmallRight) {
            if (!st.empty() && st.top().type() == SmallLeft) {
                st.pop();
            }
            else {
                // 未能出栈
                err_obj.out(tok, "找不到与之匹配的前括号");
            }
        }
    }
    // 遍历后栈不为空
    if (!this->st.empty()) {
        err_obj.out(st.top(), "找不到与之匹配的后括号");
    }
}

/// @brief 标记一级嵌套以外的嵌套结构
void Check::remarkInnerStruct()
{
    int c_s = 0, i_s = 0;
    for (auto& tok : this->doc)if (tok.type() != Note) {
        if (tok.type() == MidRight)c_s--;

        // 保留拓展关键字
        if (tok.str() != "set" &&
            tok.str() != "reset" &&
            tok.str() != "turn" &&
            tok.str() != "get" &&
            tok.str() != "read" &&
            c_s > 0) {
            tok >> CSourceCode;
        }

        if (tok.type() == MidLeft)c_s++;
        if (tok.type() == BigRight)i_s--;
        if (i_s >= 2) {
            if (tok.type() == ApartFor)tok >> inApartFor;
            if (tok.type() == AttrLR)tok >> inAttrLR;
        }
        if (tok.type() == BigLeft)i_s++;
    }
}


/// @brief 移除所有的空白符Token
void Check::removeWhiteSpace()
{
    std::vector<Token> newDoc;
    for (auto& tok:this->doc) {
        if (tok.type() == WarpLine ||
            tok.type() == TabSpace ||
            tok.type() == WhiteSpace);
        else newDoc.push_back(tok);
    }
    this->doc = newDoc;
}

/// @brief 解析代码中的拓展版C语言(ICC)
void Check::resetCode()
{
    int is_code = 0;
    for (int index = 0; index < this->doc.size() - 2; index++) {
        if (this->doc[index].type() == Note)continue;
        if (this->doc[index].type() == MidLeft)is_code++;
        if (this->doc[index].type() == MidRight)is_code--;
        if (is_code) {
            // 解析set关键字
            if (this->doc[index].str() == "set") {
                if (this->doc[index + 2].str()[0] != 'P' ||
                    this->doc[index + 2].str().length() > 4) {
#ifdef DEBUG
                    std::cout << "出错：‘set’参数不正确" << this->doc[index + 2] << std::endl;
#else
                    error(this->doc[index + 2].getPath().c_str())
                        .out(this->doc[index + 2], "‘set’参数不正确");
#endif // DEBUG
                }
                else {
                    if (this->doc[index + 2].str().length() < 3) {
#ifdef DEBUG
                        std::cout << "出错：‘set’参数不完整" << this->doc[index + 2] << std::endl;
#else
                        error(this->doc[index + 2].getPath().c_str())
                            .out(this->doc[index + 2], "‘set’参数不完整");
#endif // DEBUG
                    }
                    else {
                        std::string code = "";
                        code << "GPIO_SetBits(GPIO" << exp(this->doc[index + 2]).first 
                            << ", GPIO_Pin_" << exp(this->doc[index + 2]).second << ")";
                        this->doc[index].setStr(code);
                        this->doc[index + 2].setStr("");
                    }
                }
            }
            // 解析reset关键字
            else if (this->doc[index].str() == "reset") {
                if (this->doc[index + 2].str()[0] != 'P' ||
                    this->doc[index + 2].str().length() > 4) {
#ifdef DEBUG
                    std::cout << "出错：‘reset’参数不正确" << this->doc[index + 2] << std::endl;
#else
                    error(this->doc[index + 2].getPath().c_str())
                        .out(this->doc[index + 2], "‘reset’参数不正确");
#endif // DEBUG
                }
                else {
                    if (this->doc[index + 2].str().length() < 3) {
#ifdef DEBUG
                        std::cout << "出错：‘reset’参数不完整" << this->doc[index + 2] 
                            << std::endl;
#else
                        error(this->doc[index + 2].getPath().c_str())
                            .out(this->doc[index + 2], "‘reset’参数不完整");
#endif // DEBUG
                    }
                    else {
                        std::string code = "";
                        code << "GPIO_ResetBits(GPIO" << exp(this->doc[index + 2]).first
                            << ", GPIO_Pin_" << exp(this->doc[index + 2]).second << ")";
                        this->doc[index].setStr(code);
                        this->doc[index + 2].setStr("");
                    }
                }
            }
            // 解析turn关键字
            else if (this->doc[index].str() == "turn") {
                if (this->doc[index + 2].str()[0] != 'P' ||
                    this->doc[index + 2].str().length() > 4) {
#ifdef DEBUG
                    std::cout << "出错：‘turn’参数不正确" << this->doc[index + 2] << std::endl;
#else
                    error(this->doc[index + 2].getPath().c_str())
                        .out(this->doc[index + 2], "‘turn’参数不正确");
#endif // DEBUG
                }
                else {
                    if (this->doc[index + 2].str().length() < 3) {
#ifdef DEBUG
                        std::cout << "出错：‘turn’参数不完整" << this->doc[index + 2] << std::endl;
#else
                        error(this->doc[index + 2].getPath().c_str())
                            .out(this->doc[index + 2], "‘turn’参数不完整");
#endif // DEBUGturn
                    }
                    else {
                        std::string code = "";
                        code << "if (GPIO_ReadOutputDataBit(GPIO"
                            << exp(this->doc[index + 2]).first << ", GPIO_Pin_" << exp(this->doc[index + 2]).second << ") == 0)\n";
                        code << "GPIO_SetBits(GPIO"
                            << exp(this->doc[index + 2]).first << ", GPIO_Pin_"
                            << exp(this->doc[index + 2]).second << ");\nelse ";
                        code << "GPIO_ResetBits(GPIO"
                            << exp(this->doc[index + 2]).first << ", GPIO_Pin_"
                            << exp(this->doc[index + 2]).second << ")";
                        this->doc[index].setStr(code);
                        this->doc[index + 2].setStr("");
                    }
                }
            }
            // 解析get关键字
            else if (this->doc[index].str() == "get") {
                if (this->doc[index + 2].str()[0] != 'P' ||
                    this->doc[index + 2].str().length() > 4) {
#ifdef DEBUG
                    std::cout << "出错：‘get’参数不正确" << this->doc[index + 2] << std::endl;
#else
                    error(this->doc[index + 2].getPath().c_str())
                        .out(this->doc[index + 2], "‘get’参数不正确");
#endif // DEBUG
                }
                else {
                    if (this->doc[index + 2].str().length() < 3) {
#ifdef DEBUG
                        std::cout << "出错：‘get’参数不完整" << this->doc[index + 2] << std::endl;
#else
                        error(this->doc[index + 2].getPath().c_str())
                            .out(this->doc[index + 2], "‘get’参数不完整");
#endif // DEBUGturn
                    }
                    else {
                        std::string code = ""; 
                        code << "GPIO_ReadOutputDataBit(GPIO" << exp(this->doc[index + 2]).first
                            << ", GPIO_Pin_" << exp(this->doc[index + 2]).second << ")";
                        this->doc[index].setStr(code);
                        this->doc[index + 2].setStr("");
                    }
                }
            }
            // 解析read关键字
            else if (this->doc[index].str() == "read") {
                if (this->doc[index + 2].str()[0] != 'P' ||
                    this->doc[index + 2].str().length() > 4) {
#ifdef DEBUG
                    std::cout << "出错：‘read’参数不正确" << this->doc[index + 2] << std::endl;
#else
                    error(this->doc[index + 2].getPath().c_str())
                        .out(this->doc[index + 2], "‘read’参数不正确");
#endif // DEBUG
                }
                else {
                    if (this->doc[index + 2].str().length() < 3) {
#ifdef DEBUG
                        std::cout << "出错：‘read’参数不完整" << this->doc[index + 2] << std::endl;
#else
                        error(this->doc[index + 2].getPath().c_str())
                            .out(this->doc[index + 2], "‘read’参数不完整");
#endif // DEBUGturn
                    }
                    else {
                        std::string code = "";
                        code << "GPIO_ReadInputDataBit(GPIO" << exp(this->doc[index + 2]).first
                            << ", GPIO_Pin_" << exp(this->doc[index + 2]).second << ")";
                        this->doc[index].setStr(code);
                        this->doc[index + 2].setStr("");
                    }
                }
            }
        }
    }
    // 标记所有的C源代码
    int code = 0;
    for (auto& tok : this->doc) {
        if (tok.type() == MidLeft)code++;
        if (tok.type() == MidRight)code--;
        if (tok.type() == AttrLR && code) {
            tok >> CSourceCode;
        }
    }
}

/// @brief 进行基本语法检查
void Check::checkSyntax()
{
    for (int index = 0; index < this->doc.size(); index++) {
        if (this->doc[index].type() == AttrLR) {
            // 检查冒号前内容是否符合语法
            if (this->doc[index - 1].type() != Keyword) {
                err_obj.out(this->doc[index], "冒号之前缺少关键字/属性名");
            }
            // 检查冒号后内容是否符合语法
            if (this->doc[index + 1].type() == UnMark ||
                this->doc[index + 1].type() == Undef ||
                this->doc[index + 1].type() == ApartFor) {
                err_obj.out(this->doc[index], "冒号之后缺少对应的属性值");
            }
        }
    }
}

/// @brief 移除所有注释
void Check::removeNote()
{
    std::vector<Token> newDoc;
    for (auto& tok : this->doc) {
        if (tok.type() == Note ||
            tok.type() == NoteLeft ||
            tok.type() == NoteRight ||
            tok.type() == NoteLine);
        else newDoc.push_back(tok);
    }
    this->doc = newDoc;
}

// 报错信息模板
//void Check::error(Token& tok, std::string err_str)
//{
//    std::string s, ln;
//    for (auto& c : ls[tok.getLine() - 1]) {
//        if (c == '\t') {
//            ln += "    ";
//        }
//        else ln += c;
//    }
//    s << tok.getLine();
//    std::cout << "出错：" << err_str << std::endl;
//    std::cout << tok << std::endl;
//    std::cout << tok.getLine() << "| " << ln << std::endl;
//    std::cout << std::string(s.size(), ' ') << "| " <<
//        std::string(tok.getPos() - 1, ' ') << "^" << std::endl;
//}
