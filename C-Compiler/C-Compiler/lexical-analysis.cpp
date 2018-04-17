#include "lexical-analysis.h"

//�ж��Ƿ�Ϊ�հ��ַ� �����޸�����ֵ
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

//�ж��Ƿ�Ϊ������ �Ƿ���1 ���򷵻�0
int Reserve(string strToken) {
	int i;
	for (i = 0; i < 6; i++) {
		if (strToken.compare(key[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

//����ȡ�ļ�����һ���ַ�
void Retract(fstream &source) {
	if (!source.eof()) {
		source.seekg(-1, ios::cur);
	}
}

//�ʷ�����
void lexicalanalysis(fstream &in, fstream &out)
{
	string strToken;
	char ch;
	while (!in.eof())
	{
		ch = in.get();
		if (in.fail())
			break;

		if (isBC(ch)) { //�����հ��ַ�
			strToken = "";
		}
		else if (isalpha(ch)) { //������ĸ

			while (isalpha(ch) || isdigit(ch)) { //��ȡ����token ֱ����һ���հ��ַ�
				strToken.push_back(ch);
				column++;
				ch = in.get();
			}

			if (Reserve(strToken)) { //�ж�token�Ƿ�Ϊ������
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
		else if (isdigit(ch)) { //��������
			while (isdigit(ch)) {	//��ȡ������
				strToken.push_back(ch);
				column++;
				ch = in.get();
			}
			//����֮�������ĸ ���ִ����ʽ��ID ����
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
			switch (ch) //����OP���ַ�
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
				default:	//δʶ��ɹ�����Ϣ ��ʾ������Ϣ
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