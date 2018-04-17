#include "global.h"

//��ʼ���ս�����ͱ�
void init_vttype(map<string, string> &vttype);

//��ʼ��Ԥ�������mtable
void init_mtable(struct Mtable &mtable);

//�жϼ������Ƿ����strԪ��
int check_exist(vector<string> vec, string str);

//�ж�vtmap���Ƿ��и�vt
int checkmap(map<string, string> mp, string str);

//ɾ��vt����Ӧ��Ϊvn��Ԫ��
void del(vector<string> &vec, string str);

//�����ս�������Լ����ս������
void cal_vn_vt(fstream &in, fstream &out);

//���vector��������Ϣ ���vn,vt��Ϣ���ļ�
void output_vec(fstream &out);

//��first��
void cal_first();

//��follow��
void cal_follow();

//����first follow��
void cal_first_follow(fstream &in, fstream &out);

//���first,follow�����ļ�
void output_ff(fstream &out);

//����Ԥ�������
void cal_mtable(fstream &in, fstream &out);

//��ʼ��ջ
void stack_init(struct Stack &stack);

//Ԫ����ջ
void stack_pushin(struct Stack &stack, string str);

//��ȡջ��Ԫ��
string stack_gettop(struct Stack stack);

//ջ��Ԫ�س�ջ
void stack_popout(struct Stack &stack);

//���ջ����Ϣ
void stack_show(struct Stack stack, fstream &out);

//��ʼ���﷨��
void tree_init();

//�����﷨������
void make_tree(fstream &in, fstream &out);

//����м����
void printmidcode(fstream &out);

//��ʼ�����嶯����
void init_funcmap();

//�м��������
void midcode(fstream &in, fstream &out);