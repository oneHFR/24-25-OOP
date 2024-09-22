/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"

using namespace std;

int menu(const char *choice)
{
	char option;
	while (1) {
		option = _getch();
		if (strchr(choice, option) != NULL)
			break;
	}

	return option;
}

void reset_cmd()
{
	cct_cls();
	cct_setfontsize("新宋体", 16);
	cct_setconsoleborder(128, 30, 150, 1000);
	//cct_setconsoleborder(120, 40, 120, 9000);
}
/* ----------------------------------------------------------------------------------

1.菜单项的显示
	★ 字符指针数组
	★ 二维字符数组
	★ 一维字符数组/指针
2.选项的判断
	★ 连续区间(0-9、A-Q等)
	★ 部分连续区间(A-G、Q)
	★ 字符的大小敏感/不敏感
	★ 数字/字母混合
3.选项的处理
	★ menu函数中
	★ menu函数返回后

   ----------------------------------------------------------------------------------- */