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

int H7(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL])
{
	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����������к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	init(pCGI, s);

	/* ��״̬����ʾ���� */
	char temp[256];
	sprintf(temp, "���ڴ�С��%d�� %d��", pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
	cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y +1);
	cout << "[Q �˳�][�Ҽ� �˳�]/[R �ؿ�]";
	//void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t);
	
	int next = Game(pCGI, s);
	if (next == 1)
		H7(pCGI, s);
	return 0;
}