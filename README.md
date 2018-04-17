# C-Compiler
一个C-的编译器

# 流程
词法分析-语法分析-语义分析-代码优化
使用有限状态自动机提取token
使用ll(1)文法分析语法
建立语法分析树 构建语义动作
生成三地址代码 使用DAG图进行优化

# 文件信息
数字开头的为各步骤输出内容
global. 定义全局变量或函数
grammar-analysis. 处理语法
lexical-analysis. 处理词法
optimize. 进行优化
pretreat. 实现各种操作的预处理
symbol. 产生输入程序的符号表

# 使用
在0 prog-resource.txt中输入原始代码
打开C-Compiler.sln编译所有代码运行
语法规则见文档
