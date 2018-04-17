#include "grammar-analysis.h"

Token token;
struct Mtable mtable;
struct Stack stack;
treebit *treenode;

int nodenum = 0;		//�ڵ���
map<int, vector<string>> placelistmap;		//���ڵ�洢�ַ�����Ϣ��

map<string, bool(*)(treebit *cur)> funcmap;		//�Ƶ�ʽ��������Ӧ�����嶯������
map<int, string> midcodemap;
map<string, int> funcnum;
int curnum = 99;		//����ַ������
int curtemp = 0;		//��ʱ��������

//��ʼ���ս�����ͱ�
void init_vttype(map<string, string> &vttype)
{
	vttype.insert({ "<","COP" });
	vttype.insert({ "<=","COP" });
	vttype.insert({ ">","COP" });
	vttype.insert({ ">=","COP" });
	vttype.insert({ "==","COP" });
	vttype.insert({ "!=","COP" });
	vttype.insert({ "=","AOP" });
	vttype.insert({ "+","OOP" });
	vttype.insert({ "-","OOP" });
	vttype.insert({ "*","OOP" });
	vttype.insert({ "/","OOP" });
	vttype.insert({ ";","EOP" });
	vttype.insert({ ",","SOP" });
	vttype.insert({ "(","SOP" });
	vttype.insert({ ")","SOP" });
	vttype.insert({ "[","SOP" });
	vttype.insert({ "]","SOP" });
	vttype.insert({ "{","SOP" });
	vttype.insert({ "}","SOP" });
	vttype.insert({ "int","REVERSED" });
	vttype.insert({ "if","REVERSED" });
	vttype.insert({ "else","REVERSED" });
	vttype.insert({ "return","REVERSED" });
	vttype.insert({ "void","REVERSED" });
	vttype.insert({ "while","REVERSED" });
	vttype.insert({ "NUM","NUM" });
	vttype.insert({ "ID","ID" });
	vttype.insert({ "IDF","IDF" });
	vttype.insert({ "ID1","ID1" });
	vttype.insert({ "empty","empty" });
	vttype.insert({ "#","#" });
}

//��ʼ��Ԥ�������mtable
void init_mtable(struct Mtable &mtable)
{
	int i, j, vnnum, vtnum;
	for (iter = vn.begin(), vnnum = 1;iter != vn.end();iter++, vnnum++)
		mtable.vnname.insert({ *iter,vnnum });
	//Ԥ��������е��ս��������Ҫȥ��empty �������ֵ���
	for (iter = vt.begin(), vtnum = 1;iter != vt.end();iter++, vtnum++)
		if (*iter != "empty")
			mtable.vtname.insert({ *iter,vtnum });
		else
			vtnum--;
	//��ʼ��ȫ��ֵΪ0 �Ƶ�ʽ�����1��ʼ
	for (i = 1;i < vnnum;i++)
		for (j = 1;j < vtnum;j++)
			mtable.mtable[i][j] = 0;
	mtable.vnnum = vnnum - 1;
	mtable.vtnum = vtnum - 1;
}

//�жϼ������Ƿ����strԪ��
int check_exist(vector<string> vec, string str)
{
	vector<string>::iterator iter;
	iter = find(vec.begin(), vec.end(), str);
	if (iter != vec.end())
		return 1;
	else
		return 0;
}

//�ж�vtmap���Ƿ��и�vt
int checkmap(map<string, string> mp, string str)
{
	iter_map = vttype.find(str);
	if (iter_map != vttype.end())
		return 1;
	else
		return 0;
}

//ɾ��vt����Ӧ��Ϊvn��Ԫ��
void del(vector<string> &vec, string str)
{
	iter = find(vec.begin(), vec.end(), str);
	vec.erase(iter);
}

//�����ս�������Լ����ս������
void cal_vn_vt(fstream &in,fstream &out)
{
	string temp,part1,part2,t;
	int split, start, index, num = 1;
	init_vttype(vttype);
	while (!in.eof())
	{
		getline(in, temp);
		if (in.fail())
			break;
		temp.replace(0, temp.find(' '), "");	//ȥ��ÿ�п�ͷ������
		split = temp.find("->");
		part1.assign(temp, 0, split);
		part2.assign(temp, split + 2, temp.length() - split - 1);
		fix(part1);
		fix(part2);			//��ÿһ���﷨��Ϊ��ͷǰ�����������
		start = 0;
		grammarmap.insert({ num,{part1} });
		//��ಿ�� ���vn�����л�û�г�������� ���vt�����г�����vt�����еĸ�Ԫ��ɾ��
		if (!check_exist(vn, part1))
			vn.push_back(part1);
		if (check_exist(vt, part1))
			del(vt, part1);
		while (1)		//����ͷ��Ĳ��ְ��տո�ֿ�
		{
			index = part2.find(' ',start);
			if (index == part2.npos)
				break;
			t.assign(part2, start, index - start);
			start = index + 1;
			//���Ҳಿ���� vn��û�г�����vt��û�г��� �򽫸�Ԫ�ؼ���vt������
			if (!check_exist(vn, t) && !check_exist(vt, t))
				vt.push_back(t);
			grammarmap[num].push_back(t);
		}
		t.assign(part2, start, part2.length() - start);
		if (!check_exist(vn, t) && !check_exist(vt, t))
			vt.push_back(t);
		//�������Ƶ����յķ��ս����
		grammarmap[num].push_back(t);
		if (grammarmap[num][1] == "empty")
			emptymake.insert({ grammarmap[num][0],num });
		num++;
	}
	vt.push_back("#");
	//���
	output_vec(out);
}

//���vector��������Ϣ ���vn,vt��Ϣ���ļ�
void output_vec(fstream &out)
{
	out << "���ս��vn:" << endl;
	for (iter = vn.begin();iter != vn.end();iter++)
	{
		out << *iter << endl;
		firstlist.insert({ *iter, {} });
		followlist.insert({ *iter,{} });
		if (*iter == firststr)
			followlist[firststr].push_back("#");
	}
	out << "�ս��vt:" << endl;
	for (iter = vt.begin();iter != vt.end();iter++)
	{
		if (!checkmap(vttype, *iter))
		{
			//cout << "Undefined vt[ " << *iter << " ]!" << endl;
			//exit(0);
			;
		}
		out << *iter << endl;
		firstlist.insert({ *iter, {*iter} });
	}
}

