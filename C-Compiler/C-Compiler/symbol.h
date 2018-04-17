#pragma once
#include "global.h"

//������ű�
void output(fstream &out);

//���ɷ��ű�
void calsymboltable(fstream &in, fstream &out);

//�жϺ����Ƿ��Ѿ����� �������ڸú����ں��������е�λ�� �������򷵻�-1
int func_exist(struct func functable[255], int funcnum, string str);

//�жϱ����Ƿ�����ڵ�ǰ�����ķ��ű��� �����򷵻ر���λ�� ���򷵻�-1
int sym_exist(struct func functable[255], struct symbol *st, int num, string str);

//����ǰ���������뵽��ǰ��������
void pushsym(struct func functable[255], struct symbol *st, int curnum, int num);

//����ǰ�������뵽�������� �����ظú������ű�ĵ�ַ
struct symbol * pushfunc(struct func functable[255], int &funcnum, string str, string ret);

//��ʼ�����ű�
void init_symtable(void);

//����ǰ�������뵽��ǰ��������
void pushsym2(struct func functable[255], struct symbol *st, int curnum, int num, string name, string property, string type);