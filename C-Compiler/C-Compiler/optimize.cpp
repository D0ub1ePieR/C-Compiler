#include"optimize.h"

map<int, vector<string>> gb;		//中间代码表
map<int, vector<string>>::iterator gber;
map<int, vector<string>>::iterator gbertemp;
map<string, double>::iterator con;
vector<string>::iterator ier;
vector<string>::iterator iertemp;
vector<int> entry;			//基本块入口地址表
vector<int>::iterator it,itertemp;
int endline;	//中间代码的最后一条

map<string, int> op;

void init_op()
{
	op.insert({ "+",1 });
	op.insert({ "-",2 });
	op.insert({ "*",3 });
	op.insert({ "/",4 });
	op.insert({ "==",5 });
	op.insert({ "!=",6 });
	op.insert({ "<=",7 });
	op.insert({ "<",8 });
	op.insert({ ">=",9 });
	op.insert({ ">",10 });
}

//读入所有三地址代码
void getbetter1(fstream &in) {
	string str, part1, part2, t;
	int split = 0, index, start, num;
	while (!in.eof()) {
		getline(in, str);
		if (in.fail())
			break;
		split = str.find(' ');
		part1.assign(str, 0, split);
		part1 = part1.substr(1, part1.length() - 1);
		part2.assign(str, split + 1, str.length() - split - 1);
		fix(part1);
		fix(part2);
		//读入每一行中间代码 将代码地址 与代码内容分开
		start = 0;
		stringstream ss;
		ss << part1;
		num = atoi(ss.str().c_str());
		gb.insert({ num,{} });
		//将地址提取为数字 插入代码表中
		while (1) {
			index = part2.find(' ', start);
			if (index == part2.npos)
				break;
			t.assign(part2, start, index - start);
			start = index + 1;
			gb[num].push_back(t);
		}
		t.assign(part2, start, part2.length() - start);
		gb[num].push_back(t);
	}
	//得到所有基本块的入口
	for (gber = gb.begin(); gber != gb.end(); gber++)
	{
		//1.程序的第一条语句
		if (gber == gb.begin())
			entry.push_back(gber->first);
		//2.强制跳转语句的目的地址
		for (ier = gber->second.begin(); ier != gber->second.end(); ier++) {
			if (*ier == "Goto") {
				iertemp = ier + 1;
				stringstream ss;
				ss << *iertemp;
				num = atoi(ss.str().c_str());
				itertemp = find(entry.begin(), entry.end(), num);
					if (itertemp == entry.end())
					entry.push_back(num);
			}
			//3.条件语句的下一条语句
			if (*ier == "if") {
				gbertemp = gber;
				gbertemp++;
				gbertemp++;
				itertemp = find(entry.begin(), entry.end(), gbertemp->first);
				if (itertemp == entry.end())
					entry.push_back(gbertemp->first);
			}
			//4.函数的第一条语句
			if (*ier == "Proc")
				entry.push_back(gber->first);
		}
	}
	gber--;
	//记录所有代码地址的最后一条
	endline = gber->first;
}

//
int checkdigit(string str)
{
	int i;
	for (i = 0;i < str.length();i++)
		if (!isdigit(str[i]))
			return 0;
	return 1;
}

//
int checkvar(string str, map<string, double> cons)
{
	for (con = cons.begin();con != cons.end();con++)
		if (con->first == str)
			return 1;
	return 0;
}

//
void getbetter2(int start,int end)
{
	int i;
	int flag = 0;
	map<string, double> constant;
	for (i=start;i<=end;i++)
	{
		flag = 0;
		if (gb[i].size() == 3 && checkdigit(gb[i][2])) 
		{
			stringstream ss;
			ss << gb[i][2];
			double ll;
			ll = atol(ss.str().c_str());
			constant.insert({ gb[i][0] ,ll });
		}
		if (gb[i].size() == 3)
		{
			for (con = constant.begin(); con != constant.end(); con++)
			{
				if (con->first == gb[i][2])
					gb[i][2] = to_string(con->second);
			}
		}
		if (gb[i].size() == 5) 
		{
			if (checkdigit(gb[i][2]))
				flag++;
			if (checkdigit(gb[i][4]))
				flag++;
			for (con = constant.begin(); con != constant.end(); con++)
			{
				if (con->first == gb[i][2])
				{
					gb[i][2] = to_string(con->second);
					flag++;
				}
				if (con->first == gb[i][4])
				{
					gb[i][4] = to_string(con->second);
					flag++;
				}
			}
			if (flag == 2)
			{
				double b, d;
				b = atol(gb[i][2].c_str());
				d = atol(gb[i][4].c_str());
				switch (op[gb[i][3]])
				{
					case 1:
						gb[i][2] = to_string(b + d);
						break;
					case 2:
						gb[i][2] = to_string(b - d);
						break;
					case 3:
						gb[i][2] = to_string(b * d);
						break;
					case 4:
						gb[i][2] = to_string(b / d);
						break;
					case 5:
						gb[i][2] = to_string(b == d);
						break;
					case 6:
						gb[i][2] = to_string(b != d);
						break;
					case 7:
						gb[i][2] = to_string(b <= d);
						break;
					case 8:
						gb[i][2] = to_string(b < d);
						break;
					case 9:
						gb[i][2] = to_string(b >= d);
						break;
					case 10:
						gb[i][2] = to_string(b > d);
						break;
				};
				gb[i].pop_back();
				gb[i].pop_back();
				constant.insert({gb[i][0],atol(gb[i][2].c_str())});
			}
			if (checkvar(gb[i][0], constant) && flag == 0)
			{
				constant.erase(gb[i][0]);
			}
		}
	}
}

//优化中间代码
void optimize(fstream &in, fstream &out)
{
	init_op();
	getbetter1(in);
	//输出基本块信息
	out << "基本块入口:" << endl;
	for (it = entry.begin();it != entry.end();it++)
		out << *it << endl;
	for (it = entry.begin();it != entry.end();it++)
	{
		itertemp = it;
		itertemp++;
		if (itertemp == entry.end())
			getbetter2(*it, endline);
		else
			getbetter2(*it, *itertemp - 1);
	}
	for (gber = gb.begin();gber != gb.end();gber++)
	{
		out << gber->first << " ";
		for (ier = gber->second.begin();ier != gber->second.end();ier++)
			out << *ier << " ";
		out << endl;
	}
}