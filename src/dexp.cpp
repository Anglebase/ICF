#include"GPIOInit.h"
#include"error.h"

/// @brief 复制指定段字符串
/// @param src 源字符串
/// @param first 起始字符
/// @param end 终止字符，支持负数索引
/// @return 复制的字符串
std::string copy(std::string src, int first = 0, int end = -1) {
	std::string s;
	for (int index = first; index <= (end >= 0 ? end : src.length() + end); index++) {
		s += src[index];
	}
	return s;
}

/// @brief 判断字符是否包含于字符串
/// @param src 源字符串
/// @param ch 判断的字符
/// @return 是/否
bool induce(std::string src, char ch) {
	for (auto& c : src) {
		if (ch == c)return true;
	}
	return false;
}

/// @brief 判断字符是否为数字
/// @param s 字符
/// @return 是/否
bool isNumber(std::string s) {
	for (auto& c : s) {
		if (c < '0' || c > '9')return false;
	}
	return true;
}

/// @brief 按照指定token类型分割token列表
/// @param src 源列表
/// @param sign 类型
/// @return 分割后的对象
std::vector<std::vector<Token>> splitByType(std::vector<Token> src, TOKENS sign) {
	std::vector<std::vector<Token>> res;
	std::vector<Token> rese;
	for (auto& t : src) {
		if (t.type() == sign) {
			res.push_back(rese);
			rese.clear();
		}
		else {
			rese.push_back(t);
		}
	}
	res.push_back(rese);
	return res;
}
/// @brief 按索引起止位置复制token列表
/// @param src 源对象
/// @param first 起始位置
/// @param end 终止位置
/// @return 复制后的token列表
std::vector<Token> copy(std::vector<Token> src, int first = 0, int end = -1) {
	std::vector<Token> res;
	for (int index = first; index <= (end >= 0 ? end : src.size() + end); index++) {
		res.push_back(src[index]);
	}
	return res;
}
/// @brief 解析虚拟global选择器
/// @param st global选择器
void device::initGlobal(const Struct& st)
{
	this->dt = G;
	auto res = splitByType(st.code, ApartFor);
	for (auto& s : res) {
		auto resc = splitByType(s, AttrLR);
		if (!resc.empty() && !resc[0].empty() && resc[0][0].str() == "code") {
			this->self_code += "/* global--> */\n";
			for (auto& t : resc[1]) {
				if (t.type() != MidLeft && t.type() != MidRight) {
					this->self_code += t.str();
				}
			}
			this->self_code += "\n/* <--global */";
		}
	}
}

/// @brief 多选选择器解析
class GPIOSet {
public:
	GPIOSet(const Struct& st) {
		this->st = st;
		this->mode = OUT_PP;
		this->default_ = false;
		this->fre = 2;
		this->is_init_fre = false;
		this->is_init_mode = false;

		this->ls = this->exp(st.name);
		for (auto& n : this->ls) {
			if (n.second >= 16) {
#ifdef DEBUG
				std::cout << "无法解析的选择器 " << this->st.name << std::endl;
#else
				error(this->st.name.getPath().c_str()).out(this->st.name, "无法解析的选择器");
#endif // !DEBUG

			}
		}
		this->explain(st.code);
		
#ifdef DEBUG
		for (auto& e : this->ls) {
			std::cout << e.first << " " << e.second << std::endl;
		}
		std::cout << std::endl;
#endif
	}

