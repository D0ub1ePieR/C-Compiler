#pragma once
#include "global.h" 

//ʹ������״̬�Զ�����ʵ�ֹ���

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

//ɾ������������ע��
void delzs(fstream &in, fstream &out);

//start--status:0
//status:1	readin-ch
//		/blank	goto-2	--
//		other	goto-1	output-ch
//status:2	readin-ch
//		/blank	goto-2	--
//		other	goto-1	output-pre and ch

//ɾ�����������ж���Ŀո�,tab�Լ��س�
void delblank(fstream &in, fstream &out);

//������ݹ��Լ�����������
void delleftrecursion(fstream &in, fstream &out);

//���﷨����|���
void splitgrammar(fstream &in, fstream &out);
