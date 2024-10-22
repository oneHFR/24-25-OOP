/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string.h>
#include<iomanip>

using namespace std;
#define id_num 7

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int usage(const char* const procname)
{
	cout << "Usage: " << procname << " 要检查的学号/all 匹配学号/all 源程序名/all 相似度阀值(60-100) 输出(filename/screen)" << endl << endl;
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
		cout << prompt << id_num << "位" << endl;
		return 0;
	}
	for (int i = 0; i < id_num; i++) {
		if (p[i] > '9' || p[i] < '0') {
			cout << prompt << id_num << "位数字" << endl;
			return 0;
		}
	}
	return 1;
}

/*
Usage: b3 要检查的学号/all 匹配学号/all 源程序名/all 相似度阀值(60-100) 输出(filename/screen)

e.g. : b3 2159999 2159998 all       80 screen
	   b3 2159999 all     14-b1.cpp 75 result.txt
	   b3 all     all     14-b2.cpp 80 check.dat
	   b3 all     all     all       85 screen

	D:\OOP\14-07\Debug>b3 all al 3 4 5
要匹配的学号不是7位

*/

int valid(char* code, char* threshold, char* result)
{
	if (strlen(code) > 32)
		cout << "源程序文件名超过了32字节";

	int num = atoi(threshold);
	if (num < 60 || num>100)
		strcpy(threshold, "80"); // 我去？会覆盖？？
		return -1;

	if (strlen(result) > 32)
		cout << "输出结果文件名超过了32字节";

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 6)
		return usage(argv[0]);

	int stu1 = check_id(argv[1], "要检查的学号不是");
	int stu2 = check_id(argv[2], "要匹配的学号不是");

	if (!(stu1 && stu2)) {
		return -1;
	}
	if (strcmp(argv[1], "all") == 0) {
		if (strcmp(argv[2], "all") != 0) {
			cout << "检查学号是all，匹配学号必须是all" << endl;
			return -1;
		}
	}
	if (!strcmp(argv[1], argv[2]) && strcmp(argv[2], "all") != 0){
		cout << "匹配学号与要检查学号相同\n";
		return -1;
	}

	int helper = valid(argv[argc - 3], argv[argc - 2], argv[argc - 1]);

	cout << "参数检查通过" << endl;
	const char* prompts[] = { "检查学号", "匹配学号", "源文件名", "匹配阈值", "输出目标" };
	for (int i = 1; i < argc; i++)
		if (helper == -1 && i == 4) // 这个特判也太捞了！
			cout << prompts[i - 1] << "：" << 80 << endl;
		else
			cout << prompts[i - 1] << "：" << argv[i] << endl;

	return 0;
}
