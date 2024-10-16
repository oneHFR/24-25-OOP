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
#include "90-02-b1-gmw.h"

using namespace std;

void input(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	int min = 8;
	int max = 10;
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
	//input(pCGI);
	pCGI->row_num = 8;
	pCGI->col_num = 8;
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_font(pCGI, "新宋体", 16, 8);
	gmw_set_frame_style(pCGI, 6, 3, true);	//色块带边框，宽度为6，高度为3
	gmw_set_frame_color(pCGI, COLOR_HWHITE, COLOR_BLACK);
	gmw_set_block_border_switch(pCGI, true);

	gmw_set_rowcol(pCGI, pCGI->row_num, pCGI->col_num);
	gmw_set_colno_switch(pCGI, true);
	gmw_set_rowno_switch(pCGI, true);
	gmw_draw_frame(pCGI);

	fill(pCGI, s);
}

void fill(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL])
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			s[i][j] = rand() % 5 + 1;
			gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
		}
	}
	//for (int i = 1; i <= r; i++) {
	//	s[i][1] = 1;
	//}
	//for (int i = 1; i <= r; i++) {
	//	s[i][4] = 1;
	//}
	//for (int i = 1; i <= r; i++) {
	//	s[i][5] = 1;
	//}
	//s[5][2] = 1;
	//s[5][3] = 1;
}

int check_2(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int ir, int ic)
{
	int pool[MAX_ROW +1][MAX_COL +1] = { 0 };
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
	return 0;
}

void search(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int ir, int ic, int target, int(*pool)[MAX_COL], int option) {
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

int Game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	int grid = 1;
	while (1) {
		int loop = 1;
		int X, Y;
		int maction, mrow = -1, mcol = -1;
		int keycode1, keycode2;
		int ret;
		char temp[100] = { 0 };
		int h[MAX_ROW][MAX_COL] = { 0 };
		int num = 0;
		int sel = 0;
		int inval = 0;
		int total = 0;
		int rest = pCGI->row_num * pCGI->col_num;

		while (loop) {
			num = 0;
			int old_mrow = mrow;
			int old_mcol = mcol;
			ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);

			if (ret == CCT_MOUSE_EVENT) {
				if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
					return 0;
				}
				else if (maction == MOUSE_ONLY_MOVED) {
					if (old_mrow >= 0 && old_mcol >= 0)
						gmw_draw_block(pCGI, old_mrow, old_mcol, s[old_mrow][old_mcol], bdi_normal);
					gmw_draw_block(pCGI, mrow, mcol, s[mrow][mcol], bdi_selected);
				}
				else{
					//int check = 1;
					sprintf(temp, "选中了 %c行%d列", char('A' + mrow), mcol);
					gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
					loop = 0;
				}
			}

			else {
				switch (keycode1) {
					case 'Q':
					case 'q':
						return 0;

					case 'R':
					case 'r':
						return 1;

					case 0xE0: 
						if (old_mrow >= 0 && old_mcol >= 0)
							gmw_draw_block(pCGI, old_mrow, old_mcol, s[old_mrow][old_mcol], bdi_normal);
						gmw_draw_block(pCGI, mrow, mcol, s[mrow][mcol], bdi_selected);
						sel = 0;
						switch (keycode2) {
							case KB_ARROW_UP:
								mrow--;
								if (mrow < 0)
									mrow = pCGI->row_num - 1;
								break;
							case KB_ARROW_DOWN:
								mrow++;
								if (mrow >= pCGI->row_num)
									mrow = 0;
								break;
							case KB_ARROW_LEFT:
								mcol--;
								if (mcol < 0)
									mcol = pCGI->col_num - 1;
								break;
							case KB_ARROW_RIGHT:
								mcol++;
								if (mcol >= pCGI->col_num)
									mcol = 0;
								break;
						}
						break;

					case 13:
						if (check_2(s, pCGI->row_num, pCGI->col_num, h, old_mrow, old_mrow, 1)) {
							if (!sel) {
								// 集体变色 // 如何消失？
								for (int i = 1; i <= pCGI->row_num; i++) {
									for (int j = 1; j <= pCGI->col_num; j++) {
										if (h[i][j]) {
											gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
										}
									}
								}
								gmw_status_line(pCGI, LOWER_STATUS_LINE, "箭头键/鼠标移动取消当前选择，回车键/单击左键合成");
								sel = 1;
							}
							else if (sel) {
								//消失
								for (int i = 1; i <= pCGI->row_num; i++) {
									for (int j = 1; j <= pCGI->col_num; j++) {
										if (h[i][j]) {
											gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
										}
									}
								}

								sel = 0;
								score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest);
								sprintf(temp, "本次得分：%d 总得分：%d", num, total);
								gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

								gmw_status_line(pCGI, LOWER_STATUS_LINE, "合成完成，回车继续");
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
										break;
								}
								// 下落
								
								// 先逐行
								remove_0(s, pCGI->row_num, pCGI->col_num, h, 1);
								falling_down(s, pCGI->row_num, pCGI->col_num, h, 2, grid);
								// 再逐列
								remove_0(s, pCGI->row_num, pCGI->col_num, h, 0, 's');
								/*
								   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
								   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
								   const int bdi_value						：需要显示的值
								   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
								   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
								   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
								   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
								*/
								// gmw_move_block(pCGI, 1, 2, mb[1][2], 0, bdi_normal, UP_TO_DOWN, 1);
								//drop(s, pCGI->row_num, pCGI->col_num, grid, 2);
								remove_0(s, pCGI->row_num, pCGI->col_num, h, 2);
								// 检查继续性
								int helper_3 = check_3(s, pCGI->row_num, pCGI->col_num, h);
								if (!helper_3) {
									num = 0;
									in_h(h);
									score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest, 0);
									if (180 * (10 - rest) > 0)
										sprintf(temp, "奖励得分：%d 总得分：%d", 180 * (10 - rest), total);
									else
										sprintf(temp, "奖励得分：0 总得分：%d", total);
									gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);

									sprintf(temp, "剩余%d个星星，无可消除项，本关结束！回车继续下一关", rest);
									gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
									while (1) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
										if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
											break;
									}
									return 0;  // flag
								}

								gmw_status_line(pCGI, LOWER_STATUS_LINE, "本次合成结束， 按C继续");
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_MOUSE_EVENT)
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 'c' || keycode1 == 'C')
										break;
								}
							}
							break;
						}

						else {
							gmw_status_line(pCGI, LOWER_STATUS_LINE, "周围无相同值");
							inval = true;
						}
						break;
				}
			}
		}



	}
	return 0;
}