	/// @brief 代码生成
	/// @return 代码
	std::string toCode() {
		if (!this->is_init_fre) {
#ifdef DEBUG
			std::cout << "未对其分配频率，已赋予其默认值 2MHz " << st.name << std::endl;
#else
			warning(this->st.name.getPath().c_str())
				.out(this->st.name, "未对其分配频率，已赋予其默认值 2MHz");
#endif // DEBUG
		}
		if (!this->is_init_mode) {
#ifdef DEBUG
			std::cout << "未对其分配IO模式，已赋予其默认值 OUT_PP " << st.name << std::endl;
#else
			warning(this->st.name.getPath().c_str())
				.out(this->st.name, "未对其分配IO模式，已赋予其默认值 OUT_PP");
#endif // DEBUG
		}
		std::string res = "";
		for (auto& gn : this->ls) {
			res << "/* P" << gn.first << gn.second << "--> */\n";
			// RCC时钟启用
			switch (gn.first)
			{
			case 'A':
				if (!signSet::enable_rcc_GPIOA) {
					signSet::enable_rcc_GPIOA = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);\n";
				}
				break;
			case 'B':
				if (!signSet::enable_rcc_GPIOB) {
					signSet::enable_rcc_GPIOB = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);\n";
				}
				break;
			case 'C':
				if (!signSet::enable_rcc_GPIOC) {
					signSet::enable_rcc_GPIOC = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);\n";
				}
				break;
			case 'D':
				if (!signSet::enable_rcc_GPIOD) {
					signSet::enable_rcc_GPIOD = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);\n";
				}
				break;
			case 'E':
				if (!signSet::enable_rcc_GPIOE) {
					signSet::enable_rcc_GPIOE = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);\n";
				}
				break;
			case 'F':
				if (!signSet::enable_rcc_GPIOF) {
					signSet::enable_rcc_GPIOF = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);\n";
				}
				break;
			case 'G':
				if (!signSet::enable_rcc_GPIOG) {
					signSet::enable_rcc_GPIOG = true;
					res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);\n";
				}
				break;
			}
			// 端口初始化
			if (!signSet::is_haven_GPIOInit) {
				signSet::is_haven_GPIOInit = true;
				res << "GPIO_InitTypeDef GPIO_InitStructure;\n";
			}
			res << "GPIO_InitStructure.GPIO_Mode = GPIO_Mode_" << this->toStr() << ";\n"
				"GPIO_InitStructure.GPIO_Pin = GPIO_Pin_" << gn.second << ";\n"
				"GPIO_InitStructure.GPIO_Speed = GPIO_Speed_" << this->fre << "MHz;\n"
				"GPIO_Init(GPIO" << gn.first << ", &GPIO_InitStructure);\n";
			if (!this->default_) {
				res << "GPIO_ResetBits(GPIO" << gn.first << ", GPIO_Pin_" << gn.second << ");\n";
			}
			else {
				res << "GPIO_SetBits(GPIO" << gn.first << ", GPIO_Pin_" << gn.second << ");\n";
			}
			res << "/* <--P" << gn.first << gn.second << " */\n";
		}
		return res;
	}
