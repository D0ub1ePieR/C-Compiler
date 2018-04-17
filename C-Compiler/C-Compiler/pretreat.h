#pragma once
#include "global.h" 

//使用有限状态自动机来实现功能

//start--status:0
//status:1	readin-ch
//		'/'		goto-2	--
//		'*'		goto-1	output-ch
//		other	goto-1	output-ch
//status:2	readin-ch
//		'/'		goto-2	output-'/'
//		'*'		goto-3	--
//		other	goto-1	output-'/'and ch
//status:3	readin-ch
//		'/'		goto-3	--
//		'*'		goto-4	--
//		other	goto-3	--
//status:4	readin-ch
//		'/'		goto-5	--
//		'*'		goto-4	--
//		other	goto-3	--
//status:5	readin-ch
//		'/'		goto-2	--
//		'*'		goto-1	output-ch
//		other	goto-1	output-ch

//删除程序中所有注释
void delzs(fstream &in, fstream &out);

//start--status:0
//status:1	readin-ch
//		/blank	goto-2	--
//		other	goto-1	output-ch
//status:2	readin-ch
//		/blank	goto-2	--
//		other	goto-1	output-pre and ch

//删除程序中所有多余的空格,tab以及回车
void delblank(fstream &in, fstream &out);

//消除左递归以及公共左因子
void delleftrecursion(fstream &in, fstream &out);

//将语法根据|拆分
void splitgrammar(fstream &in, fstream &out);
