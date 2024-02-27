#pragma once
#include"globalenum.h"

// 全局公用函数
inline bool operator==(const char& c, const TYPES& type) {
    if (type & Upper) {
        if (c >= 'A' && c <= 'Z')return true;
    }
    if (type & Lower) {
        if (c >= 'a' && c <= 'z')return true;
    }
    if (type & Num) {
        if (c >= '0' && c <= '9')return true;
    }
    if (type & Underline) {
        if (c == '_')return true;
    }
    if (type & Point) {
        if (c == '.')return true;
    }
    if (type & Warp) {
        if (c == '\n')return true;
    }
    return false;
}
inline TYPES operator|(const TYPES& x, const TYPES& y) {
    return (TYPES)(((unsigned)x) | y);
}

inline std::ostream& operator<<(std::ostream& os, TOKENS token) {
#define CASE(x) case x:os << #x; break
    switch (token)
    {
        CASE(None);
        CASE(Undef);
        CASE(UnMark);
        CASE(BigLeft);
        CASE(BigRight);
        CASE(MidLeft);
        CASE(MidRight);
        CASE(SmallLeft);
        CASE(SmallRight);
        CASE(NoteLeft);
        CASE(NoteRight);
        CASE(AttrLR);
        CASE(inApartFor);
        CASE(inAttrLR);
        CASE(NoteLine);
        CASE(SplitFor);
        CASE(ApartFor);
        CASE(Number);
        CASE(Iden);
        CASE(WarpLine);
        CASE(WhiteSpace);
        CASE(TabSpace);
        CASE(Keyword);
        CASE(EnumValue);
        CASE(Note);
        CASE(CSourceCode);
        CASE(ToSpace);
        CASE(Header);
    }
    return os;
#undef CASE
}

template<class T>
inline std::string& operator<<(std::string& str, T s) {
    return str += s;
}
template<>
inline std::string& operator<<<int>(std::string& str, int num) {
    char s[12];
    _itoa_s(num, s, 10);
    s[11] = '\0';
    return str += s;
}

#define BOOL(x) ((x)?"true":"false")
//#define DEBUG