protected:
	// 多选选择器配置内容解析
	std::vector<std::pair<char, int>> exp(Token t) {
		for (auto& ch : t.str()) {
			if (induce("PABCDEFG0123456789,-", ch));
			else error(t.getPath().c_str()).out(t, "无法解析的选择器");
		}
		std::vector<std::pair<char, int>> res;
		char sign = '\0';
		auto g = StringList(t.str(), 'P');
		for (auto& s : g.std()) {
			if (s.empty())continue;
			if (s[0] >= 'A' && s[0] <= 'G')sign = s[0];
			else error(t.getPath().c_str()).out(t, "无法解析的选择器");
			auto news = copy(s, 1);
			if (induce(news, ',')) {
				auto every = StringList(news, ',');
				for (auto& one : every.std()) {
					if (one.empty())continue;
					if (induce(one, '-')) {
						auto inte = StringList(one, '-');
						if(inte.std().size()!=2)error(t.getPath().c_str()).out(t, "不完整的选择器");
						for (int i = atoi(inte[0].c_str()); i <= atoi(inte[1].c_str()); i++) {
							res.push_back(std::pair<char, int>(sign, i));
						}
					}
					else {
						res.push_back(std::pair<char, int>(sign, atoi(one.c_str())));
					}
				}
			}
			else if(induce(news,'-')) {
				auto inte = StringList(news, '-');
				if (inte.std().size() != 2)error(t.getPath().c_str()).out(t, "不完整的选择器");
				for (int i = atoi(inte[0].c_str()); i <= atoi(inte[1].c_str()); i++) {
					res.push_back(std::pair<char, int>(sign, i));
				}
			}
			else if(isNumber(news)){
				res.push_back(std::pair<char, int>(sign, atoi(news.c_str())));
			}
			else error(t.getPath().c_str()).out(t, "无法解析的选择器");
		}
		/*auto s = t.str();
		char sign = '\0';
		for (int index = 0; index < s.length(); index++) {
			if (s[index] = 'P') {
				if (index + 1 < s.length() && s[index + 1] >= 'A' && s[index + 1] <= 'G') {
					sign = s[index + 1];
					index++;
				}
				else {
					error(t.getPath().c_str()).out(t, "无法解析的选择器");
				}
			}
			if (s[index] >= '0' && s[index] <= '9') {
				if (index + 1 < s.length()) {
					if (s[index + 1] == ',') {
						res.push_back(std::pair<char, int>
							(sign, atoi((std::string("") + s[index]).c_str())));
					}
					else if (s[index + 1] == '-') {
						if (index + 2 < s.length()) {

						}
						else {
							error(t.getPath().c_str()).out(t, "选择器不完整");
						}
					}
					else if (s[index + 1] >= '0' && s[index + 1] <= '9') {

					}
				}
				else {
					res.push_back(std::pair<char, int>
						(sign, atoi((std::string("") + s[index]).c_str())));
				}
			}
		}*/
		return res;
	}
	void explain(std::vector<Token> g) {
		auto stru = splitByType(g, ApartFor);
		for (auto& part : stru) {
			if (part.empty())continue;
			auto kv = splitByType(part, AttrLR);
			if (kv[0].size() == 1) {
				if (kv[0][0].str() == "mode") {
					if (enumValues.include(kv[1][0].str())) {
#define SK(x) if (kv[1][0].str() == #x)this->mode = x,this->is_init_mode = true
						SK(OUT_OD);
						else SK(OUT_PP);
						else SK(AIN);
						else SK(IN_FALOTING);
						else SK(AF_OD);
						else SK(AF_PP);
						else SK(IPU);
						else SK(IPD);
#undef SK
					}
					else {
#ifdef DEBUG
						std::cout << "此枚举值不存在于该属性 " << kv[1][0] << std::endl;
#else
						error(kv[1][0].str().c_str()).out(kv[1][0], "此枚举值不存在于该属性");
#endif // DEBUG
					}
				}
				else if (kv[0][0].str() == "default") {
					if (kv[1][0].str() == "1") {
						this->default_ = true;
					}
					else if (kv[1][0].str() == "0") {
						this->default_ = false;
					}
					else {
#ifdef DEBUG
						std::cout << "对 default 分配了不存在的值 " << kv[1][0] << std::endl;
#else
						warning(kv[1][0].str().c_str()).out(kv[1][0],
							"对‘default’属性分配了一个不存在的值，已重新赋予默认值 0");
#endif // DEBUG
						this->default_ = false;
					}
				}
				else if(kv[0][0].str() == "f") {
					if (kv[1][0].str() == "2") {
						this->fre = 2;
						this->is_init_fre = true;
					}
					else if (kv[1][0].str() == "10") {
						this->fre = 10;
						this->is_init_fre = true;
					}
					else if (kv[1][0].str() == "50") {
						this->fre = 50;
						this->is_init_fre = true;
					}
					else {
#ifdef DEBUG
						std::cout << "对 f 分配了不存在的值 " << kv[1][0] << std::endl;
#else
						warning(kv[1][0].str().c_str()).out(kv[1][0],
							"对‘f’属性分配了一个不存在的值，已重新赋予默认值 2MHz");
#endif // DEBUG
						this->fre = 2;
					}
				}
				else if (kv[0][0].str() == "INPT") {
#ifdef DEBUG
					std::cout << "多选选择器不能配置INPT " << kv[0][0] << std::endl;
#else
					error(kv[0][0].getPath().c_str()).out(kv[0][0], "多选选择器不能配置INPT");
#endif // DEBUG
				}
				else {
#ifdef DEBUG
					std::cout << "无法解释的属性名 " << kv[0][0] << std::endl;
#else
					error(kv[0][0].getPath().c_str()).out(kv[0][0], "无法解释的属性名");
#endif // DEBUG
				}
			}
			else {
#ifdef DEBUG
				std::cout << "冒号左边存在多余字符 " << kv[0][0] << std::endl;
#else
				error(kv[0][0].getPath().c_str()).out(kv[0][0], "冒号左边存在多余字符");
#endif // DEBUG

			}
		}

	}
	// 属性转换解析
	std::string toStr() {
#define CS(x) case x:mode = #x;break
		std::string mode;
		switch (this->mode)
		{
			CS(AIN);
			CS(IN_FALOTING);
			CS(IPU);
			CS(IPD);
			CS(AF_OD);
			CS(AF_PP);
		case OUT_PP:
			mode = "Out_PP";
			break;
		case OUT_OD:
			mode = "Out_OD";
			break;
		}
		return mode;
#undef CS
	}
