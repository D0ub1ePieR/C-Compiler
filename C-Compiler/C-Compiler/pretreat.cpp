#include "pretreat.h"

//in >> noskipws 读取文件中的空白字符

//状态转换表见pretreat.h
void delzs(fstream &in, fstream &out)
{
	char temp ,pre;
	string temp_s="";		//存储可能为注释的字符串
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
					cout << "出现注释嵌套！" << endl;
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
				temp_s = "";		//一段注释结束 清空暂存的可能为的注释信息
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
				break;					//进入default分支 说明发生比较神奇的错误
		};
	}
	//如果最后注释格式右侧没有闭合 则需要将 尚为输出 正在判别的部分输出
	if (status == 2)
	{
		out << "/";		//cout << "/";
	}
	if (status == 3 || status == 4)
	{
		out << "/*" << temp_s;	//cout << "/*" << temp_s;
	}
}

//状态转换表见pretreat.h
void delblank(fstream &in, fstream &out)
{
	char temp, pre;
	int status=0;
	in >> noskipws;
	in >> temp;
	while (temp == ' ' ||  temp == '\t' || temp == '\n')
		in >> temp;
	out << temp;	//cout << temp;
	pre = temp;				//去除程序前所有的空白字符，直到第一个非换行符空格或制表符
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
					out << pre << temp;		//cout << pre << temp;			//2状态接收到一个非空白字符 则输出最近的一个空白字符
				}
				if (pre!='\n')
					pre = temp;				//如换行与制表符或空格同时出现 保留换行 保证程序大体格式不被改变
				break;
			default:
				cout << "An error occured!" << endl;
				break;						//进入default分支 说明发生比较神奇的错误
		};
	}
	return;
}

//将字符串按照空格分割,并将各部分存入数组中
int splitblank(string str, string strsplit[])
{
	int index, start, k = 0;
	string t;
	start = 0;
	while (1)		//将箭头后的部分按照空格分开
	{
		index = str.find(' ', start);
		if (index == str.npos)
			break;
		strsplit[k++].assign(str, start, index - start);
		start = index + 1;
	}
	strsplit[k++].assign(str, start, str.length() - start);
	return k;		//返回一共分为了几部分
}

//消除左递归以及公共左因子
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
			fix(part2);			//将每一行语法分为箭头前后的两个部分
			index = part2.find('|');
			p2_1.assign(part2, 0, index);
			p2_2.assign(part2, index + 1, part2.length() - index);
			fix(p2_1);
			fix(p2_2);			//将part2部分按照|前后分为两部分
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
			}			//p2_1出现part1说明出现了左递归
			else
			{
				k1 = splitblank(p2_1, str1);
				k2 = splitblank(p2_2, str2);
				
				flag = 0;
				while (str1[flag] == str2[flag])
					flag++;		//判断最左侧有几个公共因子
				if (!flag)		//flag表示公共左因子个数,为0时无公共左因子
					out << temp << endl;
				else
				{
					out << part1 << " -> ";
					for (int i = 0;i < flag;i++)
						out << str1[i] << " ";
					out << part1 << "'" << endl;	//输出替换掉的第一行
					out << part1 << "' -> ";
					for (int i = flag;i < k1;i++)
						out << str1[i] << " ";
					if (flag == k1)		//一条产生式都为公共左因子则在消除时补充empty
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

//将语法根据|拆分
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