#include"global.h"

//打开filename文件 提供程序的输入
int infile(fstream &in, string filename)
{
	in.open(filename, ios::in);
	if (!in)
	{
		cout << "文件" << filename << "打开失败!" << endl;
		return 0;
	}
	return 1;
}

//打开filename文件 提供结果的输出
int outfile(fstream &out, string filename)
{
	out.open(filename, ios::out);
	if (!out)
	{
		cout << "文件" << filename << "打开失败!" << endl;
		return 0;
	}
	return 1;
}

//关闭输入输出文件
void closefile(fstream &file1, fstream &file2)
{
	file1.close();
	file2.close();
}

//输出文件内容
void readfile(string filename)
{
	char temp;
	fstream file;
	infile(file, filename);
	file >> noskipws;
	while (!file.eof())
	{
		file >> temp;
		cout << temp;
		if (file.fail())
			break;
	}
	cout << endl;
}

//输出分割线
void divideline()
{
	cout << "------------------------------------------" << endl;
}

//各过程处理以及界面输出
void precedure(string period, int num, string filein, string fileout,void (*func)(fstream &,fstream &),string content)
{
	cout << "period:" << period << "\tstep:" << num << "\tcontent:" << content << endl;
	cout << "\tresource_file:\t\t\"" << filein << "\"" << endl;
	cout << "\tdestination_file:\t\"" << fileout << "\"" << endl;
	if (infile(in, filein) && outfile(out, fileout))
		func(in, out);
	else
		exit(0);
	closefile(in, out);
	cout << "Done!" << endl;
	divideline();
}

//去除字符串前后多余的空格
void fix(string &str)
{
	if (str[0] == ' ')
		str.assign(str, 1, str.length() - 1);		//首字符为空格
	if (str[str.length() - 1] == ' ')
		str.assign(str, 0, str.length() - 1);		//末字符为空格
}

