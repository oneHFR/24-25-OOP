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

/*
目标:2048 分数:4 最高:1356 时间:30 (R:重玩 Q:退出)
先打印canvas
游戏2048参数设置：
请输入行数[4-8] 4
请输入列数[4-10] 8
请输入目标分数[2048/4096/8192/16384] 2048
请输入动画延时[0-5] 0
*/

void input_parameter(CONSOLE_GRAPHICS_INFO* const pCGI, int *t)
{
	int x0 = 3;
	int y0 = 3;
	int x, y;
	cct_gotoxy(x0, y0);
	cout << "游戏2048参数设置：" << endl;

	cct_gotoxy(x0, y0+1);
	cout << "请输入行数[4-8]";
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->row_num;
		if (pCGI->row_num == 13 || !cin.good() || pCGI->row_num < 4 || 8 < pCGI->row_num) {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
		else
			break;
	}

	cct_gotoxy(x0, y0 + 2);
	cout << "请输入列数[4-10]"; // 这里可以宏定义一下但是我懒。。
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->col_num;
		if (!cin.good() || pCGI->col_num < 4 && 10 < pCGI->col_num) {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
		else
			break;
	}

	int min_t;
	if (pCGI->col_num * pCGI->row_num <= 20)
		min_t = 512;
	if (pCGI->col_num * pCGI->row_num >= 25)
		min_t *= 2;
	if (pCGI->col_num * pCGI->row_num >= 36)
		min_t *= 2;
	if (pCGI->col_num * pCGI->row_num >= 49)
		min_t *= 2;
	if (pCGI->col_num * pCGI->row_num >= 64)
		min_t *= 2;
	// 写不下去了hh
	cct_gotoxy(x0, y0 + 3);
	cout << "请输入目标分数[" <<min_t << '/' << 2 * min_t << '/' << 4 * min_t << '/' << 8 * min_t << "]:";
	cct_getxy(x, y);
	while (1) {
		cin >> *t;
		if (cin.good() && *t == min_t || *t == 2 * min_t || *t == 4 * min_t || *t == 8 * min_t)
			break;
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
	}

	cct_gotoxy(x0, y0 + 4);
	cout << "请输入动画延迟[0-5]";
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->delay_of_block_moved;
		if (!cin.good() || 0 > pCGI->delay_of_block_moved && pCGI->delay_of_block_moved > 5) {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
		else {
			/* 定义延时的种类
			#define DELAY_OF_DRAW_FRAME		0	//画游戏主框架时的延时
			#define DELAY_OF_DRAW_BLOCK		1	//画色块时的延时
			#define DELAY_OF_BLOCK_MOVED		2	//色块移动时的延时

			#define BLOCK_MOVED_DELAY_MS		15	//色块移动的缺省延时，单位ms（其余两个缺省为0）
			*/
			pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS + 5 * pCGI->delay_of_block_moved;
			break;
		}
	}

}

int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	int already = 0;
	for (int i = 0; i < pCGI->row_num; i++)
		for (int j = 0; j < pCGI->col_num; j++)
			if (s[i][j] != 0)
				already++;
	if (already == pCGI->row_num * pCGI->col_num)//全满
		return 0;
	else {
		while (1){
			int r_i = rand() % pCGI->row_num;
			int r_j = rand() % pCGI->col_num;
			if (!s[r_i][r_j]) {
				s[r_i][r_j] = 2;
				break;
			}
		}
		return 1;
	}
}

void init(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/*
		  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
			   const int row					：游戏区域色块行数（缺省10）
			   const int col					：游戏区域色块列数（缺省10）
			   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
			   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
	  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，
	  无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
		int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
	*/

	/*
	int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{

	pCGI->inited = CGI_INITED;

	gmw_set_rowcol(pCGI, row, col);
	gmw_set_font(pCGI);
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS);
	gmw_set_ext_rowcol(pCGI);

	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI);

	gmw_set_block_default_linetype(pCGI, 1);
	gmw_set_block_border_switch(pCGI);


	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);

	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);


	pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;

	// 	FLAG
	return 0; //此句可根据需要修改
}
	*/

	gmw_init(pCGI);
	gmw_set_font(pCGI, "新宋体", 16);
	gmw_set_frame_style(pCGI, 8, 2, false);
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);
	gmw_draw_frame(pCGI);
	gmw_set_frame_style(pCGI, 10, 5, true);

	//gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);
	//gmw_set_delay(&G2048_CGI, 1, 0);
	//gmw_set_delay(&G2048_CGI, 2, 0);
	//gmw_set_frame_style(&G2048_CGI, 3, 2, false);
	//gmw_set_rowcol(&G2048_CGI, r, c);
	//gmw_set_frame_style(&G2048_CGI, 10, 5, 1);
	//gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);
	//gmw_set_status_line_switch(&G2048_CGI, 0, 1);
	//gmw_set_status_line_switch(&G2048_CGI, 1, 0);
	
	/* 定义色块显示时，内部数组的int值与界面上显示图形的对应关系
   注：此结构体数组的值在测试用例中定义，随着游戏的不同而不同 
	typedef struct _block_display_info_ {
		const int   value;	//要显示的内部数组值
		const int   bgcolor;	//-1表示用游戏区域背景色
		const int   fgcolor;	//-1表示用游戏区域前景色
		const char* content;	//内部数组值对应的图形（如果为NULL，则直接显示内部数组的值）
	} BLOCK_DISPLAY_INFO;*/


}

static void to_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi)
{
	if (bgi->inited == CGI_INITED) { //初始化过
		cct_setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //恢复初始颜色

		cct_gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
		cout << setw(bgi->cols - 1) << ' '; //先用空格清空整行

		cct_gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
	}
	else { //未初始化过
		cct_setcolor(); //缺省颜色
		cct_gotoxy(0, 0);//光标设到指定位置
	}

	if (prompt)
		cout << prompt << "，按回车键继续...   ";
	else
		cout << "按回车键继续...   ";

	while (_getch() != '\r')
		;

	return;
}

