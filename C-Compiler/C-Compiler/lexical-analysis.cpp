#include "lexical-analysis.h"

//判断是否为空白字符 并且修改行列值
int isBC(char ch) {
	switch (ch) 
	{
		case ' ':
			column++;
			break;
		case '\t':
			column += 4;
			break;
		case '\r':
		case '\n':
			line++;
			column = 1;
			break;
		default:
			return 0;
	}
	return 1;
}

//判断是否为保留字 是返回1 否则返回0
int Reserve(string strToken) {
	int i;
	for (i = 0; i < 6; i++) {
		if (strToken.compare(key[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

//所读取文件回退一个字符
void Retract(fstream &source) {
	if (!source.eof()) {
		source.seekg(-1, ios::cur);
	}
}

//词法分析
void lexicalanalysis(fstream &in, fstream &out)
{
	string strToken;
	char ch;
	while (!in.eof())
	{
		ch = in.get();
		if (in.fail())
			break;

		if (isBC(ch)) { //遇到空白字符
			strToken = "";
		}
		else if (isalpha(ch)) { //遇到字母

			while (isalpha(ch) || isdigit(ch)) { //读取整个token 直到下一个空白字符
				strToken.push_back(ch);
				column++;
				ch = in.get();
			}

			if (Reserve(strToken)) { //判断token是否为保留字
				//cout << strToken << ", RESERVED" << endl;
				out << strToken << " RESERVED" << " " << line << " " << column - strToken.length() << endl;
			}
			else {
				// cout << strToken << ", ID" << endl;
				out << strToken << " ID" << " " << line << " " << column - strToken.length() << endl;
			}

			strToken = "";
			Retract(in);
		}
		else if (isdigit(ch)) { //遇到数字
			while (isdigit(ch)) {	//读取整个数
				strToken.push_back(ch);
				column++;
				ch = in.get();
			}
			//数字之后出现字母 发现错误格式的ID 报错
			if (isalpha(ch)) {
				cout << "[Lexical ERROR] " << " [" << line << "," << column - strToken.length() << "] " << "Invalid ID: ";

				while (isalpha(ch) || isdigit(ch)) {
					strToken.push_back(ch);
					column++;
					ch = in.get();
				}

				cout << "\"" << strToken << "\"" << endl;
				out << "^ " << strToken << " ID " << line << " " << column - strToken.length() << endl;
				out << "[Lexical ERROR] " << " [" << line << "," << column - strToken.length() << "] " << "Invalid ID: ";
				out << "\"" << strToken << "\"" << endl;
			}
			else {
				//cout << strToken << ", INT" << endl;
				out << strToken << " NUM" << " " << line << " " << column - strToken.length() << endl;
			}

			Retract(in);
			strToken = "";
		}
		else {
			switch (ch) //其他OP类字符
			{ 
				case '=':
					column++;
					ch = in.get();
					if (ch == '=')
					{
						column++;
						out << "== COP" << " " << line << " " << column - 2 << endl;
					}
					else {
						out << "=  AOP" << " " << line << " " << column - 1 << endl;
						Retract(in);
					}
					break;
				case '<':
					column++;
					ch = in.get();
					if (ch == '=') {
						column++;
						//cout << "<=, COP" << endl;
						out << "<= COP" << " " << line << " " << column - 2 << endl;
					}
					else {
						//cout << "<, COP" << endl;
						out << "<  COP" << " " << line << " " << column - 1 << endl;
						Retract(in);
					}
					break;
				case '>':
					column++;
					ch = in.get();
					if (ch == '=') {
						column++;
						//cout << ">=, COP" << endl;
						out << ">=  COP" << " " << line << " " << column - 2 << endl;
					}
					else {
						//cout << ">, COP" << endl;
						out << ">  COP" << " " << line << " " << column - 1 << endl;
						Retract(in);
					}
					break;
				case '!':
					column++;
					ch = in.get();
					if (ch == '=') {
						column++;
						out << "!=  COP" << " " << line << " " << column - 2 << endl;
					}
					else
					{
						//cout << "[Lexical ERROR] " << " [" << line << "," << column << "] " << "Invalid COP ";
						out << "[Lexical ERROR] " << " [" << line << "," << column - 1 << "] " << "Invalid COP ";
						Retract(in);
					}
					break;
				case '+':
				case '-':
				case '/':
				case '*':
					column++;
					//cout << ch << ", OOP" << endl;
					out << ch << " OOP" << " " << line << " " << column - 1 << endl;
					break;
				case ';':
					column++;
					//cout << ch << ", EOP" << endl;
					out << ch << " EOP" << " " << line << " " << column - 1 << endl;
					break;
				case '(':
				case ')':
				case ',':
				case '[':
				case ']':
				case '{':
				case '}':
					column++;
					//cout << ch << ", SOP" << endl;
					out << ch << " SOP" << " " << line << " " << column - 1 << endl;
					break;
				default:	//未识别成功的信息 显示出错信息
					column++;
					//cout << ch << ", UNKNOWN" << endl;
					out << "[Lexical ERROR] " << " [" << line << "," << column - 1 << "] " << "UNKNOW EXPRESSION " << ch << endl;
			}
		}
	}
	out << "# # " << line + 1 << " 1";
}

void fixlex(void)
{
	fstream in, out;
	string temp;
	vector<struct Token>tokens;
	vector<struct Token>::iterator vec;
	Token token;
	in.open(file_lex, ios::in);
	while (!in.eof())
	{
		in >> token.re >> token.type >> token.line >> token.colume;
		if (in.fail())
			break;
		tokens.push_back(token);
	}
	for (vec = tokens.begin();vec != tokens.end();vec++)
	{
		if (vec->type == "ID" && (vec + 1)->re == "(")
			vec->type = "IDF";
		if (vec->type == "ID" && (vec + 1)->re == "=")
			vec->type = "ID1";
	}
	in.close();
	out.open(file_lex, ios::out);
	for (vec = tokens.begin();vec != tokens.end();vec++)
		out << vec->re << " " << vec->type << " " << vec->line << " " << vec->colume << endl;
	out.close();
}