#include "symbol.h"

struct symbol
{
	string name;		//������
	string type;		//��������
	string property;	//��������
	struct symbol *st;	//�����ͳ�Ա��ַ
};			//����Ԫ�ؽṹ��

struct func
{
	string name;		//������
	string ret;			//��������ֵ
	int num;			//�����в�������
	struct symbol st[255];	//�������ű�
};			//�����ṹ��

struct func functable[255];		//���ű�
int funcnum = 0;				//���ű���Ԫ�ظ���

								//�жϺ����Ƿ��Ѿ����� �������ڸú����ں��������е�λ�� �������򷵻�-1
int func_exist(struct func functable[255], int funcnum, string str)
{
	int i;
	for (i = 0;i < funcnum;i++)
		if (functable[i].name == str)
			return i;
	return -1;
}

//�жϱ����Ƿ�����ڵ�ǰ�����ķ��ű��� �����򷵻ر���λ�� ���򷵻�-1
int sym_exist(struct func functable[255], struct symbol *st, int num, string str)
{
	int i;
	for (i = 0;i < functable[num].num;i++)
		if ((st + i)->name == str)
			return i;
	return -1;
}

//����ǰ���������뵽��ǰ��������
void pushsym(struct func functable[255], struct symbol *st, int curnum, int num)
{
	st[functable[curnum].num].name = functable[num].name;
	st[functable[curnum].num].property = "IDF";
	st[functable[curnum].num].type = "-";
	st[functable[curnum].num].st = functable[num].st;
	functable[curnum].num++;
}

//����ǰ�������뵽��ǰ��������
void pushsym2(struct func functable[255], struct symbol *st, int curnum, int num, string name, string property, string type)
{
	st[functable[curnum].num].name = name;
	st[functable[curnum].num].property = property;
	st[functable[curnum].num].type = type;
	functable[curnum].num++;
}

//����ǰ�������뵽�������� �����ظú������ű�ĵ�ַ
struct symbol * pushfunc(struct func functable[255], int &funcnum, string str, string ret)
{
	functable[funcnum].name = str;
	functable[funcnum].num = 0;
	functable[funcnum].ret = ret;
	funcnum++;
	return functable[funcnum - 1].st;
}

//��ʼ�����ű�
void init_symtable(void)
{
	pushfunc(functable, funcnum, "input", "-");
	pushfunc(functable, funcnum, "output", "-");
}

//������ű�
void output(fstream &out)
{
	int i, j;
	int set = 20;
	//����������ű�
	out << setw(set) << left << "������" << setw(set) << "�����в�������" << setw(set) << "������������" << setw(set) << "�������ű��ַ" << endl;
	for (i = 0;i < funcnum;i++)
		out << setw(set - 1) << left << functable[i].name << setw(set - 2) << functable[i].num << setw(set - 2) << functable[i].ret << "0x" << setw(set) << functable[i].st << endl;
	//�䴦��������
	for (i = 0;i < funcnum;i++)
	{
		out << endl;
		out << "������:" << functable[i].name << "\t���ű��ַ:0x" << functable[i].st << endl;
		if (functable[i].num)
			out << setw(set) << "������" << setw(set) << "����" << setw(set) << "����" << setw(set) << "��ַ" << endl;
		for (j = 0;j < functable[i].num;j++)
			out << setw(set - 1) << functable[i].st[j].name << setw(set - 1) << functable[i].st[j].property << setw(set - 1) << functable[i].st[j].type << setw(set) << functable[i].st[j].st << endl;
	}
}

//���ɷ��ű�
void calsymboltable(fstream &in, fstream &out)
{
	vector<Token> tokens;
	Token token;
	vector<Token>::iterator titer;
	struct symbol *cur = NULL;
	int flag1, flag2, num, curnum;
	//����ʷ��������
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


	//��⵱ǰtoken������Ϊ���������߱����������ݱ�����ŵ�flag1.flag2�ж�λ�� 
	for (titer = tokens.begin();titer != tokens.end();titer++)
	{
		if (titer->type == "IDF")	//��ǰtokenΪ������,��Ϊ�����Ķ���͵������ 
		{
			num = func_exist(functable, funcnum, titer->re);
			if (flag1 == 0)//֮ǰ������������ţ�˵����ǰΪ�����Ķ���
			{
				if (num != -1)//�Ѵ��ڸú�����
				{
					cur = functable[num].st;  //�õ��ú������ڷ��ű��е���Ϣ
					curnum = num;
				}
				else     //�����ڸú�����,д��ú����� 
				{
					cur = pushfunc(functable, funcnum, titer->re, (titer - 1)->re);
					curnum = funcnum - 1;
				}
			}
			else if (flag1>0)   //˵���Ǻ����ڲ��Ժ����ĵ���
			{
				if (num == -1)	//�������ű���δ���� ����û�б�����ͱ����� ����
				{
					cout << "Undefined Function [" << titer->re << "] at Line " << titer->line << " Colume " << titer->colume << endl;
					exit(0);
				}
				else//�Ѵ��ڸú��������ж��Ƿ�Ϊ�״ε��� �����������ǰ�����ı�������
				{
					if (sym_exist(functable, cur, curnum, titer->re) == -1)
						pushsym(functable, cur, curnum, num);
				}
			}
		}

		else if (titer->type == "ID")											//��ǰtokenΪ����������Ϊ�����������͵������ 
		{
			if ((titer - 1)->re == "void" || (titer - 1)->re == "int")		//ǰһ��tokenΪint��void��˵����ǰΪ���������� 
			{
				if (flag1 == 0 || (flag1 > 0 && flag2 == 0)) 				//��������ֻ���ں�����ȡ�β��л�������λ�ں����ڲ���С������ 
				{
					if (sym_exist(functable, cur, curnum, titer->re) == -1) 	//�����ڸñ��������壬��� 
						pushsym2(functable, cur, curnum, num, titer->re, "DEFINE", (titer - 1)->re);
					else														//���ڸñ������������ 
					{
						cout << "Variable [" << titer->re << "] has been defined!" << endl;
						exit(0);
					}
				}
				else
				{
					cout << "//���������ڴ����λ�� ";
				}
			}
			else															//ǰһ��token����int��void��˵��Ϊ�����ĵ��� 
			{
				if (sym_exist(functable, cur, curnum, titer->re) == -1)			//���ű��в����ڣ�˵��δ�����ʹ��
				{
					cout << "Undefined Variable [" << titer->re << "] at Line " << titer->line << " Colume " << titer->colume << endl;
					exit(0);
				}
				/* else 														//���ű��д���
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

