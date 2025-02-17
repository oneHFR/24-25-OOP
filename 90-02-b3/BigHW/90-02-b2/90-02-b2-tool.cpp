/* 2152131 计科 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <time.h>
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
/* 输入继续prompt */
void To_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi)
{
	if (bgi->inited == CGI_INITED) { //初始化过
		int l = strlen(prompt) + strlen("，按回车键继续...   ");
		cct_setcolor(COLOR_HYELLOW, COLOR_BLACK); //恢复初始颜色
		//pCGI->SLI.top_start_x = (pCGI->cols - strlen(temp)) / 2;
		cct_gotoxy(0, bgi->SLI.lower_start_y + 1);//光标设到指定位置
		cout << setw(bgi->cols - 1) << ' '; //先用空格清空整行

		cct_gotoxy((bgi->cols - l) / 2, bgi->SLI.lower_start_y + 1);//光标设到指定位置
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
/* 输入基本参数 */
void input_parameter(CONSOLE_GRAPHICS_INFO* const pCGI, int *t)
{
	int x0 = 3;
	int y0 = 3;
	int x, y;
	cct_gotoxy(x0, y0);
	cout << "游戏2048参数设置：" << endl;

	cct_gotoxy(x0, y0+1);
	cout << "请输入行数[4-6]";
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->row_num;
		if (pCGI->row_num == 13 || !cin.good() || pCGI->row_num < 4 || 6 < pCGI->row_num) {
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
	if (pCGI->col_num * pCGI->row_num >= 20)
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
			pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS / 5 + 5 * pCGI->delay_of_block_moved;
			break;
		}
	}

}
/* 绘制新增的色块 */
int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi)
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
				/*
				  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
				   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
				   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
				   const int bdi_value						：需要显示的值
				   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
				*/
				gmw_draw_block(pCGI, r_i, r_j, s[r_i][r_j], bdi);
				break;
			}
		}
		return 1;
	}
}
/* 初始化基本设置 */
void init(CONSOLE_GRAPHICS_INFO* const pCGI, int mode)
{
	if (mode == 1) {
		gmw_init(pCGI);
		gmw_set_font(pCGI, "新宋体", 16);
		gmw_set_frame_style(pCGI, 8, 2, false);
		gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
		gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);
	}

	else if (mode == 2) {
		gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);			//整个窗口颜色
		gmw_set_font(pCGI, "新宋体", 16);						//字体
		gmw_set_frame_style(pCGI, 10, 5, true);					//游戏主区域风格：每个色块宽10高5，有分隔线【数字色块带边框，宽度为10(放最多6位数字)，高度为5(为了保持色块为方形)】
		gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
		gmw_set_block_border_switch(pCGI, true);				//小色块带边框
		gmw_set_ext_rowcol(pCGI, 0, 0, 4, 4);
	}
}
/* 移动和合并效果 */
 void move_and_merge_blocks(CONSOLE_GRAPHICS_INFO* const pCGI, int s[][MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int direction, int* check_change)
{
	int distance; // 记录方块可以移动的距离

	if (direction == KB_ARROW_UP) {  // 上移
		for (int j = 0; j < pCGI->col_num; j++) {
			distance = 0;
			for (int i = 0; i < pCGI->row_num; i++) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// 移动方块
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, DOWN_TO_UP, distance);
					s[i - distance][j] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// 合并相邻相同的方块
					if (i - distance > 0 && s[i - distance][j] == s[i - distance - 1][j]) {
						s[i - distance - 1][j] *= 2;
						s[i - distance][j] = 0;
						gmw_draw_block(pCGI, i - distance, j, s[i - distance][j], bdi);
						gmw_draw_block(pCGI, i - distance - 1, j, s[i - distance - 1][j], bdi);
						distance++; // 合并后增加距离
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
	else if (direction == KB_ARROW_DOWN) {  // 下移
		for (int j = 0; j < pCGI->col_num; j++) {
			distance = 0;
			for (int i = pCGI->row_num - 1; i >= 0; i--) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// 移动方块
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, UP_TO_DOWN, distance);
					s[i + distance][j] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// 合并相邻相同的方块
					if (i + distance < pCGI->row_num - 1 && s[i + distance][j] == s[i + distance + 1][j]) {
						s[i + distance + 1][j] *= 2;
						s[i + distance][j] = 0;
						gmw_draw_block(pCGI, i + distance, j, s[i + distance][j], bdi);
						gmw_draw_block(pCGI, i + distance + 1, j, s[i + distance + 1][j], bdi);
						distance++;
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
	else if (direction == KB_ARROW_LEFT) {  // 左移
		for (int i = 0; i < pCGI->row_num; i++) {
			distance = 0;
			for (int j = 0; j < pCGI->col_num; j++) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// 移动方块
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, RIGHT_TO_LEFT, distance);
					s[i][j - distance] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// 合并相邻相同的方块
					if (j - distance > 0 && s[i][j - distance] == s[i][j - distance - 1]) {
						s[i][j - distance - 1] *= 2;
						s[i][j - distance] = 0;
						gmw_draw_block(pCGI, i, j - distance, s[i][j - distance], bdi);
						gmw_draw_block(pCGI, i, j - distance - 1, s[i][j - distance - 1], bdi);
						distance++;
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
	else if (direction == KB_ARROW_RIGHT) {  // 右移
		for (int i = 0; i < pCGI->row_num; i++) {
			distance = 0;
			for (int j = pCGI->col_num - 1; j >= 0; j--) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// 移动方块
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, LEFT_TO_RIGHT, distance);
					s[i][j + distance] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// 合并相邻相同的方块
					if (j + distance < pCGI->col_num - 1 && s[i][j + distance] == s[i][j + distance + 1]) {
						s[i][j + distance + 1] *= 2;
						s[i][j + distance] = 0;
						gmw_draw_block(pCGI, i, j + distance, s[i][j + distance], bdi);
						gmw_draw_block(pCGI, i, j + distance + 1, s[i][j + distance + 1], bdi);
						distance++;
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
}
/* 检查游戏是否成为死局*/
int check(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	// 四周
	int d_x[4] = { 1, -1, 0, 0 };
	int d_y[4] = { 0, 0, 1, -1 };
	int num = 0; // 非空数

	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			if (s[i][j] != 0)
				num++;

			// 检查四周
			for (int k = 0; k < 4; k++) {
				int s_i = i + d_x[k];
				int s_j = j + d_y[k];

				// 检查边界
				if (s_i >= 0 && s_i < pCGI->row_num && s_j >= 0 && s_j < pCGI->col_num) {
					//没有死局
					if (s[i][j] != 0 && s[i][j] == s[s_i][s_j])
						return 1;
				}
			}
		}
	}

	// 死局
	if (num == pCGI->row_num * pCGI->col_num)
		return 0;
	else
		return 1;
}
/* 游戏模式 */
void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t)
{
	int loop = 1;
	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;
	int score = 0;
	char temp[100] = { 0 };
	time_t start_time = clock();
	sprintf(temp, "目标:%d 分数:%d 时间:%d （R:重玩 Q:退出）", t, score, int(clock() - start_time) / CLOCKS_PER_SEC);
	pCGI->SLI.top_start_y = 0;
	pCGI->SLI.top_start_x = (pCGI->cols - strlen(temp)) / 2;
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

	while (loop) {
		int distance = 0;
		int check_change = 0;
		cct_disable_mouse();

		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);
		switch (keycode1) {
			case 'q':
			case 'Q':
				exit(0);
			case 'r':
			case 'R':
				return ;
			case 0xE0:
				// 根据不同的方向调用通用函数处理
				switch (keycode2) {
					case KB_ARROW_UP:
						move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_UP, &check_change);
						break;

					case KB_ARROW_DOWN:
						move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_DOWN, &check_change);
						break;

					case KB_ARROW_LEFT:
						move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_LEFT, &check_change);
						break;

					case KB_ARROW_RIGHT:
						move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_RIGHT, &check_change);
						break;
				}

				//else if (keycode1 == 'Q' || keycode1 == 'q')
				//	exit(0);
				//else if (keycode1 == 'R' || keycode1 == 'r')
				//	return ;

				// 如果有方块移动，生成新的随机方块

		}
		if (check_change) {
			//pCGI->SLI.top_start_x = (pCGI->CFI.bwidth + (pCGI->extern_right_cols + pCGI->extern_left_cols) * 2 - strlen(temp)) / 8;
			sprintf(temp, "目标:%d 分数:%d 时间:%d （R:重玩 Q:退出）", t, score, int(clock() - start_time) / CLOCKS_PER_SEC);
			pCGI->SLI.top_start_x = (pCGI->cols - strlen(temp)) / 2;
			gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
			new_one(pCGI, s, bdi);
			// 更新状态栏显示最大值
			for (int i = 0; i < pCGI->row_num; i++) {
				for (int j = 0; j < pCGI->col_num; j++) {
					score = s[i][j] > score ? s[i][j] : score;
				}
			}
		}

		loop = check(pCGI, s);
	}
	if (!loop) {
		sprintf(temp, "游戏失败！ 最终分数:%d 时间:%d", score, int(clock() - start_time) / CLOCKS_PER_SEC);
		// pCGI->SLI.top_start_x = (pCGI->CFI.bwidth + (pCGI->extern_right_cols + pCGI->extern_left_cols) * 2 - strlen(temp)) / 8;
		pCGI->SLI.top_start_x = (pCGI->cols - strlen(temp)) / 2;
		gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
	}
	else {
		sprintf(temp, "目标实现！ 最终分数:%d 时间:%d",  score, int(clock() - start_time) / CLOCKS_PER_SEC);
		pCGI->SLI.top_start_x = (pCGI->cols - strlen(temp)) / 2;
		// pCGI->SLI.top_start_x = (pCGI->CFI.bwidth + (pCGI->extern_right_cols + pCGI->extern_left_cols) * 2 - strlen(temp)) / 8;
		gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
	}
}






