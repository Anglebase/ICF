#include "Link.h"
#include<algorithm>
#include<set>
#include"error.h"
#include<iostream>

int get(int a, int b);

Link::Link(const char* filePath):code(Explain(filePath).getCode())
{
	this->NVICInit = "";
	this->INPTDeal = "";
	this->NVICGroup = 0;
	this->CheckNVIC();
	this->OutputNVICCode();
	this->OutputINPTCode();

#ifdef DEBUG
	for (auto& p : this->code) {
		std::cout << p.second << std::endl;
	}

	std::cout << this->NVICInit << std::endl;
	std::cout << this->INPTDeal << std::endl;
#endif // DEBUG

}

// 检查优先级分配是否存在错误
void Link::CheckNVIC()
{
	std::set<int> line_set;
	// 检查中断线分配是否存在重复
	for (auto& p : signSet::priority) {
		if (line_set.find(p.first.second) == line_set.end()) {
			line_set.insert(p.first.second);
		}
		else {
#ifdef DEBUG
			std::cout << "NVIC中断线重复: " << "LINE"<< p.first.second << std::endl;
#else
			std::cout << "出错：中断线分配冲突" << std::endl;
			std::cout << "LINE" << p.first.second << "被多次分配" << std::endl;
			exit(0);
#endif // DEBUG
		}
	}
	// 对做左先级进行排序并重新赋予连续值
	int p_max, s_max;
	std::sort(signSet::priority.begin(), signSet::priority.end(),
		[](std::pair<std::pair<char, int>, std::pair<int, int>> a,
			std::pair<std::pair<char, int>, std::pair<int, int>> b) {
				return a.second.first < b.second.first;
		});
	int count = -1,last=-1;
	for (auto& every : signSet::priority) {
		if (every.second.first == last); else count++;
		last = every.second.first;
		every.second.first = count;
	}
	p_max = count;
	// 对右优先级进行排序并重新分配连续值
	std::sort(signSet::priority.begin(), signSet::priority.end(),
		[](std::pair<std::pair<char, int>, std::pair<int, int>> a,
			std::pair<std::pair<char, int>, std::pair<int, int>> b) {
				return a.second.second < b.second.second;
		});
	count = -1, last = -1;
	for (auto& every : signSet::priority) {
		if (every.second.second == last); else count++;
		last = every.second.second;
		every.second.second = count;
	}
	s_max = count;
	// 检查优先级分配是否存在重复
	for (int index = 0; index < signSet::priority.size() - 1; index++) {
		if (signSet::priority[index].second.first == 
			signSet::priority[index + 1].second.first &&
			signSet::priority[index].second.second == 
			signSet::priority[index + 1].second.second) {
			std::cout << "出错：中断线优先级分组存在重复" << std::endl;
			std::cout << "    P" << signSet::priority[index].first.first << signSet::priority[index].first.second << " 与 P" << signSet::priority[index + 1].first.first << signSet::priority[index + 1].first.second << " 优先级分组重复" << std::endl;
#ifndef DEBUG
			exit(0);
#endif // DEBUG
		}
	}
	// 获取优先级分组方式
	int g = get(p_max, s_max);
	if (g == -1) {
		// 优先级分配不合理时
		std::cout << "出错：中断线优先级分配不合理" << std::endl;
#ifndef DEBUG
		exit(0);
#endif // DEBUG
	}
	else {
		this->NVICGroup = g;
	}
}

// 优先级分组
int get(int a, int b) {
	for (int i = 0; i < 5; i++) {
		if (a < (1 << i) && b < (1 << (4 - i))) {
			return i;
		}
	}
	return -1;
}

