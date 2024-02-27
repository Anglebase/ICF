#pragma once
#include "Unit.h"
#include"Mark.h"
#include"StringList.h"
#include<stack>
#include"error.h"

class Check :
    public Unit
{
public:
    Check(const char* filepath);
protected:
    void checkPair();
    void remarkInnerStruct();
    void removeWhiteSpace();
    void resetCode();
    void checkSyntax();

    void removeNote();

    //void error(Token& tok, std::string err_str);
private:
    std::stack<Token> st;// 括号栈
    StringList ls;
    error err_obj;
};

