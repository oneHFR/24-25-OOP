/* 2152131 �ƿ� ����� */
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

void input(CONSOLE_GRAPHICS_INFO* const pCGI, int min, int max)
{
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
	pCGI->extern_right_cols = 3;
	pCGI->extern_left_cols = 3;
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_HWHITE);			//����������ɫ
	gmw_set_font(pCGI, "������", 20);							//����
	//	gmw_set_frame_style(pCGI);									//��Ϸ��������ÿ��ɫ���2��1���зָ���
	gmw_set_frame_default_linetype(pCGI, 4);					//��Ϸ���������ͣ��ᵥ��˫
	gmw_set_frame_color(pCGI, COLOR_HWHITE, COLOR_BLACK);		//��Ϸ��������ɫ
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);	//����Ҫ��״̬��
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);	//��Ҫ��״̬��
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

void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{

}