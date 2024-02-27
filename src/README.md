# 源代码说明-开发文档

## 基本类
```String```是对```std::string```的封装，为其扩充了```bool include(int&,const std::string)```函数并开放了部分底层接口  
```StringList```是对```std::vector<std::string>```的封装，为其扩充了```bool include(const std::string&) const```函数并开放了部分底层接口
```error```与```warning```(均在error.h中声明)分别为报错与警告，它会标识处错误发生的地方

## 依赖类
```Token```类用于存储关键词或标识符的Token标记  
```Unit```类是```Mark```、```Check```、```Explain```的共同基类，用于传递经Token标记的文档对象  
```signSet```类是对STM32GPIO的抽象表示，用于记录代码翻译过程中的'have had'状态  
```GPIOSet```(于dexp.cpp中声明)类用于对STM32GPIO设置层的抽象声明翻译  
```GPIOInpt```(于dexp.cpp中声明)类用于对STM32GPIO中断层的抽象声明翻译  
```device```(于Explain.h中声明)类被```GPIOSet```和```GPIOInpt```所依赖  

## 执行组
```ReadFileObject```类用于读入文件内容  
```Mark```类对读入内容进行关键字/标识符的Token标记  
```Check```类执行标记后内容的语法检查与冗余内容清除(空白符、注释)  
```Explain```类用于翻译已执行语法检查的代码，使ICF代码成为一个个C语言代码片段  
```Link```类用于链接EXTI中断部分的代码  
```LinkAll```类用于链接所有C语言代码  
```WriteFileObject```类用于将翻译完成的C语言代码输出至文件/控制台  

## 其它


## 源码统计信息
<pre style="font-family:'Consolas';">
      35 text files.
      35 unique files.
       0 files ignored.

github.com/AlDanial/cloc v 2.00  T=0.05 s (694.2 files/s, 54745.6 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                             16            112            208           1854
C/C++ Header                    18             48             29            484
Markdown                         1              7              0             35
-------------------------------------------------------------------------------
SUM:                            35            167            237           2373
-------------------------------------------------------------------------------
</pre>