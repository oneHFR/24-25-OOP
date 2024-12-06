/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-01-b2-magic_ball.h"
#include "../include/common_functions.h"
#include "../include/cmd_console_tools.h"
#include <Windows.h>

using namespace std;

/*------------------------------ 打印区----------------------------------*/
// common

/*------------------------------ 查找区----------------------------------*/


/*------------------------------ 填补区----------------------------------*/
void fill_new(int (*s)[11], int r, int c, int(*h)[11], int print_new_circle)
{
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	srand((unsigned)time(NULL));
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (s[i][j] == 0) {
				//srand((unsigned)time(NULL));
				s[i][j] = rand() % 9 + 1;
				if (print_new_circle) {
					cct_gotoxy(4 * j - 2, 2 * i);
					cct_setcolor(color[s[i][j]], 0);
					cout << "○"; // ●
					Sleep(200);
				}
			}
		}
	}
	if (print_new_circle) {
		cct_setcolor(0);
		cct_gotoxy(0, 2 * r + 2);
		cout << "                              ";
		cct_gotoxy(0, 2 * r + 2);
	}

}

/*------------------------------ 提示区----------------------------------*/

/*------------------------------ 悬浮区----------------------------------*/
int location(int (*s)[11], int r, int c, int (*h)[11], int option)
{
	cct_gotoxy(0, r * 2 + 2);
	cout << "[当前光标]";
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	int x = 0, y = 0;
	int x1 = 0, y1 = 0;
	int x3 = 0, y3 = 0;
	int game_choice = 0;
	int x31 = 0, y31 = 0;
	int x32 = 0, y32 = 0;
	cct_getxy(x1, y1);
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	int SCORE = 0;
	int check_swap = 0;
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);
	while (loop) {
		cct_gotoxy(0, r * 2 + 2);
		cout << "[当前光标]";
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<x,y>处 */
		ret = cct_read_keyboard_and_mouse(x, y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {
			cct_gotoxy(x1, y1);
			cout << "                                     ";
			if (y % 2 == 0 && (x % 4 == 2 || x % 4 == 3)) {
				if (x % 4 == 3) {
					x3 = (x + 1) / 4;
					x = x - 1;
				}
				else
					x3 = (x + 2) / 4;
				y3 = y / 2;

				if (y3 <= r && x3 <= c) {
					cct_gotoxy(x1, y1);
					cout << setw(2) << char(y3 + 'A' - 1) << "行" << setw(2) << x3 << "列";
				}
				else {
					cct_gotoxy(x1, y1);
					cout << "位置非法";
				}
			}
			else {
				cct_gotoxy(x1, y1);
				cout << "位置非法";
			}

			switch (maction) {
				case MOUSE_ONLY_MOVED:
					break;
				case MOUSE_LEFT_BUTTON_CLICK:
					ret = cct_read_keyboard_and_mouse(x, y, maction, keycode1, keycode2);
					if (x % 4 == 3) {
						x3 = (x + 1) / 4;
						x = x - 1;
					}
					else
						x3 = (x + 2) / 4;
					y3 = y / 2;
					// 选中    //当前选择H行3列  //不能选择I行4列
					if (y % 2 == 0 && (x % 4 == 2 || x % 4 == 3) && (y3 <= r && x3 <= c)) {
						if (h[y3][x3] == 1) {
							cct_gotoxy(0, r * 2 + 2);
							cout << "                                     ";
							cct_gotoxy(0, r * 2 + 2);
							cout << "当前选择" << setw(4) << char(y3 + 'A' - 1) << "行" << setw(2) << x3 << "列";
							if (option == 8) {
								Sleep(400);
								cct_gotoxy(0, r * 2 + 2);
								cout << "                                     ";
								cct_gotoxy(0, r * 2 + 2);
								return -2;
							}
							if (option == 9) {
								game_choice++;
								if (game_choice % 2 == 1) {
									x31 = x3;
									y31 = y3;
								}
								if (game_choice % 2 == 0) {
									x32 = x3;
									y32 = y3;
								}
								if (x31 != x32 && y32 != y31) {
									cct_gotoxy(x, y);
									cct_setcolor(color[s[y3][x3]], 15);
									cout << "◎"; // ●
									cct_setcolor(0);
								}
								else if (x31 == x32 && y32 == y31) {
									cct_gotoxy(x, y);
									cct_setcolor(color[s[y3][x3]], 0);
									cout << "◎"; // ●
									cct_setcolor(0);
									x31 = 0;
									y31 = 0;
									x32 = 0;
									y32 = 0;
									break;
								}

								if (game_choice % 2 == 0) {
									check_swap = game_swap(s, r, c, h, x31, y31, x32, y32, &SCORE);
									if (!check_swap) {
										cct_gotoxy(0, r * 2 + 2);
										cout << "                                     ";
										cct_gotoxy(0, r * 2 + 2);
										cout << "不能交换                     ";
										Sleep(400);
									}
									else {
										int helper_9_game = check_1(s, r, c, h, 9);
										if (helper_9_game) {
											while (helper_9_game) {
												remove_circle(s, r, c, h);
												falling_down(s, r, c, h); // 下落动画
												remove_0(s, r, c, h); //把零取消 为填充提供识别项
												fill_new(s, r, c, h, 1);
												helper_9_game = check_1(s, r, c, h, 9);
											}
										}
										print_circle(s, r, c, 1, h, 1, 9);
										int helper_hint_game = hint(s, r, c, h, 1); // 无可消除项，游戏结束!
										if (!helper_9_game && !helper_hint_game) {
											cct_gotoxy(0, 0); //无可消除项，游戏结束!最终分数:3)
											cout << "无可消除项，游戏结束!最终分数:" << SCORE << ")        ";
											cct_gotoxy(0, r * 2 + 2);
											cout << "                                     ";
											cct_gotoxy(0, r * 2 + 3);

											return -4;
										}
										else {
											cct_gotoxy(14, 0);
											cout << "（当前分数：" << SCORE << " 右键退出）";
										}
									}
									x31 = 0;
									y31 = 0;
									x32 = 0;
									y32 = 0;
								}
							}
						}
						else {
							cct_gotoxy(0, r * 2 + 2);
							cout << "                                     ";
							cct_gotoxy(0, r * 2 + 2);
							cout << "不能选择" << setw(4) << char(y3 + 'A' - 1) << "行" << setw(2) << x3 << "列";
							Sleep(180); // 如何实现选中持续显示呢？
						}
					}
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:
					if (y % 2 == 0 && (x % 4 == 2 || x % 4 == 3) && (y3 <= r && x3 <= c)) {
						loop = 0; // 右键点击退出循环
						cct_gotoxy(0, r * 2 + 2);
						cout << "                                     ";
						cct_gotoxy(0, r * 2 + 2);
						return -1;
						break;
					}
					break;
				default:
					break;
			}
		}
	}
	return 0;
}
/*------------------------------ 游戏区----------------------------------*/
int game_score(int(*s)[11], int r, int c, int (*h)[11], int* SCORE)
{
	int check_swap = 0;
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			h[i][j] = 0;
		}
	}

	check_1(s, r, c, h, 9);
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (h[i][j] == 1) {
				check_swap = 1;
				(*SCORE)++;
			}
		}
	}
	return check_swap;
}