private:
	std::vector<std::pair<char, int>> ls;
	IO mode;
	bool default_;
	int fre;

	bool is_init_mode;
	bool is_init_fre;

	Struct st;
};

/// @brief 单选选择器
class GPIOinpt {
public:
	GPIOinpt(const Struct& st) {
		// 为结构赋予默认属性值
		this->st = st;
		this->mode = OUT_PP;
		this->default_ = false;
		this->fre = 2;
		this->type = Rising;
		this->have_inpt = false;
		this->is_init_fre = false;
		this->is_init_mode = false;
		this->is_init_type = false;

		this->inpt_struct = "";
		this->obj_io = this->exp();


		if (obj_io.second >= 16) {
#ifdef DEBUG
			std::cout << "无法解析的选择器 " << this->st.name << std::endl;
#else
			error(this->st.name.getPath().c_str()).out(this->st.name, "无法解析的选择器");
#endif // !DEBUG

		}

		this->explain();

	}
	// 此代码会包含中断处理函数的代码，以 @ 符分隔 (init_c @ inpt_c)
	std::string toCode() {
		if (!this->is_init_fre) {
#ifdef DEBUG
			std::cout << "未分配频率 " << this->st.name << std::endl;
#else
			warning(this->st.name.getPath().c_str())
				.out(this->st.name, "未对此对象分配频率，已赋予其默认值 2MHz");
#endif // DEBUG
		}
		if (!this->is_init_mode) {
#ifdef DEBUG
			std::cout << "未分配模式 " << this->st.name << std::endl;
#else
			warning(this->st.name.getPath().c_str())
				.out(this->st.name, "未对此对象分配工作模式，已赋予其默认值 OUT_PP");
#endif // DEBUG
		}
		if (this->have_inpt && !this->is_init_type) {
#ifdef DEBUG
			std::cout << "未分配中断触发方式 " << this->st.name << std::endl;
#else
			warning(this->st.name.getPath().c_str())
				.out(this->st.name, "未对此存在中断的对象分配中断触发方式，已赋予其默认值 Rising");
#endif // DEBUG
		}
		std::string res;
		res << "/* "  << this->st.name.str() << "--> */\n";
		// RCC时钟启用
		switch (this->obj_io.first)
		{
		case 'A':
			if (!signSet::enable_rcc_GPIOA) {
				signSet::enable_rcc_GPIOA = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);\n";
			}
			break;
		case 'B':
			if (!signSet::enable_rcc_GPIOB) {
				signSet::enable_rcc_GPIOB = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);\n";
			}
			break;
		case 'C':
			if (!signSet::enable_rcc_GPIOC) {
				signSet::enable_rcc_GPIOC = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);\n";
			}
			break;
		case 'D':
			if (!signSet::enable_rcc_GPIOD) {
				signSet::enable_rcc_GPIOD = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);\n";
			}
			break;
		case 'E':
			if (!signSet::enable_rcc_GPIOE) {
				signSet::enable_rcc_GPIOE = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);\n";
			}
			break;
		case 'F':
			if (!signSet::enable_rcc_GPIOF) {
				signSet::enable_rcc_GPIOF = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);\n";
			}
			break;
		case 'G':
			if (!signSet::enable_rcc_GPIOG) {
				signSet::enable_rcc_GPIOG = true;
				res += "RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);\n";
			}
			break;
		}
		// 端口初始化
		if (!signSet::is_haven_GPIOInit) {
			signSet::is_haven_GPIOInit = true;
			res << "GPIO_InitTypeDef GPIO_InitStructure;\n";
		}
		res << "GPIO_InitStructure.GPIO_Mode = GPIO_Mode_" << this->toStr() << ";\n"
			<< "GPIO_InitStructure.GPIO_Pin = GPIO_Pin_" << this->obj_io.second << ";\n"
			<< "GPIO_InitStructure.GPIO_Speed = GPIO_Speed_" << this->fre << "MHz;\n"
			<< "GPIO_Init(GPIO" << this->obj_io.first << ", &GPIO_InitStructure);\n";
		if (!this->default_) {
			res << "GPIO_ResetBits(GPIO" << this->obj_io.first << ", GPIO_Pin_" 
				<< this->obj_io.second << ");\n";
		}
		else {
			res << "GPIO_SetBits(GPIO" << this->obj_io.first << ", GPIO_Pin_" 
				<< this->obj_io.second << ");\n";
		}
		// EXTI
		if (have_inpt) {
			if (!signSet::enable_AFIO) {
				res << "RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);\n";
				signSet::enable_AFIO = true;
			}
			if (!signSet::is_haven_EXTIInit) {
				res << "EXTI_InitTypeDef EXTI_InitStructure;\n";
				signSet::is_haven_EXTIInit = true;
			}
			res << "EXTI_InitStructure.EXTI_Line = EXTI_Line" << this->obj_io.second << "; \n"
				<< "EXTI_InitStructure.EXTI_LineCmd = ENABLE;\n"
				<< "EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;\n"
				<< "EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_" 
				<< this->EXTItoStr() << ";\n"
				<< "EXTI_Init(&EXTI_InitStructure);\n";
		}
		res << "/* <--" << this->st.name.str() << " */\n";
		if(this->have_inpt)
			signSet::interrupt.push_back(std::pair<std::pair<char, int>, std::string>
				(this->obj_io, this->inpt_struct)
			);
		return res;
	}
