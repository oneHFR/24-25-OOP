/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_functions.h"
#include "../90-01-b2/90-01-b2-magic_ball.h"
#include "../90-02-b1/90-02-b1.h"
#include <Windows.h>

using namespace std;

/*
* BASIC
1、菜单已单独显示 >>> common_menu.cpp
2、查找指定坐标位置及周围相同项的函数 两个search的逻辑不一样无法合并 >>> check_1/2 
3、判断整个游戏(彩球)/本关(消灭星星)是否结束的函数 >>> hint
4、内部数组进行位置移动的函数 >>> falling_down
5、命令行下打印数组的函数(包括反显、特殊位置标注等) >>> print_out
6、上下两个状态栏的内容显示函数 >>> 这是什么？
7、显示初始外框架的函数 >>> print_canvas
8、绘制一个色块的函数 >>> print_circle
9、移动一个色块的函数 >>> remove_circle

*ADVANCE
1、用随机值填充矩阵的函数，两个项目要求的随机值方式有所不同，如何提炼共用? >>>  fill / input / initializa_input
2、不能采用 if-else 方式将两个完全不同的功能合并为一个函数,而应该近可能采用提取公共步骤的方法(公共步骤占到 50%+才有合并的必要)
3、各人自行定义，无具体要求，如果未提炼任何公用函数，也需要提交一个能集成在项目中编译成功的文件(仅包含一些头文件、命名空间即可
4、根据提炼结果，可以给予额外加分，具体分值待定说明:上述要求的函数放在common 中，函数声明及公共宏定义放在 include 中
*/

/*------------------------------ 初始化区----------------------------------*/
void input(int* r, int* c, int min, int max)
{
	while (1) {
		cout << "请输入行数(" << min << "-" << max << ")：" << endl;
		cin >> *r;
		if (*r >= min && *r <= max && cin.good()) {
			cin.ignore(1024, '\n');
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
		}
	}
	while (1) {
		cout << "请输入列数(" << min << "-" << max << ")：" << endl;
		cin >> *c;
		if (*c >= min && *c <= max && cin.good()) {
			cin.ignore(1024, '\n');
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
		}
	}
	cout << endl;
}

void initializa_input(int s[11][11], int r, int c, int mode) // 数组定义必须是常数
{
	if (!mode)
		return ;
	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= 10; j++) {
			s[i][j] = -1;
		}
	}
}

void fill(int s[11][11], int r, int c, int option, int mode)
{
	if (!mode)
		return ;
	srand((unsigned)time(NULL));
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			s[i][j] = rand() % option + 1;
		}
	}
	//s[9][1] = 2;
	//s[9][2] = 2;

}

/*------------------------------ 移动区----------------------------------*/
void remove_0(int (*s)[11], int r, int c, int(*h)[11], int mode, char timu)
{
	// 2 只侧移
	// 1 只变成0
	// 0 下落
	if (mode != 2) {
		for (int j = 1; j <= c; j++) {
			for (int i = 1; i <= r; i++) {
				if (h[i][j] == 1) {
					s[i][j] = 0;
					if (!mode)
						if (timu == 'm')
							h[i][j] = 0; //初始化为后面零的位置做记号做准备
				}
			}
		}

		if (mode != 1) {
			for (int j = 1; j <= c; j++) {
				for (int i = r; i > 0; i--) {
					if (s[i][j] == 0) {
						for (int w = i; w > 1; w--) {
							s[w][j] = s[w - 1][j];
						}
						s[1][j] = 0;
						int helper_i = 0;
						for (int w = i; w > 0; w--) {
							if (s[w][j] == 0) {
								helper_i = 0;
							}
							else {
								helper_i = 1;
								break;
							}
						}
						if (helper_i) {
							i++;
						}
					}
				}
			}
		}
	}


	else if (mode == 2){
		for (int j = 1; j <= c; j++) {
			int helper_ceyi = 1;
			for (int i = 1; i <= r; i++) {
				if (s[i][j] != 0) {
					helper_ceyi = 0;
					break;
				}
			}
			if (helper_ceyi) {
				for (int j1 = j + 1; j1 <= c; j1++) {
					for (int i1 = 1; i1 <= r; i1++) {
						s[i1][j1 - 1] = s[i1][j1];
					}
				}
				for (int i1 = 1; i1 <= r; i1++) {
					s[i1][c] = 0;
				}
			}
		}
		for (int j = 1; j <= c; j++) {
			int helper_ceyi = 1;
			for (int i = 1; i <= r; i++) {
				if (s[i][j] != 0) {
					helper_ceyi = 0;
					break;
				}
			}
			if (helper_ceyi) {
				for (int j1 = j + 1; j1 <= c; j1++) {
					for (int i1 = 1; i1 <= r; i1++) {
						s[i1][j1 - 1] = s[i1][j1];
					}
				}
				for (int i1 = 1; i1 <= r; i1++) {
					s[i1][c] = 0;
				}
			}
		}
		for (int j = 1; j <= c; j++) {
			int helper_ceyi = 1;
			for (int i = 1; i <= r; i++) {
				if (s[i][j] != 0) {
					helper_ceyi = 0;
					break;
				}
			}
			if (helper_ceyi) {
				for (int j1 = j + 1; j1 <= c; j1++) {
					for (int i1 = 1; i1 <= r; i1++) {
						s[i1][j1 - 1] = s[i1][j1];
					}
				}
				for (int i1 = 1; i1 <= r; i1++) {
					s[i1][c] = 0;
				}
			}
		}
		// 这里有一个bug 如果同时出现两个空列需要再一次消除 flag
	}

}


