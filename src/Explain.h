#pragma once
#include "Unit.h"
#include "explainEnum.h"
#include<map>

/// @brief 选择器结构对象
struct Struct
{
    Token name;
    std::vector<Token> code;
    void clear() {
        this->name = Token();
        this->code.clear();
    }
};

/// @brief 选择器解析类
class device
{
public:
    device(const Struct& st,std::string path);
    std::pair<CodeType, std::string> toCode();
protected:
    void initGPIO(const Struct& st);
    void initTIM(const Struct& st);
    void initGlobal(const Struct& st);
    void initMain(const Struct& st);

private:
    dType dt;

    std::string name;
    bool default_;
    IO mode;
    bool have_INTP;
    std::string self_code;
    EXTI type;
    std::pair<int, int> prt;
    int fre;

    bool IO_isdefault;
    bool type_isdefault;
    bool fre_isdefault;

    std::string path;
};


// 解释编译
class Explain :
    public Unit
{
public:
    Explain(const char* filepath);
    void createINTP();
    std::vector<std::pair<CodeType, std::string>> getCode() {
        return this->code;
    }
private:
    int itGroup;
    std::map<int, int> priority;
    std::vector<std::vector<Token>> group;// �������
    std::vector<Struct> obj;
    std::vector<std::pair<CodeType,std::string>> code;
    std::string path;
};