//��first��
void cal_first()
{
	int flag, outflag;
	unsigned int k;
	string vnname, tname, temp;
	while (1)
	{
		flag = 1;
		for (gmaper = grammarmap.begin();gmaper != grammarmap.end();gmaper++)
		{
			vnname = gmaper->second[0];
			if (check_exist(vt, gmaper->second[1]))
			{
				if (!check_exist(firstlist[vnname], gmaper->second[1]))
				{
					firstlist[vnname].push_back(gmaper->second[1]);
					flag = 0;
				}	//�жϸ��ս���Ƿ��Ѿ��ڸ��Ƶ���first����,û�������
			}		//����ʽ��һ��Ԫ��Ϊ�ս��
			else
			{
				k = 1;
				outflag = 1;
				while (1)
				{
					if (k >= gmaper->second.size())
						break;
					//cout << k << " " << gmaper->second.size() << endl;
					tname = gmaper->second[k];
					if (check_exist(vn, tname))
					{
						//��������Ƿ��Ѿ���vnname��first����,�����ھ����
						for (iter = firstlist[tname].begin();iter != firstlist[tname].end();iter++)
						{
							temp = *iter;
							if (!check_exist(firstlist[vnname], temp))
							{
								firstlist[vnname].push_back(temp);
								flag = 0;
							}
							if (*iter == "empty")	//����÷��ս����first����û��empty�������һ�Ƶ�ʽ
								outflag = 0;
						}
					}
					else
					{		//�Ƶ�ʽ�г����ս�� �ж��Ƿ���Ӻ� ������һ���Ƶ�ʽ
						outflag = 1;
						if (!check_exist(firstlist[vnname], tname))
						{
							firstlist[vnname].push_back(tname);
							flag = 0;
						}
					}
					if (outflag)
						break;
					k++;
				}
			}
		}
		//��flagΪ1ʱ��ʾ�Ѿ�û��first���������Ԫ�� fisrt����������
		if (flag)
			break;
	}
}

//��follow��
void cal_follow()
{
	int flag, outflag, exflag;
	string vnname, tname;
	vector<string>::iterator titer, temp;
	while (1)
	{
		flag = 1;
		for (gmaper = grammarmap.begin();gmaper != grammarmap.end();gmaper++)		//�����Ƶ�ʽ
		{
			for (iter = gmaper->second.begin() + 1;iter != gmaper->second.end();iter=titer)		//�����Ƶ�ʽ�и�����
			{
				titer = iter + 1;
				if (check_exist(vt, *iter))		//�����ս�� ����Ҫ����follow��
					continue;
				else
				{
					vnname = *iter;		//����һ�����ս����ʼ������ ֪�����������Ƶ��յķ��ս�������ս��
					while (1)
					{
						iter++;
						outflag = 1;		//��¼һ�����ս���ܲ����Ƶ�����
						if (iter == gmaper->second.end())		//���Ƶ�ʽ���� ��ǰ���ս��follow���������
							break;
						if (check_exist(vt, *iter))			
						{
							if (!check_exist(followlist[vnname], *iter) && *iter != "empty")
							{
								followlist[vnname].push_back(*iter);
								flag = 0;
							}
						}		//����һ���ս�� �ж��Ƿ���� ����������뵱ǰ���ս����follow����
						else
						{
							for (temp = firstlist[*iter].begin();temp != firstlist[*iter].end();temp++)
							{
								if (!check_exist(followlist[vnname], *temp) && *temp != "empty")
								{
									followlist[vnname].push_back(*temp);
									flag = 0;
								}
								if (*temp == "empty")
									outflag = 0;
							}		//����ǰ���ź����������е�first�����жϲ����뵽follow���� ֱ�����������Ƶ����յķ��ս��
						}
						if (outflag)
							break;
					}
				}
			}
			unsigned int i = 1;
			//�Ƶ�ʽ�������i�����ս�������Ƶ�Ϊ�� ���Ƶ�ʽ��ǰ�ķ��ս����follow��������Ԫ���жϲ���ӵ������еķ��ս����follow����
			while (1)
			{			
				if (check_exist(vn, *(gmaper->second.end() - i)))
				{
					string str;
					if (i >= gmaper->second.size())
						break;
					str = *(gmaper->second.end() - i);
					vnname = gmaper->second[0];
					for (temp = followlist[vnname].begin();temp != followlist[vnname].end();temp++)
					{
						if (!check_exist(followlist[str], *temp) && *temp != "empty")
						{
							followlist[str].push_back(*temp);
							flag = 0;
						}
					}
					exflag = 0;
					for (iter = firstlist[str].begin();iter != firstlist[str].end();iter++)
						if (*iter == "empty")
							exflag = 1;
					//exflag��¼�÷��ս����first�����Ƿ���empty
					if (!exflag)
						break;
					else
						i++;
				}
				else
					break;
			}
		}
		if (flag)
			break;
	}
}

//����first follow��
void cal_first_follow(fstream &in, fstream &out)
{
	cal_first();
	cal_follow();
	output_ff(out);
}

//���first,follow�����ļ�
void output_ff(fstream &out)
{
	//���first��
	out << "//first��" << endl;
	for (maper = firstlist.begin();maper != firstlist.end();maper++)
	{
		if (!check_exist(vt, maper->first))
		{
			out << maper->first << "��first��:";
			for (iter = maper->second.begin();iter != maper->second.end();iter++)
				out << *iter << " ";
			out << endl;
		}
	}
	//���follow��
	out << "//follow��" << endl;
	for (maper = followlist.begin();maper != followlist.end();maper++)
	{
		if (!check_exist(vt, maper->first))
		{
			out << maper->first << "��follow��:";
			for (iter = maper->second.begin();iter != maper->second.end();iter++)
				out << *iter << " ";
			out << endl;
		}
	}
}