/*------------------------------ prompt区----------------------------------*/
void enter_option(int option, int r)
{
	if (option == 6 || option == 7 || option == 5) {
		clear_instrcution(r);
	}
	if (option == 1) {
		cout << "按回车键进行寻找初始可消除项的操作..." << endl; // 1
	}
	else if (option == 2 || option == 5) {
		cout << "按回车键进行数组下落除0操作..." << endl;
		if (option == 2)
			cout << "下落除0后的数组(不同色标识)：" << endl;
	}
	else if (option == 3 || option == 6) {
		cout << "按回车键进行新值填充..." << endl;
		if (option == 3)
			cout << "新值填充后的数组(不同色标识)：" << endl;
	}
	else if (option == 4) {
		cout << "按回车键显示图形..." << endl;
	}
	else if (option == 7) {
		cout << "按回车键显示消除提示..." << endl;
	} // 			cout << "按回车键进行数组下落操作..." << endl;
	else if (option == 631) {
		cout << "按回车键进行数组下落操作..." << endl;
	}
	else if (option == 632) {
		cout << "本次消除结束，按回车键继续新一次的消除..." << endl;
	}

	int x;
	while (1)
	{
		x = _getch();
		if (x == 13)
			break;
	}
}

/*------------------------------ 打印区----------------------------------*/
void print_out(int (*s)[11], int r, int c, int option, int (*h)[11], int mode)
{
	if (option == 1) {
		cout << "初始数组：" << endl;
	}
	if (option == 61) {
		cout << "当前数组：" << endl;
	}
	for (int j = 0; j <= c; j++) {
		cct_setcolor(default_bg);
		if (j <= 0)
			cout << setw(3) << "|";
		else {
			if (mode == 2) {
				cout << setw(3) << j - 1;
			}
			else
				cout << setw(3) << j;
		}
	}
	cout << endl << "--+" << setfill('-') << setw(3 * c + 1) << "-" << setfill(' ') << endl;

	if (option == 1 || option == 61 || option == 621 || option == 622 || option == 10 || option == 11 || option == 12) {
		cct_setcolor(default_bg);
		for (int i = 1; i <= r; i++) {
			for (int j = 0; j <= c; j++) {
				if (!j) {
					cout << char('A' + i - 1) << " |";
				}
				else {
					cout << "  "; // 2 space
					if (option == 1 || option == 61) {
						cout << s[i][j];
					}
					else if (option == 621) {
						if (h[i][j] == 1 && s[i][j] != 0)
							cout << '*';
						else
							cout << h[i][j];
					}
					else if (option == 10 || option == 622) {
						if (h[i][j] == 1) {
							cct_setcolor(14, 1);
							if (option == 10)
								cout << s[i][j];
							else if (option == 622)
								cout << 0;
							cct_setcolor(0);
						}
						else {
							cct_setcolor(0);
							cout << s[i][j];
						}
					}
					else if (option == 11) {
						if (s[i][j] == 0) {
							cct_setcolor(14, 1);
							cout << s[i][j];
							h[i][j] = 1; // flag
							cct_setcolor(0);
						}
						else {
							cct_setcolor(0);
							cout << s[i][j];
						}
					}
					else if (option == 12) {
						if (h[i][j] == 1) {
							cct_setcolor(14, 1);
							cout << s[i][j];
							cct_setcolor(0);
							h[i][j] = 0;
						}
						else {
							cct_setcolor(0);
							cout << s[i][j];
						}
					}
				}
			}
			cout << endl;
		}
	}
	cout << endl;
}

void clear_instrcution(int r)
{
	cct_setcolor(0);
	cct_gotoxy(0, 2 * r + 2);
	cout << "                                                      ";
	cct_gotoxy(0, 2 * r + 2);
}


/*------------------------------ 判断区----------------------------------*/
int check_1(int (*s)[11], int r, int c, int (*h)[11], int option)
{
	int helper = 0;
	int helper_check_1 = 0;
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (s[i][j] == s[i][j + 1] && s[i][j] == s[i][j + 2] && s[i][j] != -1) {
				h[i][j] = h[i][j + 1] = h[i][j + 2] = 1;
				helper = 1;
			}
		}
	}
	for (int j = 1; j <= c; j++) {
		for (int i = 1; i <= r; i++) {
			if (s[i][j] == s[i + 1][j] && s[i][j] == s[i + 2][j] && s[i][j] != -1) {
				h[i][j] = h[i + 1][j] = h[i + 2][j] = 1;
				helper = 1;
			}
		}
	}
	helper_check_1 = helper;
	if (option != 9) {
		if (helper == 0) {
			cout << "初始已无可消除项" << endl;
			return helper_check_1;
		}
		else {
			cout << "初始可消除项（不同色标识）：" << endl;  // 1
		}
	}

	return helper_check_1;
}

