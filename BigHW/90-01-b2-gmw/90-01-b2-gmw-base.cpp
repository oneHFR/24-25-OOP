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
#include "90-01-b2-gmw.h"

using namespace std;

void h9(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
    int h[MAX_ROW][MAX_COL] = { 0 };
    /* 从 test中copy过来的 */
	init(pCGI, s);
	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(pCGI, pCGI->row_num, pCGI->col_num);
	/* 显示框架 */
	gmw_draw_frame(pCGI);
	fill(pCGI, s, 9);
	/* 上状态栏显示内容 */
	char temp[256];
	sprintf(temp, "窗口大小：%d行 %d列",  pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
    /* 检查初始可以消除的对象 */
	int helper_9 = check(pCGI, s, pCGI->row_num, pCGI->col_num, h);
    if (helper_9) {
        while (helper_9) {
            remove(pCGI, s, pCGI->row_num, pCGI->col_num, h);
            remove_zero(s, pCGI->row_num, pCGI->col_num, h); //把零取消 为填充提供识别项
            fall(pCGI, s, pCGI->row_num, pCGI->col_num); // 下落动画
            fill_new(pCGI, s, pCGI->row_num, pCGI->col_num, 9);
            ini_h(h);
            helper_9 = check(pCGI, s, pCGI->row_num, pCGI->col_num, h);
        }
    }

    /* 显示提示板块进入游戏模式 */
    int helper_hint = hint(pCGI, s, pCGI->row_num, pCGI->col_num, h);
    if (!helper_9 && !helper_hint) {
        gmw_status_line(pCGI, LOWER_STATUS_LINE, "（无可消除项，游戏结束!）");
        return ;
    }
    else 
        game(pCGI, s);
	return ;
}