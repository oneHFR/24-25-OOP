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

	while (1) {
		int s[MAX_ROW][MAX_COL] = { 0 };
		int maxscore = 0, t = 0;
		cct_setcolor();
		cct_cls();
		init(&G2048_CGI, 1);
		gmw_draw_frame(&G2048_CGI);

		input_parameter(&G2048_CGI, &t);
		//G2048_CGI.row_num = 5;
		//G2048_CGI.col_num = 4;
		//G2048_CGI.delay_of_block_moved = 0;
		//G2048_CGI.delay_of_block_moved = BLOCK_MOVED_DELAY_MS + 5 * G2048_CGI.delay_of_block_moved;
		/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
		init(&G2048_CGI, 2);
		gmw_draw_frame(&G2048_CGI);

		new_one(&G2048_CGI, s, bdi);
		new_one(&G2048_CGI, s, bdi);

		game(&G2048_CGI, s, bdi, t);

		To_be_continued("��Ϸ�ؿ�", &G2048_CGI);
	}
	return 0;
}