//����Ԥ�������
void cal_mtable(fstream &in, fstream &out)
{
	string name, vnname;
	int emptyflag, i, j;
	init_mtable(mtable);
	//��������ս�����ս������
	out << "vnnum: " << mtable.vnnum << endl;
	out << "vtnum: " << mtable.vtnum << endl;
	for (gmaper = grammarmap.begin();gmaper != grammarmap.end();gmaper++)
	{
		name = gmaper->second[1];		//��ǰ�Ƶ�ʽ��ͷ�Ҳ��һ������
		vnname = gmaper->second[0];		//��ǰ�Ƶ�ʽ��ͷ���ķ��ս��
		emptyflag = 0;
		for (iter = firstlist[name].begin();iter != firstlist[name].end();iter++)
		{
			if (*iter == "empty")
			{
				emptyflag = 1;
				continue;
			}
			//��name������first������Ӧ��λ�����뵱ǰ�Ƶ�ʽ�ı��
			mtable.mtable[mtable.vnname[vnname]][mtable.vtname[*iter]] = gmaper->first;
		}
		if (name == "empty"&&gmaper->second.size() > 2)
		{
			name = gmaper->second[2];
			for (iter = firstlist[name].begin();iter != firstlist[name].end();iter++)
			{
				//��name������first������Ӧ��λ�����뵱ǰ�Ƶ�ʽ�ı��
				mtable.mtable[mtable.vnname[vnname]][mtable.vtname[*iter]] = gmaper->first;
			}
		}
		//���name���Բ������� ��vnname���е�follow������Ӧ��λ������ vnname�Ƶ����յı��
		if (emptyflag)
		{
			for (iter = followlist[vnname].begin();iter != followlist[vnname].end();iter++)
				mtable.mtable[mtable.vnname[vnname]][mtable.vtname[*iter]] = emptymake[vnname];
		}
	}
	//��ʽ�����Ԥ����������ļ�
	out << setw(20) << "";
	for (iter = vt.begin();iter != vt.end();iter++)
		if (*iter != "empty")
			out << setw(8) << left << *iter;
	out << endl;
	for (iter = vn.begin(), i = 1;iter != vn.end();iter++, i++)
	{
		out << setw(20) << left << *iter;
		for (j = 1;j <= mtable.vtnum;j++)
			out << setw(8) << left << mtable.mtable[i][j];
		out << endl;
	}
}

//��ʼ��ջ
void stack_init(struct Stack &stack)
{
	stack.st.push_back("#");
	stack.st.push_back(firststr);
	stack.current = stack.st.size();
}

//Ԫ����ջ
void stack_pushin(struct Stack &stack, string str)
{
	stack.st.push_back(str);
	stack.current++;
}

//��ȡջ��Ԫ��
string stack_gettop(struct Stack stack)
{
	return stack.st[stack.current - 1];
}

//ջ��Ԫ�س�ջ
void stack_popout(struct Stack &stack)
{
	string str;
	if (stack.st.size() == 0)
	{
		cout << "ERROR:ջ��Ϊ��,�޷���ջ!" << endl;
		exit(0);
	}
	else
	{
		stack.st.pop_back();
		stack.current--;
	}
}

//���ջ����Ϣ
void stack_show(struct Stack stack, fstream &out)
{
	int i;
	out << "��ǰջ��Ԫ�ظ���:" << stack.current;
	for (i = 0;i < stack.current;i++)
		out << " " << stack.st[i];
	out << endl;
}

//��ʼ���﷨��
void tree_init()
{
	treenode = (treebit *)malloc(sizeof(treebit));
	strcpy_s(treenode->re, "head");
	strcpy_s(treenode->type, "derivation");
	treenode->bitnum = 3;
	treenode->father = NULL;
	treenode->brother = NULL;		//��ʼ�����ڵ� 
	for (int i = 1;i <= treenode->bitnum;i++)
	{
		treenode->sons[i] = (struct Gtree *)malloc(sizeof(struct Gtree));
		treenode->sons[i]->father = (struct Gtree *)malloc(sizeof(struct Gtree));
		treenode->sons[i]->brother = (struct Gtree *)malloc(sizeof(struct Gtree));
	}
	strcpy_s(treenode->sons[1]->re, "#");
	strcpy_s(treenode->sons[1]->type, "#");
	treenode->sons[1]->brother = treenode->sons[2];
	treenode->sons[1]->father = treenode;
	nodenum++;
	treenode->sons[2]->nodenum = nodenum;
	placelistmap.insert({ nodenum,{} });
	strcpy_s(treenode->sons[2]->re, firststr.c_str());
	strcpy_s(treenode->sons[2]->type, "derivation");
	treenode->sons[2]->brother = treenode->sons[3];
	treenode->sons[2]->father = treenode;
	strcpy_s(treenode->sons[3]->re, "#");
	strcpy_s(treenode->sons[3]->type, "#");
	treenode->sons[3]->brother = NULL;
	treenode->sons[3]->father = treenode;		//��ʼ�����ڵ�ĺ��ӽڵ� ʹ���Ƶ�ʽ firststr-># firststr #
}

