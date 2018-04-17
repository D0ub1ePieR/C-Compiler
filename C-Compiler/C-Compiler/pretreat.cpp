#include "pretreat.h"

//in >> noskipws ��ȡ�ļ��еĿհ��ַ�

//״̬ת�����pretreat.h
void delzs(fstream &in, fstream &out)
{
	char temp ,pre;
	string temp_s="";		//�洢����Ϊע�͵��ַ���
	int status = 0;
	in >> noskipws;
	while (!in.eof())
	{
		if (!status) status = 1;
		in >> temp;
		if (in.fail())
			break;
		switch (status)
		{
			case 1:
				if (temp != '/')
				{
					out << temp;	//cout << temp;
					status = 1;
				}
				else
					status = 2;
				pre = temp;
				break;
			case 2:
				if (temp == '/')
				{
					out << temp;	//cout << temp;
					status = 2;
				}
				else
					if (temp != '*')
					{
						out << '/' << temp;		//cout << '/' << temp;
						status = 1;
					}
					else
						status = 3;
				pre = temp;
				break;
			case 3:
				if (temp == '*')
					status = 4;
				else
					status = 3;
				temp_s.push_back(temp);
				if (pre == '/' && temp == '*')
				{
					cout << "����ע��Ƕ�ף�" << endl;
					out << "[Error Occured!]";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					exit(0);
				}
				pre = temp;
				break;
			case 4:
				if (temp == '/')
					status = 5;
				else
					if (temp == '*')
						status = 4;
					else
						status = 3;
				temp_s.push_back(temp);
				pre = temp;
				break;
			case 5:
				temp_s = "";		//һ��ע�ͽ��� ����ݴ�Ŀ���Ϊ��ע����Ϣ
				if (temp != '/')
				{
					out << temp;	//cout << temp;
					status = 1;
				}
				else
					status = 2;
				pre = temp;
				break;
			default:
				cout << "An error occured!" << endl;
				break;					//����default��֧ ˵�������Ƚ�����Ĵ���
		};
	}
	//������ע�͸�ʽ�Ҳ�û�бպ� ����Ҫ�� ��Ϊ��� �����б�Ĳ������
	if (status == 2)
	{
		out << "/";		//cout << "/";
	}
	if (status == 3 || status == 4)
	{
		out << "/*" << temp_s;	//cout << "/*" << temp_s;
	}
}

//״̬ת�����pretreat.h
void delblank(fstream &in, fstream &out)
{
	char temp, pre;
	int status=0;
	in >> noskipws;
	in >> temp;
	while (temp == ' ' ||  temp == '\t' || temp == '\n')
		in >> temp;
	out << temp;	//cout << temp;
	pre = temp;				//ȥ������ǰ���еĿհ��ַ���ֱ����һ���ǻ��з��ո���Ʊ��
	while (!in.eof())
	{
		in >> temp;
		if (in.fail())
			break;
		if (!status) status = 1;
		switch (status)
		{
			case 1:
				if (temp == ' ' || temp == '\n' || temp == '\t')
					status = 2;
				else
				{
					out << temp;	//cout << temp;
				}
				pre = temp;
				break;
			case 2:
				if (temp != ' ' && temp != '\n' && temp != '\t')
				{
					status = 1;
					out << pre << temp;		//cout << pre << temp;			//2״̬���յ�һ���ǿհ��ַ� ����������һ���հ��ַ�
				}
				if (pre!='\n')
					pre = temp;				//�绻�����Ʊ����ո�ͬʱ���� �������� ��֤��������ʽ�����ı�
				break;
			default:
				cout << "An error occured!" << endl;
				break;						//����default��֧ ˵�������Ƚ�����Ĵ���
		};
	}
	return;
}

//���ַ������տո�ָ�,���������ִ���������
int splitblank(string str, string strsplit[])
{
	int index, start, k = 0;
	string t;
	start = 0;
	while (1)		//����ͷ��Ĳ��ְ��տո�ֿ�
	{
		index = str.find(' ', start);
		if (index == str.npos)
			break;
		strsplit[k++].assign(str, start, index - start);
		start = index + 1;
	}
	strsplit[k++].assign(str, start, str.length() - start);
	return k;		//����һ����Ϊ�˼�����
}

//������ݹ��Լ�����������
void delleftrecursion(fstream &in, fstream &out)
{
	string temp, part1, part2, p2_1, p2_2;
	string str1[10], str2[10];
	int k1, k2, flag;
	int split, index;
	while (!in.eof())
	{
		getline(in, temp);
		if (in.fail())
			break;
		index = temp.find('|');
		if (index != part2.npos)
		{
			split = temp.find("->");
			part1.assign(temp, 0, split);
			part2.assign(temp, split + 2, temp.length() - split - 1);
			fix(part1);
			fix(part2);			//��ÿһ���﷨��Ϊ��ͷǰ�����������
			index = part2.find('|');
			p2_1.assign(part2, 0, index);
			p2_2.assign(part2, index + 1, part2.length() - index);
			fix(p2_1);
			fix(p2_2);			//��part2���ְ���|ǰ���Ϊ������
			if (p2_1.find(part1) == 0 && p2_1[part1.length()] == ' ')
			{
				p2_1.assign(p2_1, part1.length() + 1, p2_1.length() - part1.length());
				out << part1 << " -> ";
				if (p2_2 != "empty")
					out << p2_2 << " ";
				else
					out << p2_2 << " | ";
				out << part1 << "'" << endl;
				out << part1 << "' -> " << p2_1 << " " << part1 << "' | empty" << endl;
			}			//p2_1����part1˵����������ݹ�
			else
			{
				k1 = splitblank(p2_1, str1);
				k2 = splitblank(p2_2, str2);
				
				flag = 0;
				while (str1[flag] == str2[flag])
					flag++;		//�ж�������м�����������
				if (!flag)		//flag��ʾ���������Ӹ���,Ϊ0ʱ�޹���������
					out << temp << endl;
				else
				{
					out << part1 << " -> ";
					for (int i = 0;i < flag;i++)
						out << str1[i] << " ";
					out << part1 << "'" << endl;	//����滻���ĵ�һ��
					out << part1 << "' -> ";
					for (int i = flag;i < k1;i++)
						out << str1[i] << " ";
					if (flag == k1)		//һ������ʽ��Ϊ������������������ʱ����empty
						out << "empty ";
					out << "| ";
					for (int i = flag;i < k2;i++)
						out << str2[i] << " ";
					if (flag == k2)
						out << "empty ";
					out << endl;
				}
			}
		}
		else
			out << temp << endl;
	}
}

//���﷨����|���
void splitgrammar(fstream &in, fstream &out)
{
	string temp, output = "", t;
	int start, index, count=1;
	while (!in.eof())
	{
		getline(in, temp);
		if (in.fail())
			break;
		start = temp.find("->") + 2;
		output.assign(temp, 0, start);
		index = temp.find('|', start);
		while (index != temp.npos)
		{
			t.assign(temp, start, index - start);
			out << count++ << " " << output << t << endl;
			start = index + 1;
			index = temp.find('|', start);
		}
		t.assign(temp, start, temp.length() - start + 1);
		out << count++ << " " << output << t;
		out << endl;
	}
	maxline = count - 1;
}