#pragma once
#include"StringList.h"
#include"Token.h"
#include"ReadFileObject.h"
#include"signSet.h"

/// @brief 报错
class error {
public:
	error(const char* filepath){
		if (signSet::ls_is_empty) {
			signSet::ls = StringList(ReadFileObject(filepath).getContent(), '\n');
			this->ls = signSet::ls;
			signSet::ls_is_empty = false;
		}
		else {
			this->ls = signSet::ls;
		}
	}
	void out(Token& tok,std::string err_str);
private:
	StringList ls;
};

/// @brief 警告
class warning {
public:
	warning(const char* filepath){
		if (signSet::ls_is_empty) {
			signSet::ls = StringList(ReadFileObject(filepath).getContent(), '\n');
			this->ls = signSet::ls;
			signSet::ls_is_empty = false;
		}
		else {
			this->ls = signSet::ls;
		}
	}
	void out(Token& tok, std::string err_str);
private:
	StringList ls;
};