//�����﷨������
void make_tree(fstream &in, fstream &out)
{
	string str;
	int num, line;
	struct Gtree *cur;
	tree_init();
	stack_init(stack);				//��ʼ���﷨�������Լ��﷨ջ
	cur = treenode->sons[2];
	while (!in.eof())
	{
		if (!stack.current)
			break;
		in >> skipws;
		in >> token.re >> token.type >> token.line >> token.colume;
		if (in.fail())
			break;				//�Ӵʷ���������������ȡtoken
		stack_show(stack,out);
		str = stack_gettop(stack);
		while (mtable.vtname.find(str) == mtable.vtname.end())
		{	//ֱ��ջ��Ԫ��Ϊ�ս�� ����ѭ�� ׼���Ƚ�\��ջ����
			if (token.type != "NUM" && token.type != "ID" && token.type != "IDF" && token.type != "ID1")
				num = mtable.vtname[token.re];
			else
				num = mtable.vtname[token.type];		//���� ID����tokenͨ��ԭʼ�ַ����õ���� ����ͨ��type����
			if (mtable.mtable[mtable.vnname[str]][num] == 0)
			{
				out << "Grammar Error at Line " << token.line << " Colume " << token.colume << endl;
				cout << "Grammar Error at Line " << token.line << " Colume " << token.colume << endl;
				//cout << str << endl;
				//cout << token.re << endl;
				exit(0);
			}		//Ԥ���������������Ӧ���Ƶ�ʽ���Ϊ0 ��ʾ�����﷨����
			else
			{
				line = mtable.mtable[mtable.vnname[str]][num];
				stack_popout(stack);
				cur->bitnum = 0;
				cur->mtnum = line;
				for (iter = grammarmap[line].end() - 1;iter != grammarmap[line].begin();iter--)
				{
					if (*iter != "empty")
					{
						stack_pushin(stack, *iter);
					}
				}		//����Ϊempty�Ĳ���ʽ ѹ��ջ��

				for (iter = grammarmap[line].begin() + 1;iter != grammarmap[line].end();iter++)
				{
					if (cur->mtnum == 42)
					{
						if (*iter == "expression")
						{
							cur->bitnum++;
							cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
							cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
							cur->sons[cur->bitnum]->father = cur;
							strcpy_s(cur->sons[cur->bitnum]->re, "$42_1");
							strcpy_s(cur->sons[cur->bitnum]->type, "todo");
						}
						if (*iter == ")")
						{
							cur->bitnum++;
							cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
							cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
							cur->sons[cur->bitnum]->father = cur;
							strcpy_s(cur->sons[cur->bitnum]->re, "$42_2");
							strcpy_s(cur->sons[cur->bitnum]->type, "todo");
						}
						//42 iteration-stmt -> while ( $42_1 expression $42_2 ) statement $42_3
					}		//while����Ƶ�����Ҫ���������м����嶯��
					if (cur->mtnum == 39)
					{
						if (*iter == ")")
						{
							cur->bitnum++;
							cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
							cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
							cur->sons[cur->bitnum]->father = cur;
							strcpy_s(cur->sons[cur->bitnum]->re, "$39_1");
							strcpy_s(cur->sons[cur->bitnum]->type, "todo");
						}
						if (*iter == "selection-stmt'")
						{
							cur->bitnum++;
							cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
							cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
							cur->sons[cur->bitnum]->father = cur;
							strcpy_s(cur->sons[cur->bitnum]->re, "$39_2");
							strcpy_s(cur->sons[cur->bitnum]->type, "todo");
						}	//39 selection-stmt -> if ( expression $39_1 ) statement $39_2 selection-stmt' $39_3
					}		//if����Ƶ�����Ҫ���������м����嶯��
					if (cur->mtnum == 41)
					{
						if (*iter == "statement")
						{
							cur->bitnum++;
							cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
							cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
							cur->sons[cur->bitnum]->father = cur;
							strcpy_s(cur->sons[cur->bitnum]->re, "$41_1");
							strcpy_s(cur->sons[cur->bitnum]->type, "todo");
						}	//41 selection-stmt' -> else $41_1 statement $41_2
					}		//else����Ƶ�����Ҫ����һ���м����嶯��
					cur->bitnum++;
					cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
					if (cur->bitnum > 1)
						cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
					cur->sons[cur->bitnum]->father = cur;
					strcpy_s(cur->sons[cur->bitnum]->re, (*iter).c_str());
					if (mtable.vnname.find(*iter) != mtable.vnname.end())
						strcpy_s(cur->sons[cur->bitnum]->type, "derivation");
					else
						strcpy_s(cur->sons[cur->bitnum]->type, vttype.find(*iter)->second.c_str());
					cur->sons[cur->bitnum]->info = (struct info *)malloc(sizeof(struct info));
					nodenum++;
					cur->sons[cur->bitnum]->nodenum = nodenum;
					placelistmap.insert({ nodenum,{} });
				}		//����������ʽ������Ԫ�� ���뵽��ǰ�ڵ�ĺ��ӽڵ� ����������Ϣ�Լ��ֵܸ��׽ڵ�
				cur->bitnum++;
				stringstream ss;
				ss << cur->mtnum;
				cur->sons[cur->bitnum] = (treebit *)malloc(sizeof(treebit));
				strcpy_s(cur->sons[cur->bitnum]->re, ("$" + ss.str()).c_str());
				if (ss.str() == "42")
					strcpy_s(cur->sons[cur->bitnum]->re, "$42_3");
				if (ss.str() == "39")
					strcpy_s(cur->sons[cur->bitnum]->re, "$39_3");
				if (ss.str() == "41")
					strcpy_s(cur->sons[cur->bitnum]->re, "$41_2");
				//��������ڵ������� $ ���� ���� ����ʽ������
				strcpy_s(cur->sons[cur->bitnum]->type, "todo");
				cur->sons[cur->bitnum]->father = cur;
				cur->sons[cur->bitnum - 1]->brother = cur->sons[cur->bitnum];
				cur->sons[cur->bitnum]->brother = NULL;			//������$�ڵ� �����õڼ�������ʽ���� ���ں����������
				cur = cur->sons[1];
				if (strcmp(cur->re, "empty") == 0)
				{
					while (1)
					{
						while (!cur->brother)
						{
							cur = cur->father;
							if (strcmp(cur->re, "head") == 0)
								break;
						}
						cur = cur->brother;
						if (cur->re[0] != '$')
							break;
					}
				}		//Ӧ���굱ǰ����ʽ �����ֵܽڵ�����Ƶ� û���ֵܽڵ� ���ظ��׽ڵ� ���������ֵܽڵ� ֪���ֵܽڵ����
				str = stack_gettop(stack);
				stack_show(stack, out);
			 }
		}
		if (token.re != stack_gettop(stack) && token.type != stack_gettop(stack))
		{
			out << "Grammar Error at Line " << token.line << " Colume " << token.colume << endl;
			cout << "Grammar Error at Line " << token.line << " Colume " << token.colume << endl;
			exit(0);
		}				//ջ���ս�������뵱ǰtoken���Ͳ����� ����
		if (strstr(cur->type, "ID"))
			strcpy_s(cur->re, token.re.c_str());		//����id����ֱ�Ӹı䵱ǰ�ڵ�ԭʼ�ַ��� ���ٲ����µĽڵ�
		if (strstr(cur->type, "NUM"))
			strcpy_s(cur->re, token.re.c_str());
		while (1)
		{
			while (!cur->brother)
			{
				cur = cur->father;
				if (strcmp(cur->re, "head") == 0)
					break;
			}
			if (strcmp(cur->re, "head") == 0)
				break;
			cur = cur->brother;	
			if (cur->re[0] != '$')
				break;
		}		//Ѱ�������������ֵܽڵ�
		//ջ��Ԫ�س�ջ
		stack_popout(stack);
	}
}

