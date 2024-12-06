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
/* 具体函数功能注释详见头文件 */
void input(CONSOLE_GRAPHICS_INFO* const pCGI, int min, int max)
{
	while (1) {
		cout << "请输入行数(" << min << "-" << max << ")：" << endl;
		cin >> pCGI->row_num;
		if (pCGI->row_num >= min && pCGI->row_num <= max && cin.good()) {
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
		cin >> pCGI->col_num;
		if (pCGI->col_num >= min && pCGI->col_num <= max && cin.good()) {
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

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL])
{
	pCGI->extern_right_cols = 3;
	//pCGI->extern_up_lines = 3;
	//pCGI->extern_left_cols = 3;
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_HWHITE);			//整个窗口颜色
	gmw_set_font(pCGI, "新宋体", 24);							//字体
	//	gmw_set_frame_style(pCGI);									//游戏主区域风格：每个色块宽2高1，有分隔线
	gmw_set_frame_default_linetype(pCGI, 4);					//游戏主区域线型：横单竖双
	gmw_set_frame_color(pCGI, COLOR_HWHITE, COLOR_BLACK);		//游戏主区域颜色
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);	//不需要上状态栏
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);	//需要下状态栏
}

void fill(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int num)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			s[i][j] = rand() % num + 1;
			gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
		}
	}
}

void fill_new(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int num)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (s[i][j] == 0) {
				s[i][j] = rand() % num + 1;
				gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
			}
		}
	}
}

void ini_h(int (*h)[MAX_COL])
{
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			h[i][j] = 0;
		}
	}
}

int check(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL])
{
	int helper = 0;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (s[i][j] == s[i][j + 1] && s[i][j] == s[i][j + 2] && s[i][j] != -1) {
				h[i][j] = h[i][j + 1] = h[i][j + 2] = 1;
				helper = 1;
			}
		}
	}
	for (int j = 0; j < c; j++) {
		for (int i = 0; i < r; i++) {
			if (s[i][j] == s[i + 1][j] && s[i][j] == s[i + 2][j] && s[i][j] != -1) {
				h[i][j] = h[i + 1][j] = h[i + 2][j] = 1;
				helper = 1;
			}
		}
	}
	if (helper == 0)
		gmw_status_line(pCGI, TOP_STATUS_LINE, "初始已无可消除项");
	else
		gmw_status_line(pCGI, TOP_STATUS_LINE, "初始可消除项（不同色标识）：");
	return helper;
}

void remove(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]) 
{
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (h[i][j] == 1) {
				gmw_draw_block(pCGI, i, j, s[i][j], bdi_exploded);
				Sleep(50);
				gmw_draw_block(pCGI, i, j, 0, bdi_exploded); //0是空白
				Sleep(50);
			}
		}
	}
}

void remove_zero(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL])
{
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (h[i][j] == 1)
				s[i][j] = 0;
		}
	}
}

void fall(const CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c)
{
	for (int j = 0; j < c; ++j) {
		int step = 0;
		for (int i = r - 1; i >= 0; i--) {
			if (s[i][j] != 0) {
				if (step) {
					gmw_move_block(pCGI, i, j, s[i][j], 0, bdi_normal, UP_TO_DOWN, step);
					s[i + step][j] = s[i][j];
					s[i][j] = 0;
				}
			}
			else
				step++;
		}
	}
}

int hint(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL])
{
	int ts = 0;
	int helper_hint = 0;
	int ds[20][20] = { 0 };
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			ds[i][j] = 0;
		}
	}
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			ds[i + 1][j + 1] = s[i][j];
			h[i][j] = 0;
		}
	}

	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
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

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (h[i][j] == 1) {
				gmw_draw_block(pCGI, i, j, s[i][j], bdi_prompt);
			}
			else
				gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
		}
	}

	return helper_hint;
}

int game_score(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int* SCORE)
{
	int check_swap = 0;
	ini_h(h);

	check(pCGI, s, pCGI->row_num, pCGI->col_num, h);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (h[i][j] == 1) {
				check_swap = 1;
				(*SCORE)++;
			}
		}
	}
	return check_swap;
}

int game_swap(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int x31, int y31, int x32, int y32, int* SCORE)
{
	if (abs(x31 - x32) == 1 || abs(y31 - y32) == 1) {
		if (!(abs(x31 - x32) == 1 && abs(y31 - y32))) {
			int ts;
			ts = s[x31][y31];
			s[x31][y31] = s[x32][y32];
			s[x32][y32] = ts;

			int check_swap = game_score(pCGI, s, r, c, h, SCORE);
			if (!check_swap) {
				ts = s[x31][y31];
				s[x31][y31] = s[x32][y32];
				s[x32][y32] = ts;
				return 0;
			}

			// 绘制交换
			gmw_draw_block(pCGI, x31, y31, s[x31][y31], bdi_normal);
			gmw_draw_block(pCGI, x32, y32, s[x32][y32], bdi_normal);

			return 1;
		}
	}
	return 0;
}

int game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	int h[MAX_ROW][MAX_COL] = { 0 };
	int loop = 1;
	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;
	int SCORE = 0;
	char temp[256];
	while (1) {
		sprintf(temp, "（当前分数：%d 右键退出）", SCORE);
		gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
		int loop_2 = 1;
		int old_mrow = mrow;
		int old_mcol = mcol;
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:
					sprintf(temp, "游戏结束！当前分数：%d", SCORE);
					gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
					return -1;
				case MOUSE_LEFT_BUTTON_CLICK:
					old_mrow = mrow;
					old_mcol = mcol;
					hint(pCGI, s, pCGI->row_num, pCGI->col_num, h);
					if (h[mrow][mcol]) {
						gmw_draw_block(pCGI, mrow, mcol, s[mrow][mcol], bdi_selected);
						while (loop_2) {
							ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);
							if (maction == MOUSE_LEFT_BUTTON_CLICK && h[mrow][mcol]) {
								int check_swap = game_swap(pCGI, s, pCGI->row_num, pCGI->col_num, h, old_mrow, old_mcol, mrow, mcol, &SCORE);
								if (!check_swap) {
									gmw_status_line(pCGI, LOWER_STATUS_LINE, "不能交换");
									gmw_draw_block(pCGI, old_mrow, old_mcol, s[old_mrow][old_mcol], bdi_prompt);
									loop_2 = 0;
									break;
								}
								else if (check_swap) {
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
									int helper_hint = hint(pCGI, s, pCGI->row_num, pCGI->col_num, h);
									if (!helper_9 && !helper_hint) {
										sprintf(temp, "游戏结束！当前分数：%d", SCORE);
										gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
										gmw_status_line(pCGI, LOWER_STATUS_LINE, "（无可消除项，游戏结束!）");
										return -1;
									}
									else {
										loop_2 = 0;
										break;
									}
								}
							}
							else if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
								sprintf(temp, "游戏结束！当前分数：%d", SCORE);
								gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
								return -1;
							}
						}
					}
					else {
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "不能选择");
					}
					break;
			}
		}
	}
}