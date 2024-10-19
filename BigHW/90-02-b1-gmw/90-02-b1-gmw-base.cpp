/* 2152131 计科 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "../include/common_menu.h"
//#include "../include/common_functions.h"
#include "../include/cmd_gmw_tools.h"
#include "90-02-b1-gmw.h"

using namespace std;

int H7(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL])
{
	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，这里是有行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	init(pCGI, s);

	/* 上状态栏显示内容 */
	char temp[256];
	sprintf(temp, "窗口大小：%d行 %d列", pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
	cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y +1);
	cout << "[Q 退出][右键 退出]/[R 重开]";
	//void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t);
	
	int next = Game(pCGI, s);
	if (next == 1)
		H7(pCGI, s);
	return 0;
}