//����һ���µ���ʱ����
string newtemp()
{
	string T = "T";
	stringstream ss;
	ss << curtemp;
	curtemp++;		//��ʱ����������һ
	T.append(ss.str());	//��ʱ������������ T+��ǰ����
	return T;
}

//����м����
void printmidcode(fstream &out)
{
	map<int, string>::iterator iter_i_s;
	for (iter_i_s = midcodemap.begin();iter_i_s != midcodemap.end();iter_i_s++)
		out << "(" << iter_i_s->first << ") " << iter_i_s->second << endl;
}

bool fomula_1(treebit *temp)
{
	//1 S -> program
	return true;
}
bool fomula_2(treebit *temp)
{
	return true;
}
bool fomula_3(treebit *temp)
{
	return true;
}
bool fomula_4(treebit *temp)
{
	return true;
}
bool fomula_5(treebit *temp)
{
	return true;
}
bool fomula_6(treebit *temp)
{
	return true;
}
bool fomula_7(treebit *temp)
{
	return true;
}
bool fomula_8(treebit *temp)
{
	return true;
}
bool fomula_9(treebit *temp)
{
	return true;
}
bool fomula_10(treebit *temp)
{
	return true;
}
bool fomula_11(treebit *temp)
{
	return true;
}
bool fomula_12(treebit *temp)
{
	return true;
}
bool fomula_13(treebit *temp)
{
	//13 fun-declaration -> type-specifier IDF ( params ) 
	temp = temp->father;
	curnum++;
	midcodemap.insert({ curnum,proc + temp->sons[2]->re });		//�����м���� ���庯��
	return true;
}
bool fomula_14(treebit *temp)
{
	return true;
}
bool fomula_15(treebit *temp)
{
	return true;
}
bool fomula_16(treebit *temp)
{
	return true;
}
bool fomula_17(treebit *temp)
{
	return true;
}
bool fomula_18(treebit *temp)
{
	return true;
}
bool fomula_19(treebit *temp)
{
	return true;
}
bool fomula_20(treebit *temp)
{
	return true;
}
bool fomula_21(treebit *temp)
{
	return true;
}
bool fomula_22(treebit *temp)
{
	return true;
}
bool fomula_23(treebit *temp)
{
	return true;
}
bool fomula_24(treebit *temp)
{
	return true;
}
bool fomula_25(treebit *temp)
{
	return true;
}
bool fomula_26(treebit *temp)
{
	return true;
}
bool fomula_27(treebit *temp)
{
	return true;
}
bool fomula_28(treebit *temp)
{
	return true;
}
bool fomula_29(treebit *temp)
{
	return true;
}
bool fomula_30(treebit *temp)
{
	return true;
}
bool fomula_31(treebit *temp)
{
	return true;
}
bool fomula_32(treebit *temp)
{
	return true;
}
bool fomula_33(treebit *temp)
{
	return true;
}
bool fomula_34(treebit *temp)
{
	return true;
}
bool fomula_35(treebit *temp)
{
	return true;
}
bool fomula_36(treebit *temp)
{
	return true;
}
bool fomula_37(treebit *temp)
{
	return true;
}
bool fomula_38(treebit *temp)
{
	return true;
}
bool fomula_39_1(treebit *temp)
{
	//39 selection-stmt -> if ( expression $39_1 ) statement $39_2 selection-stmt' $39_3
	temp = temp->father;
	temp->info->ifnum1 = curnum + 2;	//if��������Ϊ��ǰcurnum+2��λ��
	curnum++;
	stringstream ss;
	ss << curnum + 2;
	midcodemap.insert({ curnum,"if " + placelistmap[temp->sons[3]->nodenum][0] + "==1 Goto " + ss.str() });
	//Ϊ����ǿ����ת��ǰλ��+2��λ��
	curnum++;
	midcodemap.insert({ curnum,"Goto " });
	//Ϊ��ǿ����ת���ٳ��� ��ʱû����� 
	return true;
}
bool fomula_39_2(treebit *temp)
{
	//39 selection-stmt -> if ( expression $39_1 ) statement $39_2 selection-stmt' $39_3
	temp = temp->father;
	curnum++;
	midcodemap.insert({ curnum,"Goto " });		//if���Ϊ��ִ�������� ǿ����ת��if�����һ�� ��ʱδ���
	temp->info->ifnum2 = curnum;		//�õ�if�ٳ���λ��ǰһ��
	return true;
}
bool fomula_39_3(treebit *temp)
{
	//39 selection-stmt -> if ( expression $39_1 ) statement $39_2 selection-stmt' $39_3
	temp = temp->father;
	if (temp->sons[8]->info->emptyflag)
	{
		stringstream ss;
		ss << temp->info->ifnum2;
		midcodemap[temp->info->ifnum1].append(ss.str());
		midcodemap.erase(curnum);
		curnum--;
	}		//emptyflagΪ1��������else��� ɾ�����һ��ǿ����ת��� ���ҽ�֮ǰ���ŵ��ж�����Ϊ��ʱ����תλ������
	else
	{
		stringstream ss;
		ss << temp->sons[8]->info->elsenum1 + 1;
		midcodemap[temp->info->ifnum1].append(ss.str());
		stringstream s;
		s << temp->sons[8]->info->elsenum2 + 1;
		midcodemap[temp->info->ifnum2].append(s.str());
	}		//emptyflagΪ0����else��� ����ж�����Ϊ�ٵ���ת��ַ�Լ�Ϊ�����������ǿ����ת��ַ
	return true;
}
bool fomula_40(treebit *temp)
{
	//40 selection-stmt' -> empty 
	temp = temp->father;
	temp->info->emptyflag = 1;		//��else��佫emptyflag��Ϊ0
	return true;
}
bool fomula_41_1(treebit *temp)
{
	//41 selection-stmt' -> else $41_1 statement $41_2
	temp = temp->father;
	temp->info->emptyflag = 0;
	temp->info->elsenum1 = curnum;		//��¼else����ڵ�ַ ��43_3ʱ����
	return true;
}
bool fomula_41_2(treebit *temp)
{
	//41 selection-stmt' -> else $41_1 statement $41_2
	temp = temp->father;
	temp->info->emptyflag = 0;
	temp->info->elsenum2 = curnum;		//��¼else�ĳ��ڵ�ַ ��43_3ʱ����
	return true;
}
bool fomula_42_1(treebit *temp)
{
	//42 iteration-stmt -> while ( $42_1 expression $42_2 ) statement $42_3
	temp = temp->father;
	temp->info->whilenum1 = curnum + 1;		//��¼while��ڵ�ַ 42_3ʱ��ת�ش˵�ַ
	return true;
}
bool fomula_42_2(treebit *temp)
{
	//42 iteration-stmt -> while ( $42_1 expression $42_2 ) statement $42_3
	temp = temp->father;
	temp->info->whilenum2 = curnum + 1;
	curnum++;
	stringstream ss;
	ss << curnum + 2;
	midcodemap.insert({ curnum,"if " + placelistmap[temp->sons[4]->nodenum][0] + "==1 Goto " + ss.str() });
	//�����м�����ж�expression�Ƿ�Ϊ�� Ϊ��ǿ����ת����ǰ��ַ+2 
	curnum++;
	midcodemap.insert({ curnum,"Goto " });
	//Ϊ����ת���ٳ��� ��ʱΪ���
	return true;
}
bool fomula_42_3(treebit *temp)
{
	//42 iteration-stmt -> while ( $42_1 expression $42_2 ) statement $42_3
	temp = temp->father;
	stringstream ss;
	ss << curnum + 2;
	midcodemap[temp->info->whilenum2 + 1].append(ss.str());
	//����ٳ���ֵ
	curnum++;
	stringstream s;
	s << temp->info->whilenum1;
	midcodemap.insert({ curnum,"Goto " + s.str() });
	//�����м���� ǿ����ת��42_1λ��
	return true;
}
bool fomula_43(treebit *temp)
{
	//43 return-stmt -> return return-stmt'
	temp = temp->father;
	temp->info->returnflag = 1;
	if (temp->sons[2]->info->emptyflag)
	{
		curnum++;
		midcodemap.insert({ curnum,"return" });
	}	//�޷���ֵ
	else
	{
		curnum++;
		midcodemap.insert({ curnum,"return " + placelistmap[temp->sons[2]->nodenum][0] });
	}	//�з���ֵ
	return true;
}
bool fomula_44(treebit *temp)
{
	//44 return-stmt' -> ; 
	temp = temp->father;
	temp->info->emptyflag = 1;
	return true;
}
bool fomula_45(treebit *temp)
{
	//45 return-stmt' -> expression ; 
	temp = temp->father;
	temp->info->emptyflag = 0;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	return true;
}
bool fomula_46(treebit *temp)
{
	//46 expression -> var1 = expression 
	temp = temp->father;
	curnum++;
	midcodemap.insert({ curnum,placelistmap[temp->sons[1]->nodenum][0] + " := " + placelistmap[temp->sons[3]->nodenum][0] });
	return true;
}
bool fomula_47(treebit *temp)
{
	//47 expression -> simple-expression
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	return true;
}
bool fomula_48(treebit *temp)
{
	//48 var -> ID var'
	temp = temp->father;
	char ts[255];
	if (temp->sons[2]->info->arrayflag)
	{
		strcat_s(ts, temp->sons[1]->re);
		strcat_s(ts, placelistmap[temp->sons[2]->nodenum][0].c_str());
		placelistmap[temp->nodenum].push_back(ts);
	}
	else
		placelistmap[temp->nodenum].push_back(temp->sons[1]->re);
	return true;
}
bool fomula_49(treebit *temp)
{
	//49 var' -> empty 
	temp = temp->father;
	temp->info->arrayflag = 0;
	return true;
}
bool fomula_50(treebit *temp)
{
	//50 var' -> [ expression ]
	temp = temp->father;
	temp->info->arrayflag = 1;
	placelistmap[temp->nodenum].push_back("[" + placelistmap[temp->sons[2]->nodenum][0] + "]");
	//temp->info.placelist.push_back("[" + temp->sons[2]->info.placelist[0] + "]");
	return true;
}
bool fomula_51(treebit *temp)
{
	//51 var1 -> ID1 var1'
	temp = temp->father;
	char ts[255];
	if (temp->sons[2]->info->arrayflag)
	{
		strcat_s(ts, temp->sons[1]->re);
		strcat_s(ts, placelistmap[temp->sons[2]->nodenum][0].c_str());
		placelistmap[temp->nodenum].push_back(ts);
	}
	else
		placelistmap[temp->nodenum].push_back(temp->sons[1]->re);
	return true;
}
bool fomula_52(treebit *temp)
{
	//52 var1' -> empty 
	temp = temp->father;
	temp->info->arrayflag = 0;
	return true;
}
bool fomula_53(treebit *temp)
{
	//53 var1' -> [ expression ] 
	temp = temp->father;
	temp->info->arrayflag = 1;
	placelistmap[temp->nodenum].push_back("[" + placelistmap[temp->sons[2]->nodenum][0] + "]");
	return true;
}
bool fomula_54(treebit *temp)
{
	//54 simple-expression -> additive-expression simple-expression'
	temp = temp->father;
	if (temp->sons[2]->info->emptyflag)
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	else
	{
		string t = newtemp();
		placelistmap[temp->nodenum].push_back(t);
		curnum++;
		midcodemap.insert({ curnum,t + " := " + placelistmap[temp->sons[1]->nodenum][0] + " " + placelistmap[temp->sons[2]->nodenum][0] });
	}
	return true;
}
bool fomula_55(treebit *temp)
{
	//55 simple-expression' -> relop additive-expression 
	temp = temp->father;
	temp->info->emptyflag = 0;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0] + " " + placelistmap[temp->sons[2]->nodenum][0]);
	return true;
}
bool fomula_56(treebit *temp)
{
	//56 simple-expression' -> empty 
	temp = temp->father;
	temp->info->emptyflag = 1;
	return true;
}
bool fomula_57(treebit *temp)
{
	//relop <=
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("<=");
	return true;
}
bool fomula_58(treebit *temp)
{
	//relop <
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("<");
	return true;
}
bool fomula_59(treebit *temp)
{
	//relop >
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(">");
	return true;
}
bool fomula_60(treebit *temp)
{
	//relop >=
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(">=");
	return true;
}
bool fomula_61(treebit *temp)
{
	//rolop ==
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("==");
	return true;
}
bool fomula_62(treebit *temp)
{
	//relop !=
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("!=");
	return true;
}
bool fomula_63(treebit *temp)
{
	//63 additive-expression -> term additive-expression'
	temp = temp->father;
	if (temp->sons[2]->info->emptyflag)
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	else
	{
		string t = newtemp();
		placelistmap[temp->nodenum].push_back(t);
		curnum++;
		midcodemap.insert({ curnum,t + " := " + placelistmap[temp->sons[1]->nodenum][0] + " " + placelistmap[temp->sons[2]->nodenum][0] });
	}
	return true;
}
bool fomula_64(treebit *temp)
{
	//64 additive-expression' -> addop term additive-expression' 
	temp = temp->father;
	temp->info->emptyflag = 0;
	if (temp->sons[3]->info->emptyflag)
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0] + " " + placelistmap[temp->sons[2]->nodenum][0]);
	else
	{
		string t = newtemp();
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0] + " " + t);
		curnum++;
		midcodemap.insert({ curnum,t + " := " + placelistmap[temp->sons[2]->nodenum][0] + " " + placelistmap[temp->sons[3]->nodenum][0] });
	}
	return true;
}
bool fomula_65(treebit *temp)
{
	//65 additive-expression' -> empty
	temp = temp->father;
	temp->info->emptyflag = 1;
	return true;
}
bool fomula_66(treebit *temp)
{
	//addop +
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("+");
	return true;
}
bool fomula_67(treebit *temp)
{
	//addop -
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("-");
	return true;
}
bool fomula_68(treebit *temp)
{
	//68 term -> factor term'
	temp = temp->father;
	if (temp->sons[2]->info->emptyflag)
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	else
	{
		string t = newtemp();
		placelistmap[temp->nodenum].push_back(t);
		curnum++;
		midcodemap.insert({ curnum,t + " := " + placelistmap[temp->sons[1]->nodenum][0] + " " + placelistmap[temp->sons[2]->nodenum][0] });
	}
	return true;
}
bool fomula_69(treebit *temp)
{
	//69 term' -> mulop factor term' 
	temp = temp->father;
	temp->info->emptyflag = 0;
	if (temp->sons[3]->info->emptyflag)
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0] + " " + placelistmap[temp->sons[2]->nodenum][0]);
	else
	{
		string t=newtemp();
		placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0] + " " + t);
		curnum++;
		midcodemap.insert({ curnum,t + " := " + placelistmap[temp->sons[2]->nodenum][0] + " " + placelistmap[temp->sons[3]->nodenum][0] });
	}
	return true;
}
bool fomula_70(treebit *temp)
{
	//70 term' -> empty
	temp = temp->father;
	temp->info->emptyflag = 1;
	return true;
}
bool fomula_71(treebit *temp)
{
	//mulop *
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("*");
	return true;
}
bool fomula_72(treebit *temp)
{
	//mulop /
	temp = temp->father;
	placelistmap[temp->nodenum].push_back("/");
	return true;
}
bool fomula_73(treebit *temp)
{
	//73 factor -> ( expression )
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[2]->nodenum][0]);
	return true;
}
bool fomula_74(treebit *temp)
{
	//74 factor -> var 
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	return true;
}
bool fomula_75(treebit *temp)
{
	//75 factor -> call
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	return true;
}
bool fomula_76(treebit *temp)
{
	//76 factor -> NUM
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(temp->sons[1]->re);
	return true;
}
bool fomula_77(treebit *temp)
{
	//77 call -> IDF ( args )
	temp = temp->father;
	string t;
	t = newtemp();
	placelistmap[temp->nodenum].push_back(t);
	if (!temp->sons[3]->info->emptyflag)
		for (iter = placelistmap[temp->sons[3]->nodenum].begin();iter != placelistmap[temp->sons[3]->nodenum].end();iter++)
		{
			curnum++;
			midcodemap.insert({ curnum,param + *iter });
		}
	curnum++;
	midcodemap.insert({ curnum ,t + " := " + call + temp->sons[1]->re });
	return true;
}
bool fomula_78(treebit *temp)
{
	//78 args -> arg-list 
	temp = temp->father;
	temp->info->emptyflag = 0;
	for (iter = placelistmap[temp->sons[1]->nodenum].begin();iter != placelistmap[temp->sons[1]->nodenum].end();iter++)
		placelistmap[temp->nodenum].push_back(*iter);
	return true;
}
bool fomula_79(treebit *temp)
{
	//79 args -> empty
	temp = temp->father;
	temp->info->emptyflag = 1;
	return true;
}
bool fomula_80(treebit *temp)
{
	//80 arg-list -> expression arg-list'
	temp = temp->father;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[1]->nodenum][0]);
	if (!temp->sons[2]->info->emptyflag)
		for (iter = placelistmap[temp->sons[2]->nodenum].begin();iter != placelistmap[temp->sons[2]->nodenum].end();iter++)
			placelistmap[temp->nodenum].push_back(*iter);
	return true;
}
bool fomula_81(treebit *temp)
{
	//81 arg-list' -> , expression arg-list' 
	temp = temp->father;
	temp->info->emptyflag = 0;
	placelistmap[temp->nodenum].push_back(placelistmap[temp->sons[2]->nodenum][0]);
	if (!temp->sons[3]->info->emptyflag)
		for (iter = placelistmap[temp->sons[3]->nodenum].begin();iter != placelistmap[temp->sons[3]->nodenum].end();iter++)
			placelistmap[temp->nodenum].push_back(*iter);
	return true;
}
bool fomula_82(treebit *temp)
{
	//82 arg-list' -> empty
	temp = temp->father;
	temp->info->emptyflag = 1;
	return true;
}

