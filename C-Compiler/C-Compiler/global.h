#pragma once
#include "include.h"

static string resource = "0 prog-resource.txt";		//Դ����
static string file_nozs = "1 prog-step1.txt";			//ȥע�ͺ���ļ�
static string file_clear = "2 prog-clear.txt";		//Ԥ������ɵ��ļ�
static string file_lex = "3-lexical.txt";		//�ʷ���������洢�ļ�
static string file_grammar = "4 grammar.txt";		//ԭʼ�﷨�ļ�
static string file_ll1grammar = "5 ll1_grammar.txt";	//����ll1�ķ����﷨�ļ�
static string file_ad_grammar = "6 ad_grammar.txt";	//������ɵ��﷨�ļ�
static string file_vn_vt = "7 vn_vtlist.txt";		//�ս��,���ս���洢�ļ�
static string file_ff = "8 first_follow.txt";		//first,follow���洢�ļ�
static string file_mtable = "9 mtable.txt";			//Ԥ�������洢�ļ�
static string file_showstack = "10 stack_content.txt";	//���ջ����Ϣ�ļ�
static string file_symtable = "11 symbol_table.txt";	//���ű�洢�ļ�
static string file_midcode = "12 middle_code.txt";		//�м�������
static string file_optimize = "13 ad_midcode.txt";		//�Ż��м����
static string period[7] = { "",
							"Ԥ����/��ʼ��",
						    "�ʷ�����",
							"�������",
							"�м��������"
};		//�ܲ��������б�
static string content1[10] = { "",
							  "ȥ��ע��",
							  "ȥ���հ��ַ�",
							  "������ݹ��Լ�����������",
							  "���﷨����|���",
							  "��vn,vt����",
							  "��fisrt,follow��",
							  "����Ԥ�������mtable"
};		//�ֲ������б�
static string content2 = "�ʷ�����";
static string content3[10] = { "",
							  "�����﷨������",
							  "���ɷ��ű�",
};
static string content4[3] = { "",
							 "�м��������",
							 "�м�����Ż�"
};
static string call = "call ";
static string param = "param ";
static string proc = "Prog ";
static int maxline;
static const int maxsize = 255;
static const string firststr = "S";

static fstream in, out;

static map<string, string> vttype;	//�ս�����ͱ�
static vector<string> vn;			//���ս������
static vector<string> vt;			//�ս������

//����Ҫʹ�õ��Ĵ洢��ָ��
static vector<string>::iterator iter;
static map<string, string>::iterator iter_map;
static map<string, vector<string>>::iterator maper;
static map<string, int>::iterator map_si;
static map<int, vector<string>> grammarmap;		//�洢�﷨���Ƶ�
static map<int, vector<string>>::iterator gmaper;
static map<string, vector<string>> firstlist;		//first��
static map<string, vector<string>> followlist;		//follow��
static map<string, int> emptymake;			//�洢���з��ս�����Ƶ������ֵ��Ƶ�ʽ���

//token�ṹ��
typedef struct Token
{
	string re;			//tokenԭʼ�ַ���
	string type;		//token����
	int line;			//token������
	int colume;			//token������
}Token;

typedef struct info
{
	int arrayflag;		//��¼�����Ƿ�Ϊ����
	int emptyflag;		//��¼�÷��ս���Ƿ��Ƴ���
	int whilenum1;		//��¼while��ڵ�����ַ����ֵ
	int whilenum2;		//��¼while���ڵ�����ַ����ֵ
	int ifnum1;			//��¼if������������ַ����ֵ
	int ifnum2;			//��¼if���ٳ�������ַ����ֵ
	int elsenum1;		//��¼else�������ַ����ֵ
	int elsenum2;		//��¼else��������ַ����ֵ
	int returnflag;
}info;

//�����ṹ��
typedef struct Gtree
{
	char re[255];		//���Ԫ��ԭ�ַ���
	char type[255];		//���Ԫ������	���ս��Ϊderivation �ս��Ϊ�������� ���ڵ�Ϊhead 
	int bitnum;			//��㺢�ӽڵ����
	int mtnum;			//��Ϊ���ս�� ��¼�Ƶ�ʽ���
	int nodenum;
	struct Gtree * sons[255];		//���ӽڵ��ַ����
	struct Gtree *brother;			//�ֵܽڵ��ַ
	struct Gtree *father;			//���׽ڵ��ַ
	struct info *info;				//������Ϣ
}treebit, *bit;

//ջ�ṹ��
typedef struct Stack
{
	vector<string> st;		//ջ
	int current;			//��ǰջ��Ԫ�ظ���
}st;

//Ԥ�������ṹ��
typedef struct Mtable
{
	map<string, int>vnname;		//���ս����
	map<string, int>vtname;		//�ս����
	int mtable[maxsize][maxsize];		//Ԥ�������
	int vnnum;		//��¼���ս������
	int vtnum;		//��¼�ս������
}mt;

//��ָ���ļ�����/��� ����1��ʾ�򿪳ɹ�������0��ʾ��ʧ��
int infile(fstream &in, string filenmae);
int outfile(fstream &out, string filenmae);

//�ر�in,out������������������ļ�
void closefile(fstream &file1,fstream &file2);

//����ļ�����
void readfile(string filename);

//�ָ���
void divideline(void);

//�����̴����Լ��������
void precedure(string period, int num, string filein, string fileout, void(*func)(fstream &, fstream &),string content);

//ȥ���ַ���ǰ�����Ŀո�
void fix(string &str);