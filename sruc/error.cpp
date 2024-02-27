#include "error.h"
#include<iostream>

/// @brief 报错信息输出
/// @param tok 所在Token
/// @param err_str 报错信息
void error::out(Token& tok, std::string err_str)
{
    std::string s, ln;
    //if(ls.std().size()>tok.getLine())
        for (auto& c : ls[tok.getLine() - 1]) {
            if (c == '\t') {
                ln += "    ";
            }
            else ln += c;
        }
    s << tok.getLine();
    std::cout << "出错：" << err_str << std::endl;
    std::cout << tok << std::endl;
    std::cout << tok.getLine() << "| " << ln << std::endl;
    std::cout << std::string(s.size(), ' ') << "| " <<
        std::string(tok.getPos() - 1, ' ') << "^" << std::endl;
    exit(0);
}

/// @brief 代码警告输出
/// @param tok 所在Token
/// @param err_str 警告信息
void warning::out(Token& tok, std::string err_str)
{
    std::string s, ln;
    for (auto& c : ls[tok.getLine() - 1]) {
        if (c == '\t') {
            ln += "    ";
        }
        else ln += c;
    }
    s << tok.getLine();
    std::cout << "警告：" << err_str << std::endl;
    std::cout << tok << std::endl;
    std::cout << tok.getLine() << "| " << ln << std::endl;
    std::cout << std::string(s.size(), ' ') << "| " <<
        std::string(tok.getPos() - 1, ' ') << "^" << std::endl;
}
