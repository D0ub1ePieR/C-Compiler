#pragma once
#include "global.h"

//输出符号表
void output(fstream &out);

//生成符号表
void calsymboltable(fstream &in, fstream &out);

//判断函数是否已经存在 并返回在该函数在函数名表中的位置 不存在则返回-1
int func_exist(struct func functable[255], int funcnum, string str);

//判断变量是否出现在当前函数的符号表中 存在则返回表中位置 否则返回-1
int sym_exist(struct func functable[255], struct symbol *st, int num, string str);

//将当前函数名加入到当前函数表中
void pushsym(struct func functable[255], struct symbol *st, int curnum, int num);

//将当前函数加入到函数表中 并返回该函数符号表的地址
struct symbol * pushfunc(struct func functable[255], int &funcnum, string str, string ret);

//初始化符号表
void init_symtable(void);

//将当前变量加入到当前函数表中
void pushsym2(struct func functable[255], struct symbol *st, int curnum, int num, string name, string property, string type);