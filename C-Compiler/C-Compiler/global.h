#pragma once
#include "include.h"

static string resource = "0 prog-resource.txt";		//源程序
static string file_nozs = "1 prog-step1.txt";			//去注释后的文件
static string file_clear = "2 prog-clear.txt";		//预处理完成的文件
static string file_lex = "3-lexical.txt";		//词法分析结果存储文件
static string file_grammar = "4 grammar.txt";		//原始语法文件
static string file_ll1grammar = "5 ll1_grammar.txt";	//符合ll1文法的语法文件
static string file_ad_grammar = "6 ad_grammar.txt";	//处理完成的语法文件
static string file_vn_vt = "7 vn_vtlist.txt";		//终结符,非终结符存储文件
static string file_ff = "8 first_follow.txt";		//first,follow集存储文件
static string file_mtable = "9 mtable.txt";			//预测分析表存储文件
static string file_showstack = "10 stack_content.txt";	//输出栈内信息文件
static string file_symtable = "11 symbol_table.txt";	//符号表存储文件
static string file_midcode = "12 middle_code.txt";		//中间代码输出
static string file_optimize = "13 ad_midcode.txt";		//优化中间代码
static string period[7] = { "",
							"预处理/初始化",
						    "词法分析",
							"翻译程序",
							"中间代码生成"
};		//总步骤名称列表
static string content1[10] = { "",
							  "去除注释",
							  "去除空白字符",
							  "消除左递归以及公共左因子",
							  "将语法根据|拆分",
							  "求vn,vt集合",
							  "求fisrt,follow集",
							  "计算预测分析表mtable"
};		//分步名称列表
static string content2 = "词法分析";
static string content3[10] = { "",
							  "生成语法分析树",
							  "生成符号表",
};
static string content4[3] = { "",
							 "中间代码生成",
							 "中间代码优化"
};
static string call = "call ";
static string param = "param ";
static string proc = "Prog ";
static int maxline;
static const int maxsize = 255;
static const string firststr = "S";

static fstream in, out;

static map<string, string> vttype;	//终结符类型表
static vector<string> vn;			//非终结符集合
static vector<string> vt;			//终结符集合

//各类要使用到的存储器指针
static vector<string>::iterator iter;
static map<string, string>::iterator iter_map;
static map<string, vector<string>>::iterator maper;
static map<string, int>::iterator map_si;
static map<int, vector<string>> grammarmap;		//存储语法的推导
static map<int, vector<string>>::iterator gmaper;
static map<string, vector<string>> firstlist;		//first集
static map<string, vector<string>> followlist;		//follow集
static map<string, int> emptymake;			//存储所有非终结符能推导出空字的推导式编号

//token结构体
typedef struct Token
{
	string re;			//token原始字符串
	string type;		//token类型
	int line;			//token所在行
	int colume;			//token所在列
}Token;

typedef struct info
{
	int arrayflag;		//记录变量是否为数组
	int emptyflag;		//记录该非终结符是否推出空
	int whilenum1;		//记录while入口的三地址代码值
	int whilenum2;		//记录while出口的三地址代码值
	int ifnum1;			//记录if语句真出口三地址代码值
	int ifnum2;			//记录if语句假出口三地址代码值
	int elsenum1;		//记录else入口三地址代码值
	int elsenum2;		//记录else出口三地址代码值
	int returnflag;
}info;

//树结点结构体
typedef struct Gtree
{
	char re[255];		//结点元素原字符串
	char type[255];		//结点元素类型	非终结符为derivation 终结符为自身类型 根节点为head 
	int bitnum;			//结点孩子节点个数
	int mtnum;			//如为非终结符 记录推导式编号
	int nodenum;
	struct Gtree * sons[255];		//孩子节点地址数组
	struct Gtree *brother;			//兄弟节点地址
	struct Gtree *father;			//父亲节点地址
	struct info *info;				//语义信息
}treebit, *bit;

//栈结构体
typedef struct Stack
{
	vector<string> st;		//栈
	int current;			//当前栈中元素个数
}st;

//预测分析表结构体
typedef struct Mtable
{
	map<string, int>vnname;		//非终结符表
	map<string, int>vtname;		//终结符表
	int mtable[maxsize][maxsize];		//预测分析表
	int vnnum;		//记录非终结符个数
	int vtnum;		//记录终结符个数
}mt;

//打开指定文件输入/输出 返回1表示打开成功，返回0表示打开失败
int infile(fstream &in, string filenmae);
int outfile(fstream &out, string filenmae);

//关闭in,out两个用来输入输出的文件
void closefile(fstream &file1,fstream &file2);

//输出文件内容
void readfile(string filename);

//分割线
void divideline(void);

//各过程处理以及界面输出
void precedure(string period, int num, string filein, string fileout, void(*func)(fstream &, fstream &),string content);

//去除字符串前后多余的空格
void fix(string &str);