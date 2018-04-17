#include "symbol.h"

struct symbol
{
	string name;		//变量名
	string type;		//变量类型
	string property;	//变量属性
	struct symbol *st;	//函数型成员地址
};			//变量元素结构体

struct func
{
	string name;		//函数名
	string ret;			//函数返回值
	int num;			//函数中参数个数
	struct symbol st[255];	//函数符号表
};			//函数结构体

struct func functable[255];		//符号表
int funcnum = 0;				//符号表中元素个数

								//判断函数是否已经存在 并返回在该函数在函数名表中的位置 不存在则返回-1
int func_exist(struct func functable[255], int funcnum, string str)
{
	int i;
	for (i = 0;i < funcnum;i++)
		if (functable[i].name == str)
			return i;
	return -1;
}

//判断变量是否出现在当前函数的符号表中 存在则返回表中位置 否则返回-1
int sym_exist(struct func functable[255], struct symbol *st, int num, string str)
{
	int i;
	for (i = 0;i < functable[num].num;i++)
		if ((st + i)->name == str)
			return i;
	return -1;
}

//将当前函数名加入到当前函数表中
void pushsym(struct func functable[255], struct symbol *st, int curnum, int num)
{
	st[functable[curnum].num].name = functable[num].name;
	st[functable[curnum].num].property = "IDF";
	st[functable[curnum].num].type = "-";
	st[functable[curnum].num].st = functable[num].st;
	functable[curnum].num++;
}

//将当前变量加入到当前函数表中
void pushsym2(struct func functable[255], struct symbol *st, int curnum, int num, string name, string property, string type)
{
	st[functable[curnum].num].name = name;
	st[functable[curnum].num].property = property;
	st[functable[curnum].num].type = type;
	functable[curnum].num++;
}

//将当前函数加入到函数表中 并返回该函数符号表的地址
struct symbol * pushfunc(struct func functable[255], int &funcnum, string str, string ret)
{
	functable[funcnum].name = str;
	functable[funcnum].num = 0;
	functable[funcnum].ret = ret;
	funcnum++;
	return functable[funcnum - 1].st;
}

//初始化符号表
void init_symtable(void)
{
	pushfunc(functable, funcnum, "input", "-");
	pushfunc(functable, funcnum, "output", "-");
}

//输出符号表
void output(fstream &out)
{
	int i, j;
	int set = 20;
	//输出函数符号表
	out << setw(set) << left << "函数名" << setw(set) << "函数中参数个数" << setw(set) << "函数返回类型" << setw(set) << "函数符号表地址" << endl;
	for (i = 0;i < funcnum;i++)
		out << setw(set - 1) << left << functable[i].name << setw(set - 2) << functable[i].num << setw(set - 2) << functable[i].ret << "0x" << setw(set) << functable[i].st << endl;
	//输处各函数表
	for (i = 0;i < funcnum;i++)
	{
		out << endl;
		out << "函数名:" << functable[i].name << "\t符号表地址:0x" << functable[i].st << endl;
		if (functable[i].num)
			out << setw(set) << "变量名" << setw(set) << "属性" << setw(set) << "类型" << setw(set) << "地址" << endl;
		for (j = 0;j < functable[i].num;j++)
			out << setw(set - 1) << functable[i].st[j].name << setw(set - 1) << functable[i].st[j].property << setw(set - 1) << functable[i].st[j].type << setw(set) << functable[i].st[j].st << endl;
	}
}

//生成符号表
void calsymboltable(fstream &in, fstream &out)
{
	vector<Token> tokens;
	Token token;
	vector<Token>::iterator titer;
	struct symbol *cur = NULL;
	int flag1, flag2, num, curnum;
	//读入词法分析结果
	while (!in.eof())
	{
		in >> token.re >> token.type >> token.line >> token.colume;
		if (in.fail())
			break;
		tokens.push_back(token);
	}
	flag1 = 0;
	flag2 = 0;
	init_symtable();


	//检测当前token的属性为函数名或者变量名，根据标记括号的flag1.flag2判断位置 
	for (titer = tokens.begin();titer != tokens.end();titer++)
	{
		if (titer->type == "IDF")	//当前token为函数名,分为函数的定义和调用情况 
		{
			num = func_exist(functable, funcnum, titer->re);
			if (flag1 == 0)//之前不存在左大括号，说明当前为函数的定义
			{
				if (num != -1)//已存在该函数名
				{
					cur = functable[num].st;  //得到该函数名在符号表中的信息
					curnum = num;
				}
				else     //不存在该函数名,写入该函数名 
				{
					cur = pushfunc(functable, funcnum, titer->re, (titer - 1)->re);
					curnum = funcnum - 1;
				}
			}
			else if (flag1>0)   //说明是函数内部对函数的调用
			{
				if (num == -1)	//函数符号表中未出现 函数没有被定义就被调用 报错
				{
					cout << "Undefined Function [" << titer->re << "] at Line " << titer->line << " Colume " << titer->colume << endl;
					exit(0);
				}
				else//已存在该函数名，判断是否为首次调用 是则加入至当前函数的变量表中
				{
					if (sym_exist(functable, cur, curnum, titer->re) == -1)
						pushsym(functable, cur, curnum, num);
				}
			}
		}

		else if (titer->type == "ID")											//当前token为变量名，分为变量的声明和调用情况 
		{
			if ((titer - 1)->re == "void" || (titer - 1)->re == "int")		//前一个token为int或void，说明当前为变量的声明 
			{
				if (flag1 == 0 || (flag1 > 0 && flag2 == 0)) 				//变量声明只能在函数获取形参中或不在其他位于函数内部的小括号内 
				{
					if (sym_exist(functable, cur, curnum, titer->re) == -1) 	//不存在该变量，则定义，入表 
						pushsym2(functable, cur, curnum, num, titer->re, "DEFINE", (titer - 1)->re);
					else														//存在该变量，不用入表 
					{
						cout << "Variable [" << titer->re << "] has been defined!" << endl;
						exit(0);
					}
				}
				else
				{
					cout << "//变量定义在错误的位置 ";
				}
			}
			else															//前一个token不是int或void，说明为函数的调用 
			{
				if (sym_exist(functable, cur, curnum, titer->re) == -1)			//符号表中不存在，说明未定义就使用
				{
					cout << "Undefined Variable [" << titer->re << "] at Line " << titer->line << " Colume " << titer->colume << endl;
					exit(0);
				}
				/* else 														//符号表中存在
				{
				cout << "Variable [" << titer->re << "] has been defined!" << endl;
				cout << titer->line;
				exit(0);
				}*/
			}
		}

		else if (titer->re == "{")
			flag1++;
		else if (titer->re == "}")
			flag1--;
		else if (titer->re == "(")
			flag2++;
		else if (titer->re == ")")
			flag2--;
	}
	output(out);
}

