#include "Mark.h"
#include"settings.h"
#include"StringList.h"

#define MARK(x,y) if (tok.str() == x)tok >> y;

Mark::Mark(const char* filepath) :
    ast(ReadFileObject(filepath).getContent()),stringlist(StringList(ast,'\n')),path(filepath)
{
    this->split();
    this->markNote();
    this->mark();
    this->markCCode();
}

/// @brief 对源文本进行Token拆分
void Mark::split()
{
    int line = 1, pos = 1;
    for (int index = 0; index < this->ast.size(); index++,pos++) {
        Token token(line, pos);
        token.setPath(this->path);
        if (this->ast[index] == (Alpha|Underline)) {
            for (int& i = index; i < this->ast.size(); i++,pos++) {
                if (this->ast[i] == (Alpha | Num | Underline))
                    token << this->ast[i];
                else break;
            }
            token >> Iden;
            index--,pos--;
        }
        else if (this->ast[index] == (Num|Point)) {
            bool wasPoint = false;
            for (int& i = index; i < this->ast.size(); i++,pos++) {
                if (this->ast[i] == (Num | Point) && !wasPoint) {
                    token << this->ast[i];
                    if (this->ast[i] == Point)
                        wasPoint = true;
                }
                else if (this->ast[i] == (Num) && wasPoint) {
                    token << this->ast[i];
                }
                else break;
            }
            token >> Number;
            index--,pos--;
        }
        else if (this->ast[index] == '/') {
            token << this->ast[index];
            if (this->ast[index + 1] == '*') {
                token << this->ast[index + 1];
                token >> NoteLeft;
                index++;
            }
            else if (this->ast[index + 1] == '/') {
                token << this->ast[index + 1];
                token >> NoteLine;
                index++;
            }
            else {
                token >> UnMark;
            }
        }
        else if (this->ast[index] == '*') {
            token << this->ast[index];
            if (this->ast[index + 1] == '/') {
                token << this->ast[index + 1];
                token >> NoteRight;
                index++;
            }
            else {
                token >> UnMark;
            }
        }
        else if (this->ast[index] == '#') {
            token << this->ast[index];
            token >> NoteLine;
        }
        else if (this->ast[index] == Warp) {
            token << this->ast[index];
            token >> WarpLine;
            line++, pos = 0;
        }
        else if (this->ast[index] == '\t') {
            token << this->ast[index];
            token >> TabSpace;
            pos += setting::TabSize-1;
        }
        else {
            token << this->ast[index];
            token >> UnMark;
        }
        this->doc.push_back(token);
    }
}

/// @brief 标记源文本中的注释
void Mark::markNote()
{
    bool isNoteLine = false;
    bool isNoteBlock = false;
    for (auto& tok : this->doc) {
        if (tok.type() == WarpLine)isNoteLine = false;
        if (tok.type() == NoteRight)isNoteBlock = false;
        if (isNoteBlock || isNoteLine)tok >> Note;
        if (tok.type() == NoteLeft)isNoteBlock = true;
        if (tok.type() == NoteLine)isNoteLine = true;
    }
}
/// @brief 标记源文本中的符号
void Mark::mark()
{
    for (auto& tok : this->doc) if (tok.type() != Note) {
        if (tok.str() == "{")tok >> BigLeft;
        if (tok.str() == "}")tok >> BigRight;
        if (tok.str() == "[")tok >> MidLeft;
        if (tok.str() == "]")tok >> MidRight;
        if (tok.str() == "(")tok >> SmallLeft;
        if (tok.str() == ")")tok >> SmallRight;
        if (tok.str() == ",")tok >> SplitFor;
        if (tok.str() == ";")tok >> ApartFor;
        if (tok.str() == ":")tok >> AttrLR;
        if (tok.str() == " ")tok >> WhiteSpace;
        if (tok.str() == "-")tok >> ToSpace;
        if (keywords.include(tok.str()))tok >> Keyword;
        if (enumValues.include(tok.str()))tok >> EnumValue;
    }
}

/// @brief 无内容
void Mark::markCCode()
{
    
}

// 返回行字符串列表
StringList& Mark::getList()
{
    return this->stringlist;
}
