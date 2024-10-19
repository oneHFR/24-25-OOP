/* 2152131 �ƿ� ����� */
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

void input(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	int min = 8;
	int max = 10;
	while (1) {
		cout << "����������(" << min << "-" << max << ")��" << endl;
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
		cout << "����������(" << min << "-" << max << ")��" << endl;
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
	gmw_set_font(pCGI, "������", 16, 8);
	gmw_set_frame_style(pCGI, 6, 3, true);	//ɫ����߿򣬿��Ϊ6���߶�Ϊ3
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
	for (int i = 0; i <= 7; i++) {
		s[i][0] = 1;
		s[i][1] = 1;
		s[i][5] = 1;
		s[i][4] = 1;
		gmw_draw_block(pCGI, i, 0, s[i][0], bdi_normal);
		gmw_draw_block(pCGI, i, 1, s[i][1], bdi_normal);
		gmw_draw_block(pCGI, i, 5, s[i][5], bdi_normal);
		gmw_draw_block(pCGI, i, 4, s[i][4], bdi_normal);
	}
	s[0][2] = 1;
	s[0][3] = 1;
	gmw_draw_block(pCGI, 0, 3, s[0][3], bdi_normal);
	gmw_draw_block(pCGI, 0, 2, s[0][2], bdi_normal);
}

void search(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int ir, int ic, int target, int option) {
	int i = 0, j = 0;
	//����
	if (option != 1) {
		for (i = ir - 1; i >= 0; i--) {
			if (s[i][ic] != target) {
				break;
			}
			else if (h[i][ic] != 1) {
				h[i][ic] = 1;
				search(s, r, c, h, i, ic, target, 2);
			}
		}
	}

	//����
	if (option != 2) {
		for (i = ir + 1; i <= r; i++) {
			if (s[i][ic] != target) {
				break;
			}
			else if (h[i][ic] != 1) {
				h[i][ic] = 1;
				search(s, r, c, h, i, ic, target, 1);
			}
		}
	}

	//����
	if (option != 3) {
		for (j = ic - 1; j >= 0; j--) {
			if (s[ir][j] != target) {
				break;
			}
			else if (h[ir][j] != 1) {
				h[ir][j] = 1;
				search(s, r, c, h, ir, j, target, 4);
			}
		}
	}

	//����
	if (option != 4) {
		for (j = ic + 1; j <= c; j++) {
			if (s[ir][j] != target) {
				break;
			}
			else if (h[ir][j] != 1) {
				h[ir][j] = 1;
				search(s, r, c, h, ir, j, target, 3);
			}
		}
	}
}

int check_valid(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int ir, int ic)
{
	int target = s[ir][ic];
	h[ir][ic] = 1;
	search(s, r, c, h, ir, ic, target, 0);

	for (int j = 0; j < c; j++) {
		for (int i = 0; i < r; i++) {
			if (h[i][j] == 1 && !(i == ir && j == ic))
				return 1;
		}
	}
	return 0;
}

void ini_h(int (*h)[MAX_COL])
{
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			h[i][j] = 0;
		}
	}
}

int check_finish(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL])
{
	int check_keep = 1;
	for (int j = 0; j < c; j++) {
		for (int i = 0; i < r; i++) {
			if (s[i][j]) {
				ini_h(h);
				check_keep = check_valid(s, r, c, h, i, j);
				if (check_keep)
					return 1;
			}
		}
	}
	return 0;
}

