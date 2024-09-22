/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_functions.h"
#include <Windows.h>

using namespace std;

void get_move(int (*s)[11], int r, int c, int* ir, int* ic)
{
	cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
	int x0, y0;
	cct_getxy(x0, y0);
	while (1) {
		cct_gotoxy(x0, y0);
		char input[100];
		cin.getline(input, 99); //一级合法性
		if ((input[0] >= 'a' && input[0] <= ('a' + r - 1)
			|| input[0] >= 'A' && input[0] <= ('A' + r - 1))
			&& (input[1] >= '0' && input[1] <= '0' + c - 1)) {
			if (input[0] >= 'A' && input[0] <= ('A' + r - 1))
				*ir = input[0] - 'A';
			else
				*ir = input[0] - 'a' + 1;
			*ic = input[1] - '0' + 1;
			break;
		}
		cct_gotoxy(0, y0 + 1);
		cout << "输入错误，请重新输入.";
		cct_showch(x0, y0, ' ', black_bg, 40);
		cct_setcolor(default_bg);
	}
	// 输入为D行1列
	cct_showch(0, y0 + 1, ' ', black_bg, 60);
	cct_setcolor(default_bg);
	cct_gotoxy(0, y0 + 1);
	cout << "输入为" << char(*ir + 'A' - 1) << "行" << *ic - 1 << "列" << endl;
}

char choice_3(int (*s)[11], int r, int c, int (*h)[11], int mode)
{
	/*
	Y 找出所有可消除项后，确定是否需要合并
	N 放弃本次操作，重新输入坐标并继续完成本次消除
	Q 放弃游戏
	*/
	char option;
	int x0, y0;
	cct_getxy(x0, y0);
	while (1) {
		cct_showch(x0, y0, ' ', black_bg, 40);
		cct_setcolor(default_bg);
		cct_gotoxy(x0, y0);
		option = _getche();
		if (strchr("yYnNqQ", option) != NULL) {
			if (option >= 'a') {
				option = option - ('a' - 'A');
			}
			break;
		}
	}

	cout << endl << endl;

	if (option == 'Y') {
		cout << "相同值归并后的数组(不同色标识)：" << endl;
		print_out(s, r, c, 622, h, 2);
		return 'Y';
	}
	else if (option == 'N') {
		H2(s, r, c, 0);
		return 'N';
	}
	else if (option == 'Q') {
		return 'Q';
	}
	else
		return '1';
}