int hint(int(*s)[11], int r, int c, int(*h)[11], int print_hint_circle)
{
	int ts = 0;
	int helper_hint = 0;
	// 还用h会不会冲突 先用着吧
	// 很烦这个有边界限制
	// 我能不能读到另外一个二位数组上
	int ds[20][20] = { 0 };
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			ds[i][j] = 0;
		}
	}
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			ds[i + 1][j + 1] = s[i][j];
			h[i][j] = 0;
		}
	}

	for (int i = 2; i <= r; i++) {
		for (int j = 2; j <= c; j++) {
			// up
			if (ds[i - 1][j]) {
				ts = ds[i - 1][j];
				ds[i - 1][j] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j - 1])
					|| (ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j + 2])
					|| (ds[i][j] == ds[i][j - 1] && ds[i][j] == ds[i][j - 2])
					|| (ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i + 2][j]) && ds[i][j] != 0) {
					// 标记加还原
					h[i - 1][j - 1] = 1;
					h[i - 2][j - 1] = 1;
					helper_hint = 1;
				}
				ts = ds[i - 1][j];
				ds[i - 1][j] = ds[i][j];
				ds[i][j] = ts;
			}

			// down
			if (ds[i + 1][j]) {
				ts = ds[i + 1][j];
				ds[i + 1][j] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j - 1])
					|| (ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j + 2])
					|| (ds[i][j] == ds[i][j - 1] && ds[i][j] == ds[i][j - 2])
					|| (ds[i][j] == ds[i - 1][j] && ds[i][j] == ds[i - 2][j]) && ds[i][j]) {
					// 标记加还原
					h[i - 1][j - 1] = 1;
					h[i][j - 1] = 1;
					helper_hint = 1;
				}
				ts = ds[i + 1][j];
				ds[i + 1][j] = ds[i][j];
				ds[i][j] = ts;
			}

			// left
			if (ds[i][j - 1]) {
				ts = ds[i][j - 1];
				ds[i][j - 1] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i - 1][j])
					|| (ds[i][j] == ds[i - 1][j] && ds[i][j] == ds[i - 2][j])
					|| (ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i + 2][j])
					|| (ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j + 2]) && ds[i][j]) {
					// 标记加还原
					h[i - 1][j - 1] = 1;
					h[i - 1][j - 2] = 1;
					helper_hint = 1;
				}
				ts = ds[i][j - 1];
				ds[i][j - 1] = ds[i][j];
				ds[i][j] = ts;
			}

			// right
			if (ds[i][j + 1]) {
				ts = ds[i][j + 1];
				ds[i][j + 1] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i - 1][j])
					|| (ds[i][j] == ds[i - 1][j] && ds[i][j] == ds[i - 2][j])
					|| (ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i + 2][j])
					|| (ds[i][j] == ds[i][j - 1] && ds[i][j] == ds[i][j - 2]) && ds[i][j]) {
					//// 标记加还原
					h[i - 1][j - 1] = 1;
					h[i - 1][j] = 1;
					helper_hint = 1;
				}
				ts = ds[i][j + 1];
				ds[i][j + 1] = ds[i][j];
				ds[i][j] = ts;
			}
		}
	}

	if (print_hint_circle) {
		int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		for (int i = 1; i <= r; i++) {
			for (int j = 1; j <= c; j++) {
				if (h[i][j] == 1) {
					cct_gotoxy(4 * j - 2, 2 * i);
					cct_setcolor(color[s[i][j]], 0);
					cout << "◎"; // ●◎
				}
			}
		}
		clear_instrcution(r);
	}
	return helper_hint;
}

void search(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int target, int(*pool)[12], int option) {
	int i = 0, j = 0;
	//向上
	if (option != 1) {
		for (i = ir - 1; i >= 0; i--) {
			if (pool[i][ic] != target) {
				break;
			}
			else if (h[i][ic] != 1) {
				h[i][ic] = 1;
				search(s, r, c, h, i, ic, target, pool, 2);
			}
		}
	}

	//向下
	if (option != 2) {
		for (i = ir + 1; i <= r; i++) {
			if (pool[i][ic] != target) {
				break;
			}
			else if (h[i][ic] != 1) {
				h[i][ic] = 1;
				search(s, r, c, h, i, ic, target, pool, 1);
			}
		}
	}

	//向左
	if (option != 3) {
		for (j = ic - 1; j >= 0; j--) {
			if (pool[ir][j] != target) {
				break;
			}
			else if (h[ir][j] != 1) {
				h[ir][j] = 1;
				search(s, r, c, h, ir, j, target, pool, 4);
			}
		}
	}

	//向右
	if (option != 4) {
		for (j = ic + 1; j <= c; j++) {
			if (pool[ir][j] != target) {
				break;
			}
			else if (h[ir][j] != 1) {
				h[ir][j] = 1;
				search(s, r, c, h, ir, j, target, pool, 3);
			}
		}
	}
}

int check_2(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int store)
{
	// 需要递归动态搜索
	// 二级合法性
	/*输入为D行1列
输入的矩阵坐标位置处无连续相同值，请重新输入*/
	h[ir][ic] = 1;
	// 考虑到之前彩球出现的问题，边框溢出，这里复制扩充尝试一下
	int pool[12][12] = { 0 };
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			pool[i][j] = s[i][j];
		}
	}
	int target = s[ir][ic];
	search(s, r, c, h, ir, ic, target, pool, 0);

	for (int j = 1; j <= c; j++) {
		for (int i = 1; i <= r; i++) {
			if (h[i][j] == 1 && !(i == ir && j == ic))
				return 1;
		}
	}
	if (!store) {
		h[ir][ic] = 0;
	}
	return 0;
}

int check_3(int (*s)[11], int r, int c, int (*h)[11]) 
{
	int check_keep = 1;
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (s[i][j]) {
				check_keep = check_2(s, r, c, h, i, j);
				if (check_keep)
					return 1;
			}
		}
	}
	return 0;
}

/*------------------------------ 悬浮区----------------------------------*/


/*------------------------------ 图形区----------------------------------*/
void set_cmd(int r, int c, int mode)
{
	cct_cls();
	int hang, lie;
	if (mode == 3) {
		cct_setfontsize("新宋体", 24);
		hang = 7 + 4 * r;
		lie = 5 + 8 * c;
	}
	else if (mode == 2) {
		cct_setfontsize("新宋体", 24);
		hang = 8 + 3 * r; // 3 * r + 4
		lie = 7 + 6 * c;
	}
	else {
		cct_setfontsize("新宋体", 32);
		hang = 6 + r;
		lie = 40;
	}
	cct_setconsoleborder(lie, hang, lie, 1000);
	cout << "屏幕：" << hang << "行" << lie << "列 ";
}

