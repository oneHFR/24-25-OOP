/* 2152131 ����� �ƿ� */
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
using namespace std;

#define LEN 256

//Usage: d3 --check �ļ��� | --convert{ wtol | ltow } Դ�ļ��� Ŀ���ļ���
//d3 --check a.txt
//d3 --convert wtol a.win.txt a.linux.txt
//d3 --convert ltow a.linux.txt a.win.txt
//ת����ɣ�ȥ��4��0x0D

int usage(const char* procname)
{
	cout << "Usage: " << procname << " --check �ļ��� | --convert { wtol|ltow } Դ�ļ��� Ŀ���ļ���" << endl;
	cout << "               " << procname << " --check a.txt" << endl;
	cout << "               " << procname << " --convert wtol a.win.txt a.linux.txt" << endl;
	cout << "               " << procname << " --convert ltow a.linux.txt a.win.txt" << endl;
	return 0;
}

int check(ifstream& in, char** argv, int index)
{
	in.open(argv[index], ios::in | ios::binary);
	if (!in.is_open()) {
		cout << "�ļ���ʧ��" << endl;
		return -2;
	}
	char curr_ch = 0, prev_ch = 0;
	int win = 0, lin = 0;

	while ((curr_ch = in.get()) != EOF) {
		if (curr_ch == 10 && prev_ch == 13) //  (CRLF)
			win = 1;
		else if (curr_ch == 10 && prev_ch != 13) // (LF)
			lin = 1;
		prev_ch = curr_ch;
	}
	in.close();
	if ((win && lin) || (!win && !lin))
		return -1;
	else if (win)
		return 0;
	else if (lin)
		return 1;
}

void wtol(ifstream& in, ofstream& out, char** argv)
{
	int remove_num = 0;
	in.open(argv[3], ios::in | ios::binary);
	out.open(argv[4], ios::out | ios::binary);

	if (!in.is_open() || !out.is_open())
		cout << "�ļ���ʧ��" << endl;

	char curr_ch = 0;
	while ((curr_ch = in.get()) != EOF) {
		if (curr_ch == 13) {  // �Ƴ� '\r' �ַ�
			remove_num++;
			continue;
		}
		else
			out.put(curr_ch);
	}
	cout << "ת����ɣ�ȥ��" << remove_num << "��0x0D" << endl;
}

void ltow(ifstream& in, ofstream& out, char** argv)
{
	int add_num = 0;
	in.open(argv[3], ios::in | ios::binary);
	out.open(argv[4], ios::out | ios::binary);

	if (!in.is_open() || !out.is_open())
		cout << "�ļ���ʧ��" << endl;

	char curr_ch = 0;
	while ((curr_ch = in.get()) != EOF) {
		if (curr_ch == 10) {
			add_num++;
			out.put(13);
		}
		out.put(curr_ch);
	}
	cout << "ת����ɣ�����" << add_num << "��0x0D" << endl;
}

int main(int argc, char* argv[])
{
	char file_name[LEN];
	ifstream in(file_name, ios::in | ios::binary);
	ofstream out;

	if ((argc != 3) && (argc != 5)) {
		usage(argv[0]);
		;
	}
	
	if (argc == 3) {
		if (strcmp(argv[1], "--check")) { // ÿ����strcmp�����÷���ֵ�����
			usage(argv[0]);
			;
		}
		else {
			int format = check(in, argv, 2);
			if (format == -2)
				;
			else if (format == -1)
				cout << "�ļ���ʽ�޷�ʶ��" << endl;
			else if (format == 0)
				cout << "Windows��ʽ" << endl;
			else if (format == 1)
				cout << "Linux��ʽ" << endl;
		}
	}

	else if (argc == 5) {
		if (strcmp(argv[1], "--convert")) { // ÿ����strcmp�����÷���ֵ�����
			usage(argv[0]);
			;
		}
		if (strcmp(argv[2], "wtol") && strcmp(argv[2], "ltow")) {
			usage(argv[0]);
			;
		}
		// ��check ��convert
		int format = check(in, argv, 3);
		if (format == -2)
			;
		if (format == -1)
			cout << "�ļ���ʽ�޷�ʶ��" << endl;
		if (format == 0)
			wtol(in, out, argv);
		if (format == 1)
			ltow(in, out, argv);
	}

	out.close();
	in.close();

	return 0;
}