int game_swap(int(*s)[11], int r, int c, int (*h)[11], int x31, int y31, int x32, int y32, int* SCORE)
{
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	int x1, y1, x2, y2;
	y1 = 2 * y31;
	y2 = 2 * y32;
	x1 = 4 * x31 - 2;
	x2 = 4 * x32 - 2;
	if (abs(x31 - x32) == 1 || abs(y31 - y32) == 1) {
		if (!(abs(x31 - x32) == 1 && abs(y31 - y32))) {
			int ts;
			ts = s[y31][x31];
			s[y31][x31] = s[y32][x32];
			s[y32][x32] = ts;

			int check_swap = game_score(s, r, c, h, SCORE);
			if (!check_swap) {
				ts = s[y31][x31];
				s[y31][x31] = s[y32][x32];
				s[y32][x32] = ts;
				cct_gotoxy(x1, y1);
				cct_setcolor(color[s[y31][x31]], 0);
				// ○
				cout << "◎"; // ●
				cct_gotoxy(x2, y2);
				cct_setcolor(color[s[y32][x32]], 0);
				cout << "◎"; // ●
				cct_setcolor(0);

				return 0;
			}

			// 绘制交换
			cct_gotoxy(x1, y1);
			cct_setcolor(color[s[y31][x31]], 0);
			cout << "○"; // ●
			cct_gotoxy(x2, y2);
			cct_setcolor(color[s[y32][x32]], 0);
			cout << "○"; // ●
			cct_setcolor(0);

			return 1;
		}
	}
	cct_gotoxy(x2, y2);
	cct_setcolor(color[s[y32][x32]], 0);
	cout << "◎"; // ●
	cct_gotoxy(x1, y1);
	cct_setcolor(color[s[y31][x31]], 0);
	cout << "◎"; // ●
	cct_setcolor(0);

	return 0;
}

