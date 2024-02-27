#pragma once
#include<string>
#include<ostream>
#include"StringList.h"
#include"settings.h"
// Token标识
typedef enum
{
    // 无义标志
    None,
    Undef,
    UnMark,
    // 成对标识符
    BigLeft,		// {
    BigRight,		// }
    MidLeft,		// [
    MidRight,		// ]
    SmallLeft,		// (
    SmallRight,		// )
    NoteLeft,		// /*
    NoteRight,		// */
    // 双目标识符
    AttrLR,			// :
    inApartFor,			// 
    inAttrLR,			// 
    ToSpace,             // -
    // 前置标识符
    NoteLine,		// //
    // 后置标识符
    SplitFor,		// ,
    ApartFor,		// ;
    // 字符串对象类型
    Number,
    Iden,
    WarpLine,
    WhiteSpace,
    TabSpace,
    Keyword,
    EnumValue,
    Note,
    CSourceCode,
    Header,
}TOKENS;

typedef enum {
    Null = 0x00u,
    Num = 0x01u,
    Upper = 0x02u,
    Lower = 0x04u,
    Underline = 0x08u,
    Alpha = 0x06u,
    Point = 0x10u,
    Warp = 0x20u,
}TYPES;


const StringList keywords(
    "global,"
    "main,"
    "code,"
    "mode,"
    "f,"
    "INPT,"
    "type,"
    "priority,"
    "func,"
    "set,"
    "reset,"
    "turn,"
    "default", ',');
const StringList enumValues(
    "OUT_PP,"
    "OUT_OD,"
    "IN_FALOTING,"
    "IPU,"
    "IPD,"
    "AF_PP,"
    "AF_OD,"
    "AIN,"
    "Rising,"
    "Falling", ',');
const StringList typeEnum(
    "Rising,"
    "Falling", ',');