# ICF语言 ——以简洁为目标

## 简介

ICF是一门用于嵌入式编程开发的语言，它相对于传统嵌入式开发语言说，更加的简介明了，使用者只需要关心它的运行逻辑，而不需要考虑代码需要符合某个条件或规矩，如此来说，它更像是一个辅助工具。它目前支持：

系列：
+ STM32F10xxxx

功能：
+ GPIO(通用输入输出接口)初始化设置与逻辑嵌入

+ EXTI(外部中断事件控制器)初始化设置与逻辑嵌入


版本记录：

|版本|完成日期|
|---:|:---:|
|1.0|2023-8-31|

## 使用说明

### 概述
+ ICF是对大小写敏感的

+ 除注释内容外，其他必须为ASCII字符

+ []中所包含的是由C源代码扩展而来的ICC代码


### ICC代码
它与原生C语言基本相同，但比原生C多了关键字表达式，关键字表达式的语法为“ (关键字) (操作目标); ”，它本身既可以当做一条语句，也可作为一个表达式使用，它与原生C相同，作为语句时最后必须追加分号。


#### 新增关键字表达式


|关键字|返回值|描述|
|:--:|:--:|:--|
|set|void|将操作目标置为高电平|
|reset|void|将操作目标置为低电平|
|turn|None|翻转操作目标的电平|
|get|int/bool|获取操作目标的输出电平作为自身返回值|
|read|int/bool|获取操作目标输入电平作为自身返回值|

_注：turn关键字类似对set、reset和get的封装，自身不存在返回值(包括void)，不可嵌套在表达式中



_

### 注释
ICF语言中支持的注释类型有三种，分别是：
```C
// 单行注释
/*
多行注释
*/
```
以及
```python
# 单行注释
```
它完全兼容 C 语言的注释方式，并且额外提供了 Python 的注释方式，以用于对注释进行区分
### 端口选择器
端口选择器即允许多选，也允许单选。

#### 对于GPIO而言
单选选择器的表示方式为‘PXn’，其中‘P’为固定项，‘X’是所对应GPIO组，通常是‘A-G’的一个字母，‘n’为端口编号，如“PA3”所选择的就是(GPIOA，GPIO_Pin_3)，“PB11”所选择的就是(GPIOB，GPIO_Pin_11)。

多选选择器与单选选择器类似，每组开始固定为P，P之后跟随所在组名，之后跟随其端口号。
其中有两种特殊符号‘-’与‘,’，‘-’允许以两端的数字为起止点，枚举其中包含的所有数字，‘,’作为分隔符，可以选择不连续的数字。
如多选选择器“PA1-3,PB2,4”所选择的端口为PA1,PA2,PA3,PB2,PB4。

#### 特别说明
ICF存在两个虚拟选择器：global 和 main。
虚拟选择器中仅有code一个允许设置的属性。

global 是全局域选择器，在其中的代码域编写的代码对于任何一个代码域都是可访问的。

main 是主逻辑域选择器，它会成为一个循环，它的代码仅有自己可以访问到。

除了全局域之外，其他代码域均不能够定义函数。

### 属性配置器
ICF可以对选择器选择到的端口进行属性设置，它的语法为
“ (属性名) : (赋予的属性值) ; ”
其中冒号和分号不可省略。

#### 属性表
对于GPIO多选选择器而言，它只有以下三个可更改属性：
|属性名|默认值|描述|
|:---|:---:|:---|
|mode|OUT_PP|指定端口输入/输出模式|
|f|2MHz|指定端口的工作频率|
|default|0|设定端口的初始状态|

对于GPIO单选选择器而言，它还可以设置INPT(中断)属性，而INPT是一个额外嵌套结构：
|INPT属性名|默认值|描述|
|:---|:---:|:---|
|type|Rising|设置中断触发的方式|
|priority||设置中断的优先级|
|func|[ /* None */ ]|中断处理函数函数体|

对于虚拟选择器而言，他仅允许设置code属性
|属性名|默认值|描述|
|:---|:---:|:---|
|code|[ /* None */ ]|代码体|

#### 属性枚举值

