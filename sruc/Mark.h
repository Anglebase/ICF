#pragma once
#include "Unit.h"
#include"String.h"
#include"ReadFileObject.h"

/// @brief Token标记类
class Mark :
    public Unit
{
public:
    Mark(const char* filepath);
protected:
    void split();
    void markNote();
    void mark();
    void markCCode();
    StringList& getList();
private:
    String ast;
    StringList stringlist;
    std::string path;
};

