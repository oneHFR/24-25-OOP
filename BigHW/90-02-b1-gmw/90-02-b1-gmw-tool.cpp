/* 2152131 �ƿ� ����� */
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
					sprintf(temp, "ѡ���� %c��%d��", char('A' + mrow), mcol);
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
								// �����ɫ // �����ʧ��
								for (int i = 1; i <= pCGI->row_num; i++) {
									for (int j = 1; j <= pCGI->col_num; j++) {
										if (h[i][j]) {
											gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
										}
									}
								}
								gmw_status_line(pCGI, LOWER_STATUS_LINE, "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�");
								sel = 1;
							}
							else if (sel) {
								//��ʧ
								for (int i = 1; i <= pCGI->row_num; i++) {
									for (int j = 1; j <= pCGI->col_num; j++) {
										if (h[i][j]) {
											gmw_draw_block(pCGI, i, j, s[i][j], bdi_normal);
										}
									}
								}

								sel = 0;
								score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest);
								sprintf(temp, "���ε÷֣�%d �ܵ÷֣�%d", num, total);
								gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

								gmw_status_line(pCGI, LOWER_STATUS_LINE, "�ϳ���ɣ��س�����");
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
										break;
								}
								// ����
								
								// ������
								remove_0(s, pCGI->row_num, pCGI->col_num, h, 1);
								falling_down(s, pCGI->row_num, pCGI->col_num, h, 2, grid);
								// ������
								remove_0(s, pCGI->row_num, pCGI->col_num, h, 0, 's');
								/*
								   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
								   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
								   const int bdi_value						����Ҫ��ʾ��ֵ
								   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
								   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
								   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
								   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
								*/
								// gmw_move_block(pCGI, 1, 2, mb[1][2], 0, bdi_normal, UP_TO_DOWN, 1);
								//drop(s, pCGI->row_num, pCGI->col_num, grid, 2);
								remove_0(s, pCGI->row_num, pCGI->col_num, h, 2);
								// ��������
								int helper_3 = check_3(s, pCGI->row_num, pCGI->col_num, h);
								if (!helper_3) {
									num = 0;
									in_h(h);
									score(pCGI->row_num, pCGI->col_num, h, &num, &total, &rest, 0);
									if (180 * (10 - rest) > 0)
										sprintf(temp, "�����÷֣�%d �ܵ÷֣�%d", 180 * (10 - rest), total);
									else
										sprintf(temp, "�����÷֣�0 �ܵ÷֣�%d", total);
									gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);

									sprintf(temp, "ʣ��%d�����ǣ��޿���������ؽ������س�������һ��", rest);
									gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
									while (1) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
										if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
											break;
									}
									return 0;  // flag
								}

								gmw_status_line(pCGI, LOWER_STATUS_LINE, "���κϳɽ����� ��C����");
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
							gmw_status_line(pCGI, LOWER_STATUS_LINE, "��Χ����ֵͬ");
							inval = true;
						}
						break;
				}
			}
		}



	}
	return 0;
}