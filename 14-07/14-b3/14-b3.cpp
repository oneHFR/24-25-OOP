/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>
#include<iomanip>

using namespace std;
#define id_num 7

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int usage(const char* const procname)
{
	cout << "Usage: " << procname << " Ҫ����ѧ��/all ƥ��ѧ��/all Դ������/all ���ƶȷ�ֵ(60-100) ���(filename/screen)" << endl << endl;
	cout << "e.g. : " << procname << " 2159999 2159998 all       80 screen" << endl;
	cout << "       " << procname << " 2159999 all     14-b1.cpp 75 result.txt" << endl;
	cout << "       " << procname << " all     all     14-b2.cpp 80 check.dat" << endl;
	cout << "       " << procname << " all     all     all       85 screen" << endl;

	return 0;
}

int check_id(char* p, const char* prompt)
{
	if (!strcmp(p, "all"))
		return 1;
	if (strlen(p) != id_num) {
		cout << prompt << id_num << "λ" << endl;
		return 0;
	}
	for (int i = 0; i < id_num; i++) {
		if (p[i] > '9' || p[i] < '0') {
			cout << prompt << id_num << "λ����" << endl;
			return 0;
		}
	}
	return 1;
}

/*
Usage: b3 Ҫ����ѧ��/all ƥ��ѧ��/all Դ������/all ���ƶȷ�ֵ(60-100) ���(filename/screen)

e.g. : b3 2159999 2159998 all       80 screen
	   b3 2159999 all     14-b1.cpp 75 result.txt
	   b3 all     all     14-b2.cpp 80 check.dat
	   b3 all     all     all       85 screen

	D:\OOP\14-07\Debug>b3 all al 3 4 5
Ҫƥ���ѧ�Ų���7λ

*/

int valid(char* code, char* threshold, char* result)
{
	if (strlen(code) > 32)
		cout << "Դ�����ļ���������32�ֽ�";

	int num = atoi(threshold);
	if (num < 60 || num>100)
		strcpy(threshold, "80"); // ��ȥ���Ḳ�ǣ���
		return -1;

	if (strlen(result) > 32)
		cout << "�������ļ���������32�ֽ�";

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 6)
		return usage(argv[0]);

	int stu1 = check_id(argv[1], "Ҫ����ѧ�Ų���");
	int stu2 = check_id(argv[2], "Ҫƥ���ѧ�Ų���");

	if (!(stu1 && stu2)) {
		return -1;
	}
	if (strcmp(argv[1], "all") == 0) {
		if (strcmp(argv[2], "all") != 0) {
			cout << "���ѧ����all��ƥ��ѧ�ű�����all" << endl;
			return -1;
		}
	}
	if (!strcmp(argv[1], argv[2]) && strcmp(argv[2], "all") != 0){
		cout << "ƥ��ѧ����Ҫ���ѧ����ͬ\n";
		return -1;
	}

	int helper = valid(argv[argc - 3], argv[argc - 2], argv[argc - 1]);

	cout << "�������ͨ��" << endl;
	const char* prompts[] = { "���ѧ��", "ƥ��ѧ��", "Դ�ļ���", "ƥ����ֵ", "���Ŀ��" };
	for (int i = 1; i < argc; i++)
		if (helper == -1 && i == 4) // �������Ҳ̫���ˣ�
			cout << prompts[i - 1] << "��" << 80 << endl;
		else
			cout << prompts[i - 1] << "��" << argv[i] << endl;

	return 0;
}