// 输出链接后的NVIC初始化代码
void Link::OutputNVICCode()
{
	this->NVICInit << "NVIC_PriorityGroupConfig(NVIC_PriorityGroup_" 
		<< this->NVICGroup << ");\n";
	std::sort(signSet::priority.begin(), signSet::priority.end(),
		[](std::pair<std::pair<char, int>, std::pair<int, int>> a,
			std::pair<std::pair<char, int>, std::pair<int, int>> b) {
				return a.first.second < b.first.second;
		});
	std::sort(signSet::interrupt.begin(), signSet::interrupt.end(),
		[](std::pair<std::pair<char, int>, std::string> a,
			std::pair<std::pair<char, int>, std::string> b) {
				return a.first.second < b.first.second;
		});
	for (auto& e : signSet::priority) {
		if (e.first.second <= 4) {
			this->N1_4.push_back(e);
		}
		else if (e.first.second >= 5 && e.first.second <= 9) {
			this->N9_5.push_back(e);
		}
		else if (e.first.second >= 10 && e.first.second <= 15) {
			this->N15_10.push_back(e);
		}
	}
	for (auto& e : signSet::interrupt) {
		if (e.first.second <= 4) {
			this->I1_4.push_back(e);
		}
		else if (e.first.second >= 5 && e.first.second <= 9) {
			this->I9_5.push_back(e);
		}
		else if (e.first.second >= 10 && e.first.second <= 15) {
			this->I15_10.push_back(e);
		}
	}
	if (!this->I9_5.empty()) {
		if (this->N9_5.size() > 1) {
			std::cout << "注：对中断线5-9的优先级进行了多次分配，将以 P" << this->N9_5[0].first.first
				<< this->N9_5[0].first.second << " 的优先级分配为准";
		}
		else if(this->N9_5.empty()) {
			std::cout << "出错：未对中断线5-9中的任何一个分配优先级" << std::endl;
#ifndef DEBUG
			exit(0);
#endif // DEBUG
		}
	}
	if (!this->I15_10.empty()) {
		if (this->N15_10.size() > 1) {
			std::cout << "注：对中断线10-15的优先级进行了多次分配，将以 P"
				<< this->N15_10[0].first.first
				<< this->N15_10[0].first.second << " 的优先级分配为准" << std::endl;
		}
		else if (this->N15_10.empty()) {
			std::cout << "出错：未对中断线10-15中的任何一个分配优先级" << std::endl;
#ifndef DEBUG
			exit(0);
#endif // DEBUG
		}
	}
	for (auto& key : signSet::priority)
		this->NVICInit << "GPIO_EXTILineConfig(GPIO_PortSourceGPIO"
		<< key.first.first << ", GPIO_PinSource"
		<< key.first.second << ");\n";

	if (!this->N1_4.empty()) {
		for (auto& o : this->N1_4) {
			if (!signSet::is_haven_NVICInit) {
				this->NVICInit << "NVIC_InitTypeDef NVIC_InitStructure;\n";
				signSet::is_haven_NVICInit = true;
			}
			this->NVICInit << "NVIC_InitStructure.NVIC_IRQChannel = EXTI"
				<< o.first.second << "_IRQn;\n"
				<< "NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;\n"
				<< "NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = "
				<< o.second.first << ";\n"
				<< "NVIC_InitStructure.NVIC_IRQChannelSubPriority = "
				<< o.second.second << ";\n"
				<< "NVIC_Init(&NVIC_InitStructure);\n";
		}
	}
	if (!this->N9_5.empty()) {
		auto& o = this->N9_5[0];
		if (!signSet::is_haven_NVICInit) {
			this->NVICInit << "NVIC_InitTypeDef NVIC_InitStructure;\n";
			signSet::is_haven_NVICInit = true;
		}
		this->NVICInit << "NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;\n"
			<< "NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;\n"
			<< "NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = "
			<< o.second.first << ";\n"
			<< "NVIC_InitStructure.NVIC_IRQChannelSubPriority = "
			<< o.second.second << ";\n"
			<< "NVIC_Init(&NVIC_InitStructure);\n";
	}
	if (!this->N15_10.empty()) {
		auto& o = this->N15_10[0];
		if (!signSet::is_haven_NVICInit) {
			this->NVICInit << "NVIC_InitTypeDef NVIC_InitStructure;\n";
			signSet::is_haven_NVICInit = true;
		}
		this->NVICInit << "NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;\n"
			<< "NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;\n"
			<< "NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = "
			<< o.second.first << ";\n"
			<< "NVIC_InitStructure.NVIC_IRQChannelSubPriority = "
			<< o.second.second << ";\n"
			<< "NVIC_Init(&NVIC_InitStructure);\n";
	}
}

// 输出链接后的中断函数代码
void Link::OutputINPTCode()
{
	this->INPTDeal << "// interrupt func\n";
	if(!this->I1_4.empty())
		for (auto& ip : this->I1_4)if (!ip.second.empty()) {
			this->INPTDeal << "void EXTI" << ip.first.second << "_IRQHandler(void){\n"
				<< ip.second << "\nEXTI_ClearITPendingBit(EXTI_Line"
				<< ip.first.second << ");\n}\n";
		}
	if (!this->I9_5.empty()) {
		this->INPTDeal << "void EXTI9_5_IRQHandler(void){\n";
		for (auto& ip : this->I9_5)if (!ip.second.empty()) {
			this->INPTDeal << "if (EXTI_GetITStatus(EXTI_Line" 
				<< ip.first.second << ") == SET){\n"
				<< ip.second << "\nEXTI_ClearITPendingBit(EXTI_Line" 
				<< ip.first.second << ");\n}\n";
		}
		this->INPTDeal << "}\n";
	}
	if (!this->I15_10.empty()) {
		this->INPTDeal << "void EXTI15_10_IRQHandler(void){\n";
		for (auto& ip : this->I15_10) if (!ip.second.empty()) {
			this->INPTDeal << "if (EXTI_GetITStatus(EXTI_Line"
				<< ip.first.second << ") == SET){\n"
				<< ip.second << "\nEXTI_ClearITPendingBit(EXTI_Line"
				<< ip.first.second << ");\n}\n";
		}
		this->INPTDeal << "}\n";
	}
}
