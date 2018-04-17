#pragma once
#include "global.h"

//�ֱ���syn	���ʷ���symbol	����type	|�ֱ���syn	���ʷ���symbol	����type
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

//COP �Ƚϲ�����:		<	<=	>	>=	==	!=
//AOP ��ֵ������:		=
//OOP ���������:		+	-	*	/
//EOP ��ĩ������:		;
//SOP �ṹ�ָ���:		(	)	,	[	]	{	}
//RESERVED ������:	int if else return void while

//�������ַ���
const string key[6] = { "else","if","int","return","void","while" };
//��¼����token���ڵ�������
static int line = 1;		
static int column = 1;		

int isBC(char ch);		//�ж��Ƿ�Ϊ�հ��ַ�
int Reserve(string strToken);
void Retract(fstream &source);
void lexicalanalysis(fstream &in, fstream &out);	//�ʷ�����
void fixlex(void);