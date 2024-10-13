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

/*
Ŀ��:2048 ����:4 ���:1356 ʱ��:30 (R:���� Q:�˳�)
�ȴ�ӡcanvas
��Ϸ2048�������ã�
����������[4-8] 4
����������[4-10] 8
������Ŀ�����[2048/4096/8192/16384] 2048
�����붯����ʱ[0-5] 0
*/

void input_parameter(CONSOLE_GRAPHICS_INFO* const pCGI, int *t)
{
	int x0 = 3;
	int y0 = 3;
	int x, y;
	cct_gotoxy(x0, y0);
	cout << "��Ϸ2048�������ã�" << endl;

	cct_gotoxy(x0, y0+1);
	cout << "����������[4-8]";
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->row_num;
		if (pCGI->row_num == 13 || !cin.good() || pCGI->row_num < 4 || 8 < pCGI->row_num) {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
		else
			break;
	}

	cct_gotoxy(x0, y0 + 2);
	cout << "����������[4-10]"; // ������Ժ궨��һ�µ�����������
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->col_num;
		if (!cin.good() || pCGI->col_num < 4 && 10 < pCGI->col_num) {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
		else
			break;
	}

	int min_t;
	if (pCGI->col_num * pCGI->row_num <= 20)
		min_t = 512;
	if (pCGI->col_num * pCGI->row_num >= 25)
		min_t *= 2;
	if (pCGI->col_num * pCGI->row_num >= 36)
		min_t *= 2;
	if (pCGI->col_num * pCGI->row_num >= 49)
		min_t *= 2;
	if (pCGI->col_num * pCGI->row_num >= 64)
		min_t *= 2;
	// д����ȥ��hh
	cct_gotoxy(x0, y0 + 3);
	cout << "������Ŀ�����[" <<min_t << '/' << 2 * min_t << '/' << 4 * min_t << '/' << 8 * min_t << "]:";
	cct_getxy(x, y);
	while (1) {
		cin >> *t;
		if (cin.good() && *t == min_t || *t == 2 * min_t || *t == 4 * min_t || *t == 8 * min_t)
			break;
		else {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
	}

	cct_gotoxy(x0, y0 + 4);
	cout << "�����붯���ӳ�[0-5]";
	cct_getxy(x, y);
	while (1) {
		cin >> pCGI->delay_of_block_moved;
		if (!cin.good() || 0 > pCGI->delay_of_block_moved && pCGI->delay_of_block_moved > 5) {
			cin.clear();
			cin.ignore(1024, '\n');
			cct_showch(x, y, ' ', pCGI->area_fgcolor, pCGI->area_bgcolor, 20);
			cct_gotoxy(x, y);
		}
		else {
			/* ������ʱ������
			#define DELAY_OF_DRAW_FRAME		0	//����Ϸ�����ʱ����ʱ
			#define DELAY_OF_DRAW_BLOCK		1	//��ɫ��ʱ����ʱ
			#define DELAY_OF_BLOCK_MOVED		2	//ɫ���ƶ�ʱ����ʱ

			#define BLOCK_MOVED_DELAY_MS		15	//ɫ���ƶ���ȱʡ��ʱ����λms����������ȱʡΪ0��
			*/
			pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS + 5 * pCGI->delay_of_block_moved;
			break;
		}
	}

}

int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	int already = 0;
	for (int i = 0; i < pCGI->row_num; i++)
		for (int j = 0; j < pCGI->col_num; j++)
			if (s[i][j] != 0)
				already++;
	if (already == pCGI->row_num * pCGI->col_num)//ȫ��
		return 0;
	else {
		while (1){
			int r_i = rand() % pCGI->row_num;
			int r_j = rand() % pCGI->col_num;
			if (!s[r_i][r_j]) {
				s[r_i][r_j] = 2;
				break;
			}
		}
		return 1;
	}
}

void init(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/*
		  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
			   const int row					����Ϸ����ɫ��������ȱʡ10��
			   const int col					����Ϸ����ɫ��������ȱʡ10��
			   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
			   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
	  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У�
	  ���к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
		int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
	*/

	/*
	int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{

	pCGI->inited = CGI_INITED;

	gmw_set_rowcol(pCGI, row, col);
	gmw_set_font(pCGI);
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_delay(pCGI, DELAY_OF_DRAW_BLOCK, 0);
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, BLOCK_MOVED_DELAY_MS);
	gmw_set_ext_rowcol(pCGI);

	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI);

	gmw_set_block_default_linetype(pCGI, 1);
	gmw_set_block_border_switch(pCGI);


	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);

	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);


	pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;

	// 	FLAG
	return 0; //�˾�ɸ�����Ҫ�޸�
}
	*/

	gmw_init(pCGI);
	gmw_set_font(pCGI, "������", 16);
	gmw_set_frame_style(pCGI, 8, 2, false);
	gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
	gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);
	gmw_draw_frame(pCGI);
	gmw_set_frame_style(pCGI, 10, 5, true);

	//gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);
	//gmw_set_delay(&G2048_CGI, 1, 0);
	//gmw_set_delay(&G2048_CGI, 2, 0);
	//gmw_set_frame_style(&G2048_CGI, 3, 2, false);
	//gmw_set_rowcol(&G2048_CGI, r, c);
	//gmw_set_frame_style(&G2048_CGI, 10, 5, 1);
	//gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);
	//gmw_set_status_line_switch(&G2048_CGI, 0, 1);
	//gmw_set_status_line_switch(&G2048_CGI, 1, 0);
	
	/* ����ɫ����ʾʱ���ڲ������intֵ���������ʾͼ�εĶ�Ӧ��ϵ
   ע���˽ṹ�������ֵ�ڲ��������ж��壬������Ϸ�Ĳ�ͬ����ͬ 
	typedef struct _block_display_info_ {
		const int   value;	//Ҫ��ʾ���ڲ�����ֵ
		const int   bgcolor;	//-1��ʾ����Ϸ���򱳾�ɫ
		const int   fgcolor;	//-1��ʾ����Ϸ����ǰ��ɫ
		const char* content;	//�ڲ�����ֵ��Ӧ��ͼ�Σ����ΪNULL����ֱ����ʾ�ڲ������ֵ��
	} BLOCK_DISPLAY_INFO;*/


}

static void to_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi)
{
	if (bgi->inited == CGI_INITED) { //��ʼ����
		cct_setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //�ָ���ʼ��ɫ

		cct_gotoxy(0, bgi->SLI.lower_start_y + 2);//����赽ָ��λ��
		cout << setw(bgi->cols - 1) << ' '; //���ÿո��������

		cct_gotoxy(0, bgi->SLI.lower_start_y + 2);//����赽ָ��λ��
	}
	else { //δ��ʼ����
		cct_setcolor(); //ȱʡ��ɫ
		cct_gotoxy(0, 0);//����赽ָ��λ��
	}

	if (prompt)
		cout << prompt << "�����س�������...   ";
	else
		cout << "���س�������...   ";

	while (_getch() != '\r')
		;

	return;
}

