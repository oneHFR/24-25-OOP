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
    int h[MAX_ROW][MAX_COL] = { 0 };
    /* �� test��copy������ */
	init(pCGI, s);
	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(pCGI, pCGI->row_num, pCGI->col_num);
	/* ��ʾ��� */
	gmw_draw_frame(pCGI);
	fill(pCGI, s, 9);
	/* ��״̬����ʾ���� */
	char temp[256];
	sprintf(temp, "���ڴ�С��%d�� %d��",  pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
    /* ����ʼ���������Ķ��� */
	int helper_9 = check(pCGI, s, pCGI->row_num, pCGI->col_num, h);
    if (helper_9) {
        while (helper_9) {
            remove(pCGI, s, pCGI->row_num, pCGI->col_num, h);
            remove_zero(s, pCGI->row_num, pCGI->col_num, h); //����ȡ�� Ϊ����ṩʶ����
            fall(pCGI, s, pCGI->row_num, pCGI->col_num); // ���䶯��
            fill_new(pCGI, s, pCGI->row_num, pCGI->col_num, 9);
            ini_h(h);
            helper_9 = check(pCGI, s, pCGI->row_num, pCGI->col_num, h);
        }
    }

    /* ��ʾ��ʾ��������Ϸģʽ */
    int helper_hint = hint(pCGI, s, pCGI->row_num, pCGI->col_num, h);
    if (!helper_9 && !helper_hint) {
        gmw_status_line(pCGI, LOWER_STATUS_LINE, "���޿��������Ϸ����!��");
        return ;
    }
    else 
        game(pCGI, s);
	return ;
}