void search(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int ir, int ic, int target, int(*pool)[MAX_COL], int option) {
	int i = 0, j = 0;
	//����
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

	//����
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

	//����
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

	//����
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

void ceyi(const CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c)
{
	int stride = 0;
	for (int j = 0; j < c - 1; ++j) {
		stride = 0;
		for (int st = j; st < c; st++) {
			if (s[r - 1][st] != 0)
				break;
			else
				stride++;
		}
			if (stride) {
				for (int i = r - 1; i >= 0; i--) {
					if (s[i][j + stride]) {
						gmw_move_block(pCGI, i, j + stride, s[i][j + stride], 0, bdi_normal, RIGHT_TO_LEFT, stride);
						s[i][j] = s[i][j + stride];
						s[i][j + stride] = 0;
					}
				}
			}
	}
}
void Score(int r, int c, int (*h)[MAX_COL], int* num, int* total, int* rest, int mode = 0)
{
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
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

void remove_0(int (*s)[MAX_COL], int r, int c, int(*h)[MAX_COL])
{
	for (int j = 0; j < c; j++) {
		for (int i = 0; i < r; i++) {
			if (h[i][j] == 1) {
				s[i][j] = 0;
			}
		}
	}
}



int Game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	int grid = 1;
	int h[MAX_ROW][MAX_COL] = { 0 };
	while (1) {
		int loop = 1;
		int X, Y;
		int maction, mrow = -1, mcol = -1;
		int keycode1, keycode2;
		int ret;
		char temp[100] = { 0 };
		int num = 0;
		int sel = 0;
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
					// �ָ�
					for (int i = 0; i < pCGI->row_num; i++) {
						for (int j = 0; j < pCGI->col_num; j++) {
							if (h[i][j]) {
								gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
							}
						}
					}
					ini_h(h);
					sel = 0;
					if (old_mrow >= 0 && old_mcol >= 0) {
						gmw_draw_block(pCGI, old_mrow, old_mcol, s[old_mrow][old_mcol], bdi_normal);
					}
					gmw_draw_block(pCGI, mrow, mcol, s[mrow][mcol], bdi_selected);
				}
				else{
					ini_h(h);
					if (check_valid(s, pCGI->row_num, pCGI->col_num, h, mrow, mcol)) {
						if (!sel) {
							// �����ɫ // �����ʧ��
							for (int i = 0; i < pCGI->row_num; i++) {
								for (int j = 0; j < pCGI->col_num; j++) {
									if (h[i][j]) {
										gmw_draw_block(pCGI, i, j, s[i][j], bdi_selected);
									}
								}
							}
							gmw_status_line(pCGI, LOWER_STATUS_LINE, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
							sel = 1;
						}

						else if (sel) {
							//��ʧ
							for (int i = 0; i < pCGI->row_num; i++) {
								for (int j = 0; j < pCGI->col_num; j++) {
									if (h[i][j]) {
										gmw_draw_block(pCGI, i, j, 0, bdi_normal);
									}
								}
							}

							sel = 0;
							Score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest);
							sprintf(temp, "���ε÷֣�%d �ܵ÷֣�%d", num, total);
							gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
							sprintf(temp, "�ϳ���ɣ��س���/�����������0");
							gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
							// flag
							while (1) {
								ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
								if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
									break;
								else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
									break;
							}
							// ����
							remove_0(s, pCGI->row_num, pCGI->col_num, h);
							fall(pCGI, s, pCGI->row_num, pCGI->col_num);
							ceyi(pCGI, s, pCGI->row_num, pCGI->col_num);
							int helper_3 = check_finish(s, pCGI->row_num, pCGI->col_num, h);
							if (!helper_3) {
								num = 0;
								ini_h(h);
								Score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest, 0);
								if (180 * (10 - rest) > 0)
									sprintf(temp, "�����÷֣�%d �ܵ÷֣�%d", 180 * (10 - rest), total);
								else
									sprintf(temp, "�����÷֣�0 �ܵ÷֣�%d", total);
								gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);

								sprintf(temp, "ʣ��%d�����ǣ��޿���������ؽ������س�������һ��", rest);
								gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
								while (grid) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
										break;
								}
								return 27;
							}
							//in_h(h);

							gmw_status_line(pCGI, LOWER_STATUS_LINE, "���κϳɽ���������");
							ini_h(h);
						}
						//break;
					}

					else {
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "��Χ����ֵͬ����ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����");
					}
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

					case 13:
						if (check_valid(s, pCGI->row_num, pCGI->col_num, h, mrow, mcol)) {
							if (!sel) {
								// �����ɫ // �����ʧ��
								for (int i = 0; i < pCGI->row_num; i++) {
									for (int j = 0; j < pCGI->col_num; j++) {
										if (h[i][j]) {
											gmw_draw_block(pCGI, i, j, s[i][j], bdi_selected);
										}
									}
								}
								gmw_status_line(pCGI, LOWER_STATUS_LINE, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
								sel = 1;
							}

							else if (sel) {
								//��ʧ
								for (int i = 0; i < pCGI->row_num; i++) {
									for (int j = 0; j < pCGI->col_num; j++) {
										if (h[i][j]) {
											gmw_draw_block(pCGI, i, j, 0, bdi_normal);
										}
									}
								}

								sel = 0;
								Score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest);
								sprintf(temp, "���ε÷֣�%d �ܵ÷֣�%d", num, total);
								gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
								sprintf(temp, "�ϳ���ɣ��س���/�����������0");
								gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
								// flag
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
										break;
								}
								// ����
								remove_0(s, pCGI->row_num, pCGI->col_num, h);
								fall(pCGI, s, pCGI->row_num, pCGI->col_num);
								ceyi(pCGI, s, pCGI->row_num, pCGI->col_num);

								int helper_3 = check_finish(s, pCGI->row_num, pCGI->col_num, h);
								if (!helper_3) {
									num = 0;
									ini_h(h);
									Score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest, 0);
									if (180 * (10 - rest) > 0)
										sprintf(temp, "�����÷֣�%d �ܵ÷֣�%d", 180 * (10 - rest), total);
									else
										sprintf(temp, "�����÷֣�0 �ܵ÷֣�%d", total);
									gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

									sprintf(temp, "ʣ��%d�����ǣ��޿���������ؽ������س�������һ��", rest);
									gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
									while (grid) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
										if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
											break;
									}
									return 27;
								}
								//in_h(h);

								gmw_status_line(pCGI, LOWER_STATUS_LINE, "���κϳɽ���������");
								ini_h(h);
							}
							break;
						}

						else {
							gmw_status_line(pCGI, LOWER_STATUS_LINE, "��Χ����ֵͬ");
						}
						break;

					case 0xE0: 
						// �ָ�
						for (int i = 0; i < pCGI->row_num; i++) {
							for (int j = 0; j < pCGI->col_num; j++) {
								if (h[i][j]) {
									gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
								}
							}
						}
						ini_h(h);
						sel = 0;
						if (old_mrow >= 0 && old_mcol >= 0) 
							gmw_draw_block(pCGI, old_mrow, old_mcol, s[old_mrow][old_mcol], bdi_normal);
						switch (keycode2) {
							case KB_ARROW_UP:
								do {
										mrow--;
										if (mrow < 0)
											mrow = pCGI->row_num - 1;
								} while (!s[mrow][mcol]);
								break;
							case KB_ARROW_DOWN:
								do {
									mrow++;
									if (mrow >= pCGI->row_num)
										mrow = 0;
								} while (!s[mrow][mcol]);
								break;
							case KB_ARROW_LEFT:
								do {
									mcol--;
									if (mcol < 0)
										mcol = pCGI->col_num - 1;
								} while (!s[mrow][mcol]);
								break;
							case KB_ARROW_RIGHT:
								do {
									mcol++;
									if (mcol >= pCGI->col_num)
										mcol = 0;
								} while (!s[mrow][mcol]);
								break;
						}
						gmw_draw_block(pCGI, mrow, mcol, s[mrow][mcol], bdi_selected);
						break;
				}
			}
		}
	}
	return 0;
}