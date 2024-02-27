#include "Explain.h"
#include"Check.h"
#include<iostream>

Explain::Explain(const char* filepath) {
	this->doc = Check(filepath).getDoc();
	this->path = filepath;
	// 选择器解析
	bool is_group_body = false;
	int count = 0;
	std::vector<Token> g;
	for (auto& tok : this->doc) {
		if (!is_group_body && (tok.type() == Iden || tok.type() == Keyword))is_group_body = true;
		if (is_group_body)g.push_back(tok);
		if (tok.type() == BigLeft)count++;
		if (tok.type() == BigRight) {
			count--;
			if (count == 0) {
				this->group.push_back(g);
				g.clear();
			}
		}
	}
	this->createINTP();
	for (auto& s : this->obj) {
		this->code.push_back(device(s, this->path).toCode());
	}

#ifdef DEBUG
	for (auto& s : this->code) {
		std::cout << s.second << std::endl;
	}

	for (auto& s : this->group) {
		for (auto& c : s) {
			std::cout << c << std::endl;
		}
		std::cout << std::endl;
	}
#endif // DEBUG

}

// 中断内容引入
void Explain::createINTP()
{
	int count = 0;
	bool state = true;
	Struct o;
	for (auto& g : this->group) {
		state = true;
		for (auto& t : g) {
			if (t.type() == BigLeft) {
				count++;
				continue;
			}
			if (count == 0) {
				o.name += t.str();
				o.name >> Header;
				if (state) {
					o.name.setPos(t.getLine(), t.getPos());
					o.name.setPath(t.getPath());
					state = false;
				}
			}
			if (t.type() == BigRight) {
				count--;
			}
			if (count > 0) {
				o.code.push_back(t);
			}
		}
		obj.push_back(o);
		o.clear();
	}
}

// 选择器解析器的分发
device::device(const Struct& st,std::string path)
{
	this->dt = Unknown;

	this->IO_isdefault = true;
	this->type_isdefault = true;
	this->fre_isdefault = true;

	this->default_ = false;
	this->mode = OUT_PP;
	this->have_INTP = false;
	this->self_code = "";
	this->type = Rising;
	this->prt = std::pair<int, int>(-1, -1);
	this->fre = 2;
	this->path = path;

	if (st.name.str() == "global") {
		this->initGlobal(st);
	}
	else if (st.name.str()[0] == 'P') {
			this->initGPIO(st);
	}
	else if (st.name.str()[0] == 'T') {
		this->initTIM(st);
	}
	else if (st.name.str() == "main") {
		this->initMain(st);
	}
	else {
		auto s = st.name;
		error(this->path.c_str()).out(s, "未知的选择器目标");
	}
}