+ ##### mode (工作模式)
|值|描述|
|:--|:--|
|OUT_PP|推挽输出(若不指定，则为此项)|
|OUT_OD|开漏输出|
|AF_PP|复用推挽输出|
|AF_OD|复用开漏输出|
|AIN|模拟输入|
|IPU|上拉输入|
|IPD|下拉输入|
|IN_FLOATING|浮空输入|

+ ##### f (工作频率)
|值|描述|
|:--|:--|
|2MHz|(若不指定，则为此项)|
|10MHz||
|50MHz||

+ ##### default (初始状态)
|值|描述|
|:--|:--|
|0|初始状态为低电平(若不指定，则为此项)|
|1|初始状态为高电平|

+ ##### INPT (外部中断)
> + type (中断触发模式)
>
> |值|描述|
> |:--|:--|
> |Rising|上升沿触发|
> |Falling|下降沿触发|
>
> 此属性允许进行多选，每个选项之间有且只有一个空格
> 
> + priority (中断优先级)
> 
> 它由两个无符号整型数字构成，中间由一个空格分隔，前面的数字为抢占优先级，后面的数字为响应优先级，使用者只需要指定数字的大小关系，两数字的取值范围均为 0~2147483647 ，编译器在编译时会自动分析大小关系以生成合适的优先级位分组与合适的中断优先级，如果分配存在重复或不合理的情况，将会报错
> 
>  + func (中断处理)
> 
> 它的值为由 ‘[]’所包含的源代码
> 

#### 示例

##### 一
```
PA1{
    mode:OUT_PP;
    f:50MHz;
    default:1;
}
```
以上代码的功能就是将PA1初始化为推挽输出(OUT_PP)模式，并将工作频率设置为50MHz，再将其初始状态设置为高电平。

##### 二
```
PB2{
    mode:IPU;
    f:50MHz;
    INPT:{
        type:Rising Falling;
        prioirty:1 1;
        func:[
            /* C代码 */
        ];
    };
}
```
以上代码的功能就是将PB2指定为中断输入引脚，当PB2接收到上升沿或下降沿时，会执行func内的代码

#### 简易项目示例
```
global{
    code:[
        void Delay_us(uint32_t xus)
        {
        	SysTick->LOAD = 72 * xus;				//设置定时器重装值
        	SysTick->VAL = 0x00;					//清空当前计数值
        	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
        	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
        	SysTick->CTRL = 0x00000004;				//关闭定时器
        }
    ];
}
PB1{
    mode:OUT_PP;
    f:50MHz;
    default:0;
}
PB2{
    mode:OUT_PP;
    f:50MHz;
    default:1;
}
PC13{
    mode:OUT_PP;
    f:50MHz;
    default:0;
}
PA2{
    mode:IPU;
    f:50MHz;
    INPT:{
        type:Rising;
        priority:0 0;
        func:[
            turn PB1;
            turn PB2;
        ];
    }
}
main{
    code:[
        Delay_us(5000000);
        turn PC13;
    ];
}
```
硬件部分：在PB1、PB2、PC13的端口接入LED，PA2端口接入开关。
上述代码的功能是在PC13固定闪烁LED，并捕捉PA2端口的上升沿，捕获到后翻转PB1和PB2端口电平

```C
#include "stm32f10x.h"
/* global--> */

void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 72 * xus;   //
    SysTick->VAL = 0x00;        //
    SysTick->CTRL = 0x00000005; //
    while (!(SysTick->CTRL & 0x00010000))
        ;                       //
    SysTick->CTRL = 0x00000004; //
}

/* <--global */
int main(void)
{
    /* PB1--> */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    /* <--PB1 */
    /* PB2--> */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_2);
    /* <--PB2 */
    /* PC13--> */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    /* <--PC13 */
    /* PA2--> */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStructure);
    /* <--PA2 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    while (1)
    {
        /* main--> */

        Delay_us(5000000);
        if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)
            GPIO_SetBits(GPIOC, GPIO_Pin_13);
        else
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);

        /* <--main */
    }
}
// interrupt func
void EXTI2_IRQHandler(void)
{

    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1) == 0)
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
    else
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_2) == 0)
        GPIO_SetBits(GPIOB, GPIO_Pin_2);
    else
        GPIO_ResetBits(GPIOB, GPIO_Pin_2);

    EXTI_ClearITPendingBit(EXTI_Line2);
}
```