void print_canvas(int(*s)[11], int r, int c, int grid, int speed, int mode)
{ //     print_canvas(s, r, c, 1, 0);
	// grid 0 meiyou
	// grid 1 youbianjie
	// speed 1 快速！
	// speed 0 有动画！
	int x, y;
	int m;
	if (speed) 
		m = 0;
	else
		m = 0; // flag
	cct_gotoxy(0, 1);
	if (mode == 2) {
		cct_setcolor();
		if(!grid)
			cout << setw(2) << ' '; //
		for (int j = 0; j < c; j++) {
			if(!j && grid)
				cout << setw(6 + 1 * grid) << j;
			else
				cout << setw(6 + 2 * grid) << j;
		}

		cout << endl;
	}
	cct_setcolor(15, 0);
	if (mode == 2) {
		cct_gotoxy(2, 2);
		cout << "╔═";
	}
	else
		cout << "╔";
	for (int i = 1; i < c; i++) {
		Sleep(m);
		if (grid) {
			if (mode == 2)
				cout << "══╦═";
			else
				cout << "═╦"; // "═╩";
		}
		else {
			if (mode == 2)
				cout << "═══";
			else
				cout << "═";
		}
	}
	if (mode == 2)
		cout << "══╗" << endl;
	else
		cout << "═╗" << endl;
	cct_setcolor(0); 
	/*---------------------------------------------------------------------------------------*/
	if (mode == 1) {
		for (int i = 1; i <= r; i++) {
			for (int j = 0; j <= c + 1; j++) {
				if (j == 0) {
					cct_setcolor(15, 0);
					cout << "║";
					if (grid) {
						for (int w = 1; w < c; w++) {
							cout << "  ║";
						}
						cout << "  ";
					}
					else {
						cct_setcolor(0);
						cout << setfill(' ') << setw(2 * c) << ' ';
					}
					cct_setcolor(15, 0);
					cout << "║";
					Sleep(m);
				}
			}
			cout << endl;

			if (i != r && grid) {
				for (int j = 0; j <= 2 * c; j++) {
					if (j == 0) {
						cout << "╠";
					}
					else if (j == 2 * c) {
						cout << "╣";
					}
					else if (j % 2 == 0) {
						cout << "╬";
					}
					else {
						cout << "═";
					}
				}
				cout << endl;
			}
		}
	}


	else if (mode == 2) {
		if (grid) {
			for (int i = 0; i < r; i++) {
				cout << "  ";
				cct_setcolor(15, 0);
				cout << "║";
				for (int j = 0; j < c; j++) {
					cout << "      ║";
					Sleep(m);
				}
				cct_setcolor();
				cout << endl;

				cout << (char)(i + 'A') << ' ';
				cct_setcolor(15, 0);
				cout << "║";
				for (int j = 0; j < c; j++) {
					cout << "      ║";
					Sleep(m);
				}
				cct_setcolor();
				cout << endl;

				cout << "  ";
				cct_setcolor(15, 0);
				cout << "║";
				for (int j = 0; j < c; j++) {
					cout << "      ║";
					Sleep(m);
				}
				cct_setcolor();
				cout << endl;

				if (i != r - 1) {
					cout << "  ";
					cct_setcolor(15, 0);
					cout << "╠";
					for (int j = 0; j < c - 1; j++) {
						cout << "═══╬";
						Sleep(m);
					}
					cout << "═══╣";
					cct_setcolor();
					cout << endl;
				}
			}
		}
		else
			for (int i = 0; i < r; i++) {
				cout << "  ";
				cct_setcolor(15, 0);
				cout << "║";
				for (int j = 0; j < c; j++) {
					cout << "      ";
					Sleep(m);
				}
				cout << "║";
				cct_setcolor();
				cout << endl;

				cout << (char)(i + 'A') << ' ';
				cct_setcolor(15, 0);
				cout << "║";
				for (int j = 0; j < c; j++) {
					cout << "      ";
					Sleep(m);
				}
				cout << "║";
				cct_setcolor();
				cout << endl;

				cout << "  ";
				cct_setcolor(15, 0);
				cout << "║";
				for (int j = 0; j < c; j++) {
					cout << "      ";
					Sleep(m);
				}
				cout << "║";
				cct_setcolor();
				cout << endl;
			}

	}

	/*---------------------------------------------------------------------------------------*/
	cct_setcolor(15, 0);
	if (mode == 2) {
		cct_getxy(x, y);
		cct_gotoxy(2, y);
		cout << "╚═";
	}
	else
		cout << "╚";
	for (int i = 1; i < c; i++) {
		Sleep(m);
		if (grid) {
			if (mode == 2)
				cout << "══╩═";
			else
				cout << "═╩"; // "═╩";
		}
		else {
			if (mode == 2)
				cout << "═══";
			else
				cout << "═";
		}
	}
	if (mode == 2) {
		cout << "══╝" << endl;
		//cct_getxy(x, y);
		//cout << x << endl;
		//cout << y << endl;
	}
	else
		cout << "═╝" << endl;
	cct_setcolor(0);
}


