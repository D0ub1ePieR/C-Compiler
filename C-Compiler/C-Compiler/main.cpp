#include "global.h"

int main()
{
	cout << "resource file:" << "Saved in \"" << resource << "\"" << endl;
	divideline();
	//Ԥ���� 
	//P1 1.ɾ��ע�� 2.ɾ������հ��ַ� 
	precedure(period[1], 1, resource, file_nozs, delzs, content1[1]);
	precedure(period[1], 2, file_nozs, file_clear, delblank, content1[2]);
	//P1 �﷨���� 3.���﷨����|����  4.���first,follow��
	precedure(period[1], 3, file_grammar, file_ll1grammar, delleftrecursion, content1[3]);
	precedure(period[1], 4, file_ll1grammar, file_ad_grammar, splitgrammar, content1[4]);
	precedure(period[1], 5, file_ad_grammar, file_vn_vt,cal_vn_vt, content1[5]);
	precedure(period[1], 6, file_ad_grammar, file_ff, cal_first_follow, content1[6]);
	precedure(period[1], 7, file_ad_grammar, file_mtable, cal_mtable, content1[7]);
	//�ʷ�����
	precedure(period[2], 1, file_clear, file_lex, lexicalanalysis, content2);
	fixlex();
	//�������
	precedure(period[3], 1, file_lex, file_showstack, make_tree, content3[1]);
	precedure(period[3], 2, file_lex, file_symtable, calsymboltable, content3[2]);
	//�м��������
	precedure(period[4], 1, file_lex, file_midcode, midcode, content4[1]);
	precedure(period[4], 2, file_midcode, file_optimize, optimize, content4[2]);
	return 0;
}