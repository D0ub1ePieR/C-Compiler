#pragma once
#include "global.h"

//种别码syn	单词符号symbol	类型type	|种别码syn	单词符号symbol	类型type
//0			#				end		|14			<=				operator
//1			else			keyword	|15			>=				operator
//2			if				keyword |16			==				operator
//3			int				keyword |17			!=				operator
//4			return			keyword |18			(				operator
//5			void			keyword |19			)				operator
//6			while			keyword |20			[				array
//7			+				operator|21			]				array
//8			-				operator|22			;				line_end
//9			*				operator|23			,				dot
//10		/				operator|24			{				struct
//11		<				operator|25			}				struct
//12		>				operator|26			letter|letter*	id
//13		=				operator|27			digit|digit*	num

//COP 比较操作符:		<	<=	>	>=	==	!=
//AOP 赋值操作符:		=
//OOP 运算操作符:		+	-	*	/
//EOP 句末操作符:		;
//SOP 结构分隔符:		(	)	,	[	]	{	}
//RESERVED 保留字:	int if else return void while

//保留字字符表
const string key[6] = { "else","if","int","return","void","while" };
//记录各个token所在的行与列
static int line = 1;		
static int column = 1;		

int isBC(char ch);		//判断是否为空白字符
int Reserve(string strToken);
void Retract(fstream &source);
void lexicalanalysis(fstream &in, fstream &out);	//词法分析
void fixlex(void);