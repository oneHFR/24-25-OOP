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
#include "90-02-b2.h"

using namespace std;

// 2048 
/*
Ŀ��:2048 ����:4 ���:1356 ʱ��:30 (R:���� Q:�˳�)
�ȴ�ӡcanvas
��Ϸ2048�������ã�
����������[4-8] 4
����������[4-10] 8
������Ŀ�����[2048/4096/8192/16384] 2048
�����붯����ʱ[0-5] 0
*/
int main()
{
	srand((unsigned int)time(0));

	CONSOLE_GRAPHICS_INFO G2048_CGI;

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  // 0�ո�
	{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{4, COLOR_HRED, COLOR_BLACK, NULL},
	{8, COLOR_GREEN, COLOR_BLACK, NULL},
	{16, COLOR_RED, COLOR_BLACK, NULL},
	{32, COLOR_HGREEN, COLOR_BLACK, NULL},
	{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{128, COLOR_HRED, COLOR_BLACK, NULL},
	{256, COLOR_HWHITE, COLOR_BLACK, NULL},
	{512, COLOR_HBLACK, COLOR_BLACK, NULL},
	{1024, COLOR_HPINK, COLOR_BLACK, NULL},
	{2048, COLOR_WHITE, COLOR_BLACK, NULL},
	{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
	{BDI_VALUE_END, -1, -1, NULL} // NULL
	};

	while (1) {
		int s[MAX_ROW][MAX_COL] = { 0 };
		int maxscore = 0, t = 0;
		cct_setcolor();
		cct_cls();
		init(&G2048_CGI);
		input_parameter(&G2048_CGI, &t);
		gmw_draw_frame(&G2048_CGI);
		new_one(&G2048_CGI, s);

		int x, y;
		cout << "\n��С�������������End����...";
		cct_getxy(x, y);
		while (1) {
			char keepon[1024] = { 0 };
			cin.getline(keepon, 1024);
			for (int i = 0; keepon[i] != 0; i++) {
				if (keepon[i] <= 'z' && keepon[i] >= 'a') {
					keepon[i] = keepon[i] - ('a' - 'A');
				}
			}
			if (!strcmp(keepon, "END")) {
				break;
			}
			else {
				cct_gotoxy(0, y + 1);
				cout << "�����������������";
				cct_gotoxy(x, y);
				cout << setw(50) << " ";
				cct_gotoxy(x, y);
			}
		}

	}


	return 0;
}