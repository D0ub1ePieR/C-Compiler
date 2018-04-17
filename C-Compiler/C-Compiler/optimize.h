#include"global.h"

//读入所有三地址代码
void getbetter1(fstream &in);

//常量优化
void getbetter2(int start, int end);

//优化中间代码
void optimize(fstream &in, fstream &out);