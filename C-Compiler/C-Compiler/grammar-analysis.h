#include "global.h"

//初始化终结符类型表
void init_vttype(map<string, string> &vttype);

//初始化预测分析表mtable
void init_mtable(struct Mtable &mtable);

//判断集合中是否存在str元素
int check_exist(vector<string> vec, string str);

//判断vtmap中是否有该vt
int checkmap(map<string, string> mp, string str);

//删除vt集中应该为vn的元素
void del(vector<string> &vec, string str);

//计算终结符集合以及非终结符集合
void cal_vn_vt(fstream &in, fstream &out);

//输出vector中所有信息 输出vn,vt信息至文件
void output_vec(fstream &out);

//求first集
void cal_first();

//求follow集
void cal_follow();

//计算first follow集
void cal_first_follow(fstream &in, fstream &out);

//输出first,follow集至文件
void output_ff(fstream &out);

//计算预测分析表
void cal_mtable(fstream &in, fstream &out);

//初始化栈
void stack_init(struct Stack &stack);

//元素入栈
void stack_pushin(struct Stack &stack, string str);

//获取栈顶元素
string stack_gettop(struct Stack stack);

//栈顶元素出栈
void stack_popout(struct Stack &stack);

//输出栈中信息
void stack_show(struct Stack stack, fstream &out);

//初始化语法树
void tree_init();

//生成语法分析树
void make_tree(fstream &in, fstream &out);

//输出中间代码
void printmidcode(fstream &out);

//初始化语义动作表
void init_funcmap();

//中间代码生成
void midcode(fstream &in, fstream &out);