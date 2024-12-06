/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-01-b1-hanoi.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include <Windows.h>

using namespace std;
/* ----------------------------------------------------------------------------------
	 本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

	 本文件要求：
	1、不允许定义全局变量（含外部全局和静态全局，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等
   ----------------------------------------------------------------------------------- */

   /* demo中首先执行此句，将cmd窗口设置为40行x120列（缓冲区宽度120列，行数9000行，即cmd窗口右侧带有垂直滚动杆）*/
int main()
{
	cct_setconsoleborder(120, 40, 120, 9000);
	while (1) {
		reset_cmd();
		cout << setfill('-') << setw(33) << "-" << setfill(' ') << endl;
		cout << "1.基本解" << endl;
		cout << "2.基本解(步数记录)" << endl;
		cout << "3.内部数组显示(横向)" << endl;
		cout << "4.内部数组显示(纵向+横向)" << endl;
		cout << "5.图形解-预备-画三个圆柱" << endl;
		cout << "6.图形解-预备-在起始柱上画n个盘子" << endl;
		cout << "7.图形解-预备-第一次移动" << endl;
		cout << "8.图形解-自动移动版本" << endl;
		cout << "9.图形解-游戏版" << endl;
		cout << "0.退出" << endl;
		cout << setfill('-') << setw(33) << "-" << setfill(' ') << endl;
		cout << "[请选择:] ";

		const char* choice = "1234567890";
		int option = menu(choice);
		option = option - '0';
		cout << option << endl;
		cout << endl << endl;

		if (!option)
			break;
		switch (option) {
			case 1:
				h1();
				break;
			case 2:
				h2();
				break;
			case 3:
				h3();
				break;
			case 4:
				h4();
				break;
			case 5:
				h5();
				break;
			case 6:
				h6();
				break;
			case 7:
				h7();
				break;
			case 8:
				h8();
				break;
			case 9:
				h9();
				break;
		}

		cout << "按回车键继续" << endl;
		while (1) {
			int next_option = _getch();
			if (next_option == 13) {
				cct_cls();
				break;
			}
		}
	}

	return 0;
}