void print_circle(int(*s)[11], int r, int c, int grid, int(*h)[11], int speed, int option)
{
	int m = 5;
	if (speed) {
		m = 0;
	}
	else {
		m = 5;
	}
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	if (grid) {
		for (int i = 1; i <= r; i++) {
			cct_gotoxy(0, 2 * i);
			for (int j = 1; j <= c; j++) {
				cct_setcolor(color[s[i][j]], 0);
				int x, y;
				cct_getxy(x, y);
				cct_gotoxy(x + 2, y);
				if (h[i][j] == 1) {
					cout << "●"; // 
				}
				else {
					cout << "○"; // ●
				}
				if (option != 9) {
					Sleep(5);
				}
			}
			cout << endl;
			cct_setcolor(0);
		}
	}
	else {
		for (int i = 1; i <= r; i++) {
			cct_gotoxy(2, 1 + i);
			for (int j = 1; j <= c; j++) {
				cct_setcolor(color[s[i][j]], 0);
				if (h[i][j] == 1) {
					cout << "●"; // 
				}
				else {
					cout << "○"; // ●
				}
				Sleep(m);
			}
			cout << endl;
			cct_setcolor(0);
		}
	}
}

void remove_circle(int(*s)[11], int r, int c, int(*h)[11])
{
	// 通过反复尝试找到了终于可以证明demo是横向一个一个扫描消除的
	int x, y;
	int m = 200;
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	for (int i = 1; i <= r; i++) {
		cct_gotoxy(0, 2 * i);
		for (int j = 1; j <= c; j++) {
			cct_setcolor(color[s[i][j]], 0);
			cct_getxy(x, y);
			cct_gotoxy(x + 2, y);
			if (h[i][j] == 1) {
				cout << "●"; // ● ¤ ◎
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "○"; // ● ¤ ◎
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "¤"; // ● ¤ ◎
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "○"; // ● ¤ ◎
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "¤"; // ● ¤ ◎
				cct_setcolor(15, 0);
				cct_gotoxy(x + 2, y);
				cout << "  ";
			}
			else {
				cct_gotoxy(x + 4, y);
			}
		}
	}
	clear_instrcution(r);
}

void print_star(int(*s)[11], int r, int c, int grid, int mode)
{
	// 1 变色 2 消失 3 恢复
	int color[6] = { -1, 6, 7, 8, 9, 10};
	int x, y;
	int change = 0;
	if (mode) {
		x = (2 * grid + 6) * c - 2 - 2 * grid, y = (3 + grid) * r - grid;
		if (mode == 1)
			change = 15;
		cct_setcolor(color[s[r][c]], change);
		if (mode == 2)
			cct_setcolor(15, 15);
		cct_gotoxy(x, y);
		cout << "╔═╗" << endl;
		cct_gotoxy(x, y + 1);
		cout << "║  ║" << endl;
		cct_gotoxy(x, y + 2);
		cout << "╚═╝" << endl;
		cct_gotoxy(x + 2, y + 1);
		cout << "★";
		cct_setcolor();
		if (mode == 3)
			Sleep(3);
	}
	else
		for (int i = 1; i <= r; i++) {
			for (int j = 1; j <= c; j++) {
				x = (2 * grid + 6) * j - 2 - 2 * grid, y = (3 + grid) * i - grid;
				cct_setcolor(color[s[i][j]], 0);
				//cct_setcolor(15, 15);
				cct_gotoxy(x, y);
				cct_gotoxy(x, y);
				cout << "╔═╗" << endl;
				cct_gotoxy(x, y + 1);
				cout << "║  ║" << endl;
				cct_gotoxy(x, y + 2);
				cout << "╚═╝" << endl;
				cct_gotoxy(x + 2, y + 1);
				cout << "★";
				cct_setcolor();
			}
		}
}

void n_star(int x, int y, int e, int change)
{
	int color[6] = { -1, 6, 7, 8, 9, 10 };
	cct_setcolor(color[e], change);
	cct_gotoxy(x - 2, y - 1);
	cct_getxy(x, y);
	cout << "╔═╗" << endl;
	cct_gotoxy(x, y + 1);
	cout << "║  ║" << endl;
	cct_gotoxy(x, y + 2);
	cout << "╚═╝" << endl;
	cct_gotoxy(x + 2, y + 1);
	cout << "★";
	cct_setcolor(default_bg);
}
void lan(int(*s)[11], int j, int w, int lie, int hang, int helper_down)
{
	int m = 5;
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	cct_setcolor(color[s[w][j]], 0);
	cct_gotoxy(lie, hang);
	cout << "○";
	Sleep(m);
	if (hang - 1 > 0) {
		cct_setcolor(15, 0);
		cct_gotoxy(lie, hang - 1);
		cout << "  ";
		Sleep(m);
	}

	cct_setcolor(color[s[w][j]], 0);
	cct_gotoxy(lie, hang + 1);
	cout << "○";
	Sleep(m);
	cct_setcolor(15, 0);
	cct_gotoxy(lie, hang);
	cout << "═";
}
void drop(int(*s)[11], int ir, int ic, int grid, int mode, int step = -1)
{
	int color[6] = { -1, 6, 7, 8, 9, 10 };
	int x, y;
	if (mode == 1) {
		for (int n = 0; n < step; n++) {
			for (int j = 1; j <= 3 + grid; j++) {
				x = (2 * grid + 6) *ic - 2 - 2 * grid, y = (3 + grid) * (ir + n) - grid + j;
				cct_setcolor(color[s[ir][ic]], 0);
				cct_gotoxy(x, y);
				cout << "╔═╗" << endl;
				cct_gotoxy(x, y + 1);
				cout << "║  ║" << endl;
				cct_gotoxy(x, y + 2);
				cout << "╚═╝" << endl;
				cct_gotoxy(x + 2, y + 1);
				cout << "★";
				cct_setcolor(15, 15);
				cct_gotoxy(x, y - 1);
				cout << "╔═╗" << endl;
				cct_setcolor();
				if (j == 3 + grid) {
					cct_setcolor(15, 0);
					cct_gotoxy(x, y - 1);
					cout << "═══" << endl;
					cct_setcolor();
				}
				Sleep(3);
			}
		}

	}

	else if (mode == 2) {
		int m[11] = { -1 };
		for (int j = 1; j <= ic; j++) {
			for (int o = 1; o <= ir; o++) {
				if (s[o][j] != 0) {
					m[j] = 0;
					break;
				}
				m[j] = 1;

			}
	
		}
		for (int j = 1; j <= ic; j++) {
			if (m[j] == 1) {
				//for()
				for (int i = 1; i <= 2 * grid + 6; i++) {
					x = (2 * grid + 6) * ic - 2 - 2 * grid - i, y = (3 + grid) * j - grid;
					cct_setcolor(color[s[ir][ic]], 0);
					cct_gotoxy(x, y);
					cout << "╔═╗" << endl;
					cct_gotoxy(x, y + 1);
					cout << "║  ║" << endl;
					cct_gotoxy(x, y + 2);
					cout << "╚═╝" << endl;
					cct_gotoxy(x + 2, y + 1);
					cout << "★";
					cct_setcolor(15, 15);
					cct_gotoxy(x + 6, y + 0);
					cout << ' ';
					cct_gotoxy(x + 6, y + 1);
					cout << ' ';
					cct_gotoxy(x + 6, y + 2);
					cout << ' ';
					cct_setcolor();
					Sleep(1);
				}
			}
		}

	}
}