//��ʼ�����嶯����
void init_funcmap()
{
	funcmap.insert({"$1",fomula_1});
	funcmap.insert({ "$2",fomula_2 });
	funcmap.insert({ "$3",fomula_3 });
	funcmap.insert({ "$4",fomula_4 });
	funcmap.insert({ "$5",fomula_5 });
	funcmap.insert({ "$6",fomula_6 });
	funcmap.insert({ "$7",fomula_7 });
	funcmap.insert({ "$8",fomula_8 });
	funcmap.insert({ "$9",fomula_9 });
	funcmap.insert({ "$10",fomula_10 });
	funcmap.insert({ "$11",fomula_11 });
	funcmap.insert({ "$12",fomula_12 });
	funcmap.insert({ "$13",fomula_13 });
	funcmap.insert({ "$14",fomula_14 });
	funcmap.insert({ "$15",fomula_15 });
	funcmap.insert({ "$16",fomula_16 });
	funcmap.insert({ "$17",fomula_17 });
	funcmap.insert({ "$18",fomula_18 });
	funcmap.insert({ "$19",fomula_19 });
	funcmap.insert({ "$20",fomula_20 });
	funcmap.insert({ "$21",fomula_21 });
	funcmap.insert({ "$22",fomula_22 });
	funcmap.insert({ "$23",fomula_23 });
	funcmap.insert({ "$24",fomula_24 });
	funcmap.insert({ "$25",fomula_25 });
	funcmap.insert({ "$26",fomula_26 });
	funcmap.insert({ "$27",fomula_27 });
	funcmap.insert({ "$28",fomula_28 });
	funcmap.insert({ "$29",fomula_29 });
	funcmap.insert({ "$30",fomula_30 });
	funcmap.insert({ "$31",fomula_31 });
	funcmap.insert({ "$32",fomula_32 });
	funcmap.insert({ "$33",fomula_33 });
	funcmap.insert({ "$34",fomula_34 });
	funcmap.insert({ "$35",fomula_35 });
	funcmap.insert({ "$36",fomula_36 });
	funcmap.insert({ "$37",fomula_37 });
	funcmap.insert({ "$38",fomula_38 });
	funcmap.insert({ "$39_1",fomula_39_1 });
	funcmap.insert({ "$39_2",fomula_39_2 });
	funcmap.insert({ "$39_3",fomula_39_3 });
	funcmap.insert({ "$40",fomula_40 });
	funcmap.insert({ "$41_1",fomula_41_1 });
	funcmap.insert({ "$41_2",fomula_41_2 });
	funcmap.insert({ "$42_1",fomula_42_1 });
	funcmap.insert({ "$42_2",fomula_42_2 });
	funcmap.insert({ "$42_3",fomula_42_3 });
	funcmap.insert({ "$43",fomula_43 });
	funcmap.insert({ "$44",fomula_44 });
	funcmap.insert({ "$45",fomula_45 });
	funcmap.insert({ "$46",fomula_46 });
	funcmap.insert({ "$47",fomula_47 });
	funcmap.insert({ "$48",fomula_48 });
	funcmap.insert({ "$49",fomula_49 });
	funcmap.insert({ "$50",fomula_50 });
	funcmap.insert({ "$51",fomula_51 });
	funcmap.insert({ "$52",fomula_52 });
	funcmap.insert({ "$53",fomula_53 });
	funcmap.insert({ "$54",fomula_54 });
	funcmap.insert({ "$55",fomula_55 });
	funcmap.insert({ "$56",fomula_56 });
	funcmap.insert({ "$57",fomula_57 });
	funcmap.insert({ "$58",fomula_58 });
	funcmap.insert({ "$59",fomula_59 });
	funcmap.insert({ "$60",fomula_60 });
	funcmap.insert({ "$61",fomula_61 });
	funcmap.insert({ "$62",fomula_62 });
	funcmap.insert({ "$63",fomula_63 });
	funcmap.insert({ "$64",fomula_64 });
	funcmap.insert({ "$65",fomula_65 });
	funcmap.insert({ "$66",fomula_66 });
	funcmap.insert({ "$67",fomula_67 });
	funcmap.insert({ "$68",fomula_68 });
	funcmap.insert({ "$69",fomula_69 });
	funcmap.insert({ "$70",fomula_70 });
	funcmap.insert({ "$71",fomula_71 });
	funcmap.insert({ "$72",fomula_72 });
	funcmap.insert({ "$73",fomula_73 });
	funcmap.insert({ "$74",fomula_74 });
	funcmap.insert({ "$75",fomula_75 });
	funcmap.insert({ "$76",fomula_76 });
	funcmap.insert({ "$77",fomula_77 });
	funcmap.insert({ "$78",fomula_78 });
	funcmap.insert({ "$79",fomula_79 });
	funcmap.insert({ "$80",fomula_80 });
	funcmap.insert({ "$81",fomula_81 });
	funcmap.insert({ "$82",fomula_82 });
}

//ʹ��������ȱ��������﷨������
void deepsearch(treebit *temp,fstream &out)
{
	if (strstr(temp->type, "derivation") != 0)
	{
		//out << temp->re << " " << temp->nodenum << endl;
		for (int i = 1;i <= temp->bitnum;i++)
			deepsearch(temp->sons[i],out);
	}
	else
	{
		//out << temp->re << endl;
		stringstream ss;
		ss << temp->re;
		if (ss.str().find("$") != ss.str().npos)
			funcmap[ss.str()](temp);
	}
}

//�����м����
void midcode(fstream &in, fstream &out)
{
	treebit *temp;
	init_funcmap();
	temp = treenode;
	deepsearch(temp->sons[2],out);
	printmidcode(out);
}