protected:
	std::pair<char, int> exp() {
		if (st.name.str()[0] == 'P' &&
			(st.name.str()[1] >= 'A' && st.name.str()[1] <= 'G') &&
			isNumber(copy(st.name.str(), 2))) {
			return std::pair<char, int>(st.name.str()[1], atoi(copy(st.name.str(), 2).c_str()));
		}
		else {
#ifdef DEBUG
			std::cout << "不存在的选择器目标 " << st.name << std::endl;
#else
			error(st.name.getPath().c_str()).out(st.name, "不存在的选择器目标");
#endif // DEBUG
		}
		return std::pair<char, int>();
	}

	void explain() {
		auto parts = splitByType(this->st.code, ApartFor);
		for (auto& part : parts) {
			if (part.empty())continue;
			auto pt = splitByType(part, AttrLR);
			if(pt[0].size()>1){
#ifdef DEBUG
				std::cout << "冒号左侧存在多余字符 " << pt[0][0] << std::endl;
#else
				error(pt[0][0].getPath().c_str()).out(pt[0][0], "冒号左侧存在多余字符");
#endif // DEBUG
			}
			if (pt[0][0].str() == "mode") {
				if (enumValues.include(pt[1][0].str())) {
#define SK(x) if (pt[1][0].str() == #x)this->mode = x,this->is_init_mode = true
					SK(OUT_OD);
					else SK(OUT_PP);
					else SK(AIN);
					else SK(IN_FALOTING);
					else SK(AF_OD);
					else SK(AF_PP);
					else SK(IPU);
					else SK(IPD);
#undef SK
				}
				else {
#ifdef DEBUG
					std::cout << "此枚举值不存在于该属性 " << pt[1][0] << std::endl;
#else
					error(pt[1][0].str().c_str()).out(pt[1][0], "此枚举值不存在于该属性");
#endif // DEBUG
				}

			}
			else if (pt[0][0].str() == "default") {
				if (pt[1][0].str() == "1") {
					this->default_ = true;
				}
				else if (pt[1][0].str() == "0") {
					this->default_ = false;
				}
				else {
#ifdef DEBUG
					std::cout << "对 default 分配了不存在的值 " << pt[1][0] << std::endl;
#else
					warning(pt[1][0].str().c_str()).out(pt[1][0],
						"对‘default’属性分配了一个不存在的值，已重新赋予默认值 0");
#endif // DEBUG
					this->default_ = false;
				}
			}
			else if (pt[0][0].str() == "f") {
				if (pt[1][0].str() == "2") {
					this->fre = 2;
					this->is_init_fre = true;
				}
				else if (pt[1][0].str() == "10") {
					this->fre = 10;
					this->is_init_fre = true;
				}
				else if (pt[1][0].str() == "50") {
					this->fre = 50;
					this->is_init_fre = true;
				}
				else {
#ifdef DEBUG
					std::cout << "对 f 分配了不存在的值 " << pt[1][0] << std::endl;
#else
					warning(pt[1][0].str().c_str()).out(pt[1][0],
						"对‘f’属性分配了一个不存在的值，已重新赋予默认值 2MHz");
#endif // DEBUG
					this->fre = 2;
				}
			}
			else if (pt[0][0].str() == "INPT") {
				this->have_inpt = true;
				auto stru = copy(pt[1], 0, -2);
				auto inpart = splitByType(stru, inApartFor);
				for (auto& ipt : inpart) {
					if (ipt.empty())continue;
					auto ipn = splitByType(ipt, inAttrLR);
					if (ipn[0].size() > 1) {
#ifdef DEBUG
						std::cout << "冒号左侧存在多余字符 " << pt[0][0] << std::endl;
#else
						error(pt[0][0].getPath().c_str()).out(pt[0][0], "冒号左侧存在多余字符");
#endif // DEBUG
					}
					if (ipn[0][0].str() == "type") {
						if (ipn[1].size() == 1) {
							if (!typeEnum.include(ipn[1][0].str())) {
#ifdef DEBUG
								std::cout << "此枚举值不存在于该属性 " << ipn[1][0] << std::endl;
#else
								error(ipn[1][0].getPath().c_str())
									.out(ipn[1][0], "此枚举值不存在于该属性");
#endif // DEBUG
							}
							if (ipn[1][0].str() == "Rising") {
								this->type = Rising;
								this->is_init_type = true;
							}
							else if (ipn[1][0].str() == "Falling") {
								this->type = Falling;
								this->is_init_type = true;
							}
						}
						else if (ipn.size() == 2) {
							if (!typeEnum.include(ipn[1][0].str())) {
#ifdef DEBUG
								std::cout << "此枚举值不存在于该属性 " << ipn[1][0] << std::endl;
#else
								error(ipn[1][0].getPath().c_str())
									.out(ipn[1][0], "此枚举值不存在于该属性");
#endif // DEBUG
							}

							if (!typeEnum.include(ipn[1][1].str())) {
#ifdef DEBUG
								std::cout << "此枚举值不存在于该属性 " << ipn[1][1] << std::endl;
#else
								error(ipn[1][1].getPath().c_str())
									.out(ipn[1][1], "此枚举值不存在于该属性");
#endif // DEBUG
							}
							if (ipn[1][0].str() != ipn[1][1].str()) {
								this->type = Ris_Fall;
								this->is_init_type = true;
							}
							else {
#ifdef DEBUG
								std::cout << "枚举值重复 " << ipn[1][1] << std::endl;
#else
								error(ipn[1][1].getPath().c_str())
									.out(ipn[1][1], "枚举值重复");
#endif // DEBUG
							}
						}
						else {
#ifdef DEBUG
							std::cout << "指定了不合法的参数 " << ipn[0][0] << std::endl;
#else
							error(ipn[0][0].getPath().c_str())
								.out(ipn[0][0], "指定了不合法的参数");
#endif // DEBUG
						}
					}
					else if (ipn[0][0].str() == "priority") {
						if (ipn[1].size() == 2) {
							signSet::priority.push_back(
								std::pair<std::pair<char, int>, std::pair<int, int>>(
									this->obj_io, std::pair<int, int>(
										atoi(ipn[1][0].str().c_str()),
										atoi(ipn[1][1].str().c_str())
									)
								)
							);
						}
						else if (ipn[1].size() < 2) {
#ifdef DEBUG
							std::cout << "优先级指定不完整 " << ipn[1][0] << std::endl;
#else
							error(ipn[1][0].getPath().c_str())
								.out(ipn[1][0], "优先级指定不完整");
#endif // DEBUG
						}
						else if (ipn[1].size() > 2) {
#ifdef DEBUG
							std::cout << "优先级指定参数过多 " << ipn[1][0] << std::endl;
#else
							error(ipn[1][0].getPath().c_str())
								.out(ipn[1][0], "优先级指定参数过多");
#endif // DEBUG
						}
					}
					else  if (ipn[0][0].str() == "func") {
						auto c = copy(ipn[1], 1, -2);
						for (auto& t : c) {
							this->inpt_struct += t.str();
						}
					}
					else {
#ifdef DEBUG
						std::cout << "无法解释的属性名 " << ipn[0][0] << std::endl;
#else
						error(ipn[0][0].getPath().c_str()).out(ipn[0][0], "无法解释的属性名");
#endif // DEBUG
					}
				}
			}
			else {
#ifdef DEBUG
				std::cout << "无法解释的属性名 " << pt[0][0] << std::endl;
#else
				error(pt[0][0].getPath().c_str()).out(pt[0][0], "无法解释的属性名");
#endif // DEBUG
			}
		}
	}
	// 属性转换解析
	std::string toStr() {
#define CS(x) case x:mode = #x;break
		std::string mode;
		switch (this->mode)
		{
			CS(AIN);
			CS(IN_FALOTING);
			CS(IPU);
			CS(IPD);
			CS(AF_OD);
			CS(AF_PP);
		case OUT_PP:
			mode = "Out_PP";
			break;
		case OUT_OD:
			mode = "Out_OD";
			break;
		}
		return mode;
#undef CS
	}
	std::string EXTItoStr() {
		switch (this->type)
		{
		case Rising:
			return "Rising";
			break;
		case Falling:
			return "Falling";
			break;
		case Ris_Fall:
			return "Rising_Falling";
			break;
		}
		return "";
	}