void falling_down(int(*s)[11], int r, int c, int(*h)[11], int mode, int grid)
{
	int x, y, w;
	int lie = 2*c, hang = 2 * r;  // c r
	int already_up = 2 * r; // 最近一次下落的坐标
	int already_bottom = 2 * r; // 最后一次到底部的纵坐标 和上面的不一样
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	int helper_down = 0; // 已经下落的数量
	int step = 0; // 需要下降的高差

	for (int j = 1; j <= c; j++) {
		helper_down = 0;
		if (mode == 1) {
			lie = j * 4 - 2;
			hang = 2 * r;  // c r
			already_up = 2 * r; // 最近一次下落的坐标
			already_bottom = 2 * r; // 最后一次到底部的纵坐标 和上面的不一样
		}
		else {
			lie = (j * (3 + grid) - 1 + 3 * grid) * 2;
			hang = (3 + grid) * r - grid;
			already_up = (3 + grid) * r - grid; // 最近一次下落的坐标
			already_bottom = (3 + grid) * r - grid; // 最后一次到底部的纵坐标 和上面的不一样
			int color[6] = { -1, 6, 7, 8, 9, 10 };
		}

		cct_gotoxy(lie, hang); //到达底部往上遍历
		for (int i = r; i > 0; i--) {
			
			int helper_star = 0;
			if (h[i][j] == 1) {
				int helper_star = 0;
				step = 0;
				for (w = i; w > 0; w--) {

					if (h[w][j] == 1) {
						step++;
					}
					else {
						break;
					}
				}
				// 此时w是下移的彩球序号
				if (mode == 1) {
					hang = w * 2 + 1;
					already_up = 2 * w;
				}
				else {
					hang = (3 + grid) * w - grid +1;
					already_up = (3 + grid) * w - grid;
				}

				for (int step_i = 1; step_i < 10; step_i++) {
					if (hang < already_bottom && hang > 1) { // flag
						if (mode == 1)
							lan(s, j, w, lie, hang, helper_down);
						else if(!helper_star) {
							helper_star = 1;
							drop(s, w, j, grid, 1, step);
						}

						if (mode == 1)
							hang += 2;
						else
							hang += (3 + grid);
						helper_down = 1;
					}
					else {
						cct_getxy(x, y);
						already_bottom = min(y, already_bottom);
						break;
					}
				}
			}
			else if (helper_down) {
				if (mode == 1) {
					hang = i * 2 + 1;
					already_up = i * 2;
				}
				else {
					hang = i * (3+grid) + 1;
					already_up = i * (3+grid);
				}
				for (int step_i = 1; step_i < 10; step_i++) {
					if (hang < already_bottom && hang > 1) {
						if (mode == 1)
							lan(s, j, i, lie, hang, helper_down);
						else if (!helper_star) {
							helper_star = 1;
							drop(s, i, j, grid, 1, step);

						}
						if (mode == 1)
							hang += 2;
						else
							hang += 3 + grid;
					}
					else {
						cct_getxy(x, y);
						already_bottom = min(y, already_bottom);
						break;
					}
				}
			}
			else {
				if (mode == 1)
					already_bottom -= 2;
				else
					already_bottom -= (3+grid);

			}
			if (mode == 1)
				i = min(i, already_up / 2);
			else
				i = min(i, already_up / (3 + grid));
			
		}
	}
	if (mode == 1)
		clear_instrcution(r);
}


void location_2(int (*s)[11], int r, int c, int grid, int mode)
{
	int first = 0, prompt = 0;
	// 箭头键/鼠标移动，回车键/单击左键选择, Q/单击右键结束
	// 本次合成结束， 按C/单击左键继续新一次合成；
	// 箭头键/鼠标移动取消当前选择，回车键/单击左键合成                                                                                    
	// 本次得分:125 总分:125
	// 周围无相同值，箭头键/鼠标移动，回车键/单击左键选择, Q/单击右键结束
	int x = 6, y = 4, X = 0, Y = 0;
	int ret, maction, keycode1, keycode2;
	int loop = 1;
	cct_enable_mouse();

	int sel = 0;
	int inval = 0;
	int total = 0;
	int rest = r * c;
	int h[11][11] = { -12 };
	int sx = 0, sy = 0;
	while (loop) {

		int num = 0;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		cct_setcursor(CURSOR_INVISIBLE);
		if (ret == CCT_MOUSE_EVENT) {
			if (sx == X && sy == Y) {

			}
			else {
				//恢复
				for (int i = 1; i <= r; i++) {
					for (int j = 1; j <= c; j++) {
						// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
						if (h[i][j]) {
							print_star(s, i, j, grid, 3);
						}
					}
				}
				in_h(h);
			}
			sx = X;
			sy = Y;



			int check = 1;
			if (grid) {
				if (Y > 2 && Y < 4 + (r - 1) * (3 + grid) + 2 && X>2 && X < 6 + (c - 1) * (6 + 2 * grid) + 4) {
					for (int i = 0; i < c - 1; i++)
						if (X == 6 + i * (6 + 2 * grid) + 4 || X == 6 + i * (6 + 2 * grid) + 5)
							check = 0;
					for (int i = 0; i < r - 1; i++)
						if (Y == 4 + i * (3 + grid) + 2)
							check = 0;
				}
				else
					check = 0;
			}
			if (check) {
				if (Y > 2 && Y < 4 + (r - 1) * (3 + grid) + 2
					&& X >2 && X < 6 + (c - 1) * (6 + 2 * grid) + 4) {
					int x15 = 6 + ((X - 4) / (sw + 2 * grid)) * (6 + 2 * grid);
					int y15 = 4 + ((Y - 3) / (grid + sh)) * (3 + grid);
					n_star(x, y, s[((y - 3) / (sh + grid)) + 1][(x - 4) / (sw + 2 * grid) + 1], 0);
					n_star(x15, y15, s[((Y - 3) / (grid + sh)) + 1][(X - 4) / (2 * grid + sw) + 1], 15);
					// r2 = (Y - 3) / (grid + sh) + 1
					if ((x != x15 || y != y15) && !first) {
						if (mode == 2) {
							//恢复
							for (int i = 1; i <= r; i++) {
								for (int j = 1; j <= c; j++) {
									// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
									if (h[i][j]) {
										print_star(s, i, j, grid, 3);
									}
								}
							}
							in_h(h);
						}
						n_star(x, y, s[((y - 3) / (grid + sh)) + 1][(x - 4) / (2 * grid + sw) + 1], 0);

					}
					if (first)
						first = false;
					if (inval && (x != x15 || y != y15)) {
						inval = 0;
						cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
						cct_setcolor(default_bg);
					}
					if (sel && (x != x15 || y != y15)) {
						sel = 0;
						cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
						cct_setcolor(default_bg);
					}

					x = x15;
					y = y15;

					if (!inval && !sel) {
						cct_gotoxy(0, (3 + grid) * r + (4 - grid));
						cout << "[当前鼠标] " << (char)('A' + (Y - 3) / (grid + sh)) << "行" << (X - 4) / (2 * grid + sw) << "列" << "                                                      ";
					}


					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));

					if (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_RIGHT_BUTTON_CLICK) {
						if (mode == 2) {
							int ir = (y - 3) / (grid + sh) + 1;
							int ic = (x - 4) / (2 * grid + sw) + 1;
							in_h(h);
							if (check_2(s, r, c, h, ir, ic, 1)) {
								if (!sel && !inval) {
									// 集体变色 // 如何消失？
									for (int i = 1; i <= r; i++) {
										for (int j = 1; j <= c; j++) {
											// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
											if (h[i][j]) {
												print_star(s, i, j, grid, 1);
											}
										}
									}
									cct_gotoxy(0, (3 + grid) * r + (4 - grid));
									//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
									cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
									sel = 1;
								}

								else if (sel && !inval) {
									//消失
									for (int i = 1; i <= r; i++) {
										for (int j = 1; j <= c; j++) {
											if (h[i][j]) {
												print_star(s, i, j, grid, 2);
											}
										}
									}

									sel = 0;
									score(r, c, h, &num, &total, &rest);
									cct_gotoxy(0, 0);
									cout << "                                                              ";
									cct_gotoxy(0, 0);
									cout << "本次得分：" << num << " 总得分：" << total;

									//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
									cct_gotoxy(0, (3 + grid) * r + (4 - grid));
									cout << "合成完成，回车键/单击左键下落0" << "                                                              ";
									// flag
									while (1) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

										if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
											break;
										else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
											break;
									}
									// 下落
									// 先逐行
									remove_0(s, r, c, h, 1);
									falling_down(s, r, c, h, 2, grid);
									// 再逐列
									remove_0(s, r, c, h, 0, 's');
									drop(s, r, c, grid, 2);
									remove_0(s, r, c, h, 2);

									//in_h(h);

									//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
									cct_gotoxy(0, (3 + grid) * r + (4 - grid));
									cout << "本次合成结束， 按C/单击左键继续新一次合成；" << "                                                              ";
									while (1) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
										if (ret == CCT_MOUSE_EVENT)
											break;
										else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 'c' || keycode1 == 'C')
											break;
									}
									if (!grid)
										loop = 0;
								}
								//break;
							}

							else {
								cout << "周围无相同值，箭头键/鼠标移动，回车键/单击左键选择, Q/单击右键结束";
								inval = true;
							}
						}
						else {
							cout << "选中了" << (char)('A' + (y - 3) / (grid + sh)) << "行" << (x - 4) / (2 * grid + sw) << "列" << "                                                      ";
							loop = 0;
						}
					}
				}

			}

			else {
				n_star(x, y, s[((y - 3) / (sh + grid)) + 1][(x - 4) / (sw + 2 * grid) + 1], 0);
				if (!first) {
					n_star(x, y, s[((y - 3) / (sh + grid)) + 1][(x - 4) / (sw + 2 * grid) + 1], 0);
					first = true;
				}
				if (inval) {
					inval = 0;
					first = 1;
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
				}
				if (sel) {
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					sel = 0;
					first = 1;
				}
				cct_gotoxy(0, (3 + grid) * r + (4 - grid));
				// 	cct_gotoxy(0, 4 * r + 3);
				// 	cct_gotoxy(0, 3 * r + 4);
				cout << "[当前鼠标] 位置非法" << "                                                      ";
			}
		}


		else if (ret == CCT_KEYBOARD_EVENT) {
			if ((keycode1 == 224 &&
				(keycode2 == KB_ARROW_UP || keycode2 == KB_ARROW_DOWN || keycode2 == KB_ARROW_LEFT || keycode2 == KB_ARROW_RIGHT))
				|| keycode1 == 13 || keycode1 == 'q' || keycode1 == 'Q') {
				cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
				cct_setcolor(default_bg);
			}

			//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
			cct_gotoxy(0, (3 + grid) * r + (4 - grid));
			//
			//
			//

			switch (keycode1) {
				case 224:
					n_star(x, y, s[((y - 3) / (grid + sh)) + 1][(x - 4) / (2 * grid + sw) + 1], 0);
					//恢复
					for (int i = 1; i <= r; i++) {
						for (int j = 1; j <= c; j++) {
							// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
							if (h[i][j]) {
								print_star(s, i, j, grid, 3);
							}
						}
					}
					in_h(h);
					sel = 0;

					switch (keycode2) {
						case KB_ARROW_UP:

							if (y == 4)
								y = 4 + (r - 1) * (3 + grid);
							else
								y -= sh + grid;

							break;
						case KB_ARROW_DOWN:
							if (y == 4 + (r - 1) * (3 + grid))
								y = 4;
							else
								y += sh + grid;
							break;
						case KB_ARROW_LEFT:
							if (x == 6)
								x = 6 + (c - 1) * (6 + 2 * grid);
							else
								x -= sw + 2 * grid;

							break;
						case KB_ARROW_RIGHT:
							if (x == 6 + (c - 1) * (6 + 2 * grid))
								x = 6;
							else
								x += sw + 2 * grid;

							break;
					}
					n_star(x, y, s[((y - 3) / (grid + sh)) + 1][(x - 4) / (2 * grid + sw) + 1], 15);
					//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					cout << "[当前键盘]" << (char)('A' + (y - 3) / (grid + sh)) << "行" << (x - 4) / (2 * grid + sw) << "列" << "                                                      ";
					break;


				case 13:
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					if (mode == 2) {
						int ir = (y - 3) / (grid + sh) + 1;
						int ic = (x - 4) / (2 * grid + sw) + 1;
						in_h(h);
						if (check_2(s, r, c, h, ir, ic, 1)) {
							if (!sel) {
								// 集体变色 // 如何消失？
								for (int i = 1; i <= r; i++) {
									for (int j = 1; j <= c; j++) {
										// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
										if (h[i][j]) {
											print_star(s, i, j, grid, 1);
										}
									}
								}
								//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
								cct_gotoxy(0, (3 + grid) * r + (4 - grid));
								cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
								sel = 1;
							}
							else if (sel) {
								//消失
								for (int i = 1; i <= r; i++) {
									for (int j = 1; j <= c; j++) {
										if (h[i][j]) {
											print_star(s, i, j, grid, 2);
										}
									}
								}

								sel = 0;
								score(r, c, h, &num, &total, &rest);
								cct_gotoxy(0, 0);
								cout << "                                                              ";
								cct_gotoxy(0, 0);
								cout << "本次得分：" << num << " 总得分：" << total;

								cct_gotoxy(0, (3 + grid) * r + (4 - grid));
								cout << "合成完成，回车键/单击左键下落0" << "                                                              ";
								// flag
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

									if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
										break;
								}
								// 下落
								// 先逐行
								remove_0(s, r, c, h, 1);
								falling_down(s, r, c, h, 2, grid);
								// 再逐列
								remove_0(s, r, c, h, 0, 's');
								drop(s, r, c, grid, 2);
								remove_0(s, r, c, h, 2);

								//in_h(h);

								cct_gotoxy(0, (3 + grid) * r + (4 - grid));
								cout << "本次合成结束， 按C/单击左键继续新一次合成；" << "                                                              ";
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_MOUSE_EVENT)
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 'c' || keycode1 == 'C')
										break;
								}
								if (!grid)
									loop = 0;
							}
							break;
						}

						else {
							cout << "周围无相同值，箭头键/鼠标移动，回车键/单击左键选择, Q/单击右键结束";
							inval = true;
						}
					}

					else {
						cout << "选中了" << (char)('A' + (y - 3) / (grid + sh)) << "行" << (x - 4) / (2 * grid + sw) << "列" << "                                                      ";
						loop = 0;
					}
					break;

				case 'q':
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					loop = 0;
					break;


				case'Q':
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					loop = 0;
					break;
			}
		}
	}

	cct_disable_mouse();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
}

void score(int r, int c, int (*h)[11], int* num, int* total, int* rest, int mode)
{
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (h[i][j] == 1)
				(*num)++;
		}
	}
	*rest -= *num;
	*num = (*num) * (*num) * 5;
	if (*rest < 10 && mode == 0) {
		*total = *num + 180 * (10 - *rest);
	}
	else
		*total += *num;
}

void in_h(int (*h)[11])
{
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			h[i][j] = 0;
		}
	}
}