/*刚开始全部写在Tool里面忘掉还有一个graph了！！*/
/*------------------------------ 图形区----------------------------------*/
/*
*
/*绘制
void set_cmd(int r);
void reset_cmd();
void print_canvas(int(*s)[11], int r, int c, int grid, int speed);
void print_circle(int(*s)[11], int r, int c, int grid, int(*h)[11], int speed, int option);
void remove_circle(int(*s)[11], int r, int c, int(*h)[11]);
/*动态下落
void falling_down(int(*s)[11], int r, int c, int(*h)[11]);
void lan(int(*s)[11], int j, int w, int lie, int hang, int helper_down);
*
*/

/*------------------------------ 回车区----------------------------------*/

/*------------------------------ 淘汰区----------------------------------*/
void tips(int(*s)[11], int r, int c, int(*h)[11])
{
	// 这个思路不好 没写完 先空着 想复杂了 后面不好变通
	int helper_num = 0;
	int helper_check_r = 0;
	int helper_check_c = 0;
	int helper_r_1 = 0;
	int helper_r_3 = 0;
	int helper_c_1 = 0;
	int helper_c_3 = 0;
	for (int i = 1; i < r; i++) {
		for (int j = 1; j < c; j++) {
			int w[10][10] = { 0 };
			helper_r_1 = 0;
			helper_r_3 = 0;
			helper_c_1 = 0;
			helper_c_3 = 0;
			helper_check_r = 0;
			helper_check_c = 0;

			for (int y = j - 1; y <= j + 1; j++) {
				for (int x = i - 1; x <= i + 1; i++) {
					helper_num = 0;
					if (s[i][j] == s[x][y]) {
						if (x == i - 1 && helper_r_1 == 0) {
							helper_r_1 = 1;
							helper_check_r += x;
						}
						else if (x == i + 1 && helper_r_3 == 0) {
							helper_r_3 = 1;
							helper_check_r += x;
						}
						if (y == j - 1 && helper_c_1 == 0) {
							helper_c_1 = 1;
							helper_check_c += y;
						}
						else if (y == j + 1 && helper_c_3 == 0) {
							helper_c_3 = 1;
							helper_check_c += y;
						}
						helper_num++;
					}
				}
			}
			if (helper_num >= 2) {
				if (!(helper_check_c == 2 * j) || !(helper_check_r == 2 * i)) {

				}
			}
			else {
				;
			}
		}
	}
}

/*
			step = 1 +helper_down;
			helper_up_search = 0;
			if (h[i][j] == 1 || helper_down && (2 * i < already_up)) {
				if (h[i][j]) {
					for (w = i - 1; w > 0; w--) {
						if (h[w][j] == 1)
							step++;
						else
							helper_up_search = 1;

						if (helper_up_search) {
							if (h[w - helper_down][j] != 1 && w < already_up / 2 && w - helper_down > 0 ) {
								if (2 * (w - helper_down) < already_up) {
									hang = 2 * r - 2 * (9 - i) - 2 * step + 1;
									already_up = min(hang, already_up) - 1;
									for (int step_i = 0; step_i < step; step_i++) {
										if (hang < 2 * r - 2 * helper_down && hang < already_bottom - 1 && already_up > 0) {
											lan(s, j, w, lie, hang, helper_down);
											hang = hang + 2;
										}
										else
											break;
									}
									helper_down++;
									cct_getxy(x, y);
									already_bottom = min(y, already_bottom);
									break;

								}
							}
						}
					}
				}

				else {
					if (2 * i < already_up && w - helper_down > 0) {
						hang = 2 * r - 2 * (9 - i - 1) - 2 * step + 1;
						already_up = min(hang, already_up) - 1;
						for (int step_i = 0; step_i < step; step_i++) {
							if (hang < 2 * r - 2 * helper_down && hang < already_bottom - 1 && already_up > 0) {
								lan(s, j, w, lie, hang, helper_down);
								hang = hang + 2;
							}
						}
						helper_down++;
						cct_getxy(x, y);
						already_bottom = min(y, already_bottom);
						break;
					}
					else if (2 * i == already_up - 2) {
						hang = 2 * r - 2 * (9 - i) + 1;
						already_up -= 2;
						for (int step_i = 0; step_i < step; step_i++) {
							if (hang < 2 * r - 2 * helper_down && hang < already_bottom - 1 && already_up > 0) {
								lan(s, j, i, lie, hang, 0);
								hang = hang + 2;
							}
						}

					}

				}
			}

*/









