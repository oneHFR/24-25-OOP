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

	while (1) {
		int s[MAX_ROW][MAX_COL] = { 0 };
		int maxscore = 0, t = 0;
		cct_setcolor();
		cct_cls();
		init(&G2048_CGI, 1);
		gmw_draw_frame(&G2048_CGI);

		input_parameter(&G2048_CGI, &t);
		//G2048_CGI.row_num = 5;
		//G2048_CGI.col_num = 4;
		//G2048_CGI.delay_of_block_moved = 0;
		//G2048_CGI.delay_of_block_moved = BLOCK_MOVED_DELAY_MS + 5 * G2048_CGI.delay_of_block_moved;
		/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
		init(&G2048_CGI, 2);
		gmw_draw_frame(&G2048_CGI);

		new_one(&G2048_CGI, s, bdi);
		new_one(&G2048_CGI, s, bdi);

		game(&G2048_CGI, s, bdi, t);

		To_be_continued("游戏重开", &G2048_CGI);
	}
	return 0;
}