private:
	std::pair<char, int> obj_io;
	IO mode;
	bool default_;
	int fre;

	bool is_init_mode;
	bool is_init_fre;
	bool is_init_type;
	
	Struct st;

	std::string inpt_struct;
	bool have_inpt;
	EXTI type;
};

/// @brief GPIO选择器
/// @param st GPIO选择器对象
void device::initGPIO(const Struct& st)
{
	if (st.name.str().size() > 4) {
		this->dt = GPIO;
		GPIOSet g(st);
		this->self_code = g.toCode();
	}
	else {
		this->dt = GPIOsignle;
		GPIOinpt g(st);
		this->self_code = g.toCode();
	}
}

/// @brief 定时器选择器解析
/// @param st 定时器选择器对象
void device::initTIM(const Struct& st)
{
	this->dt = TIM;

}

/// @brief 虚拟选择器main解析
/// @param st main对象
void device::initMain(const Struct& st)
{
	this->dt = M;
	auto res = splitByType(st.code, ApartFor);
	for (auto& s : res) {
		auto resc = splitByType(s, AttrLR);
		if (!resc.empty() && !resc[0].empty() && resc[0][0].str() == "code") {
			this->self_code += "/* main--> */\n";
			for (auto& t : resc[1]) {
				if (t.type() != MidLeft && t.type() != MidRight) {
					this->self_code += t.str();
				}
			}
			this->self_code += "\n/* <--main */";
		}
	}
}

/// @brief 将选择器对象编译为代码
/// @return 编译后代码
std::pair<CodeType, std::string> device::toCode() {
	switch (this->dt)
	{
	case G:
		return std::pair<CodeType, std::string>(GLOBAL, this->self_code);
		break;
	case M:
		return std::pair<CodeType, std::string>(MAIN, this->self_code);
		break;
	case GPIO:
		return std::pair<CodeType, std::string>(GPIOINIT, this->self_code);
		break;
	case GPIOsignle:
		return std::pair<CodeType, std::string>(GPIOINPT, this->self_code);
		break;
	case TIM:

		break;
	}
	return std::pair<CodeType, std::string>(NONE, "");
}