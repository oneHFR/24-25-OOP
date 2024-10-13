/* 2152131 计科 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "../include/common_menu.h"
#include "../include/common_functions.h"
#include "../include/cmd_gmw_tools.h"
#include "90-02-b2.h"

using namespace std;

// 2048 
/*
目标:2048 分数:4 最高:1356 时间:30 (R:重玩 Q:退出)
先打印canvas
游戏2048参数设置：
请输入行数[4-8] 4
请输入列数[4-10] 8
请输入目标分数[2048/4096/8192/16384] 2048
请输入动画延时[0-5] 0
*/
int main()
{
	srand((unsigned int)time(0));

	CONSOLE_GRAPHICS_INFO G2048_CGI;

	const BLOCK_DISPLAY_INFO bdi[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  // 0空格
	{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{4, COLOR_HRED, COLOR_BLACK, NULL},
	{8, COLOR_GREEN, COLOR_BLACK, NULL},
	{16, COLOR_RED, COLOR_BLACK, NULL},
	{32, COLOR_HGREEN, COLOR_BLACK, NULL},
	{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{128, COLOR_HRED, COLOR_BLACK, NULL},
	{256, COLOR_HWHITE, COLOR_BLACK, NULL},
	{512, COLOR_HBLACK, COLOR_BLACK, NULL},
	{1024, COLOR_HPINK, COLOR_BLACK, NULL},
	{2048, COLOR_WHITE, COLOR_BLACK, NULL},
	{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
	{BDI_VALUE_END, -1, -1, NULL} // NULL
	};

	while (1) {
		int s[MAX_ROW][MAX_COL] = { 0 };
		int maxscore = 0, t = 512;
		cct_setcolor();
		cct_cls();
		init(&G2048_CGI, 1);
		gmw_draw_frame(&G2048_CGI);

		//input_parameter(&G2048_CGI, &t);
		G2048_CGI.row_num = 5;
		G2048_CGI.col_num = 4;
		G2048_CGI.delay_of_block_moved = 0;
		G2048_CGI.delay_of_block_moved = BLOCK_MOVED_DELAY_MS + 5 * G2048_CGI.delay_of_block_moved;
		/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
		init(&G2048_CGI, 2);
		gmw_draw_frame(&G2048_CGI);

		new_one(&G2048_CGI, s, bdi);
		new_one(&G2048_CGI, s, bdi);

		game(&G2048_CGI, s, bdi, t);

		int x, y;
		cct_getxy(x, y);
		while (1) {
			char keepon[1024] = { 0 };
			cin.getline(keepon, 1024);
			for (int i = 0; keepon[i] != 0; i++) {
				if (keepon[i] <= 'z' && keepon[i] >= 'a') {
					keepon[i] = keepon[i] - ('a' - 'A');
				}
			}
			if (!strcmp(keepon, "END")) {
				break;
			}
			else {
				cct_gotoxy(0, y + 1);
				cout << "输入错误，请重新输入";
				cct_gotoxy(x, y);
				cout << setw(50) << " ";
				cct_gotoxy(x, y);
			}
		}

	}


	return 0;
}