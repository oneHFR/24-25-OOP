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
#include "90-01-b2-gmw.h"

using namespace std;

void h9(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	init(pCGI, s);
	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(pCGI, pCGI->row_num, pCGI->col_num);
	/* ��ʾ��� */
	gmw_draw_frame(pCGI);
	fill(pCGI, s, 9);
	/* ��״̬����ʾ���� */
	char temp[256];
	sprintf(temp, "���ڴ�С��%d�� %d��",  pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

	game(pCGI, s);

	return ;
}