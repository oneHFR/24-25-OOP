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

int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi)
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
				/*
				  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
				   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
				   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
				   const int bdi_value						����Ҫ��ʾ��ֵ
				   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
				*/
				gmw_draw_block(pCGI, r_i, r_j, s[r_i][r_j], bdi);
				break;
			}
		}
		return 1;
	}
}

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int mode)
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
	if (mode == 1) {
		gmw_init(pCGI);
		gmw_set_font(pCGI, "������", 16);
		gmw_set_frame_style(pCGI, 8, 2, false);
		gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);
		gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);
	}

	else if (mode == 2) {
		gmw_set_color(pCGI, COLOR_BLACK, COLOR_WHITE);			//����������ɫ
		gmw_set_font(pCGI, "������", 16);						//����
		gmw_set_frame_style(pCGI, 10, 5, true);					//��Ϸ��������ÿ��ɫ���10��5���зָ��ߡ�����ɫ����߿򣬿��Ϊ10(�����6λ����)���߶�Ϊ5(Ϊ�˱���ɫ��Ϊ����)��
		gmw_set_frame_color(pCGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
		gmw_set_block_border_switch(pCGI, true);				//Сɫ����߿�
	}


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

// ͨ�õķ����ƶ��ͺϲ�����
 void move_and_merge_blocks(CONSOLE_GRAPHICS_INFO* const pCGI, int s[][MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int direction, int* check_change)
{
	int distance; // ��¼��������ƶ��ľ���

	if (direction == KB_ARROW_UP) {  // ����
		for (int j = 0; j < pCGI->col_num; j++) {
			distance = 0;
			for (int i = 0; i < pCGI->row_num; i++) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// �ƶ�����
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, DOWN_TO_UP, distance);
					s[i - distance][j] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// �ϲ�������ͬ�ķ���
					if (i - distance > 0 && s[i - distance][j] == s[i - distance - 1][j]) {
						s[i - distance - 1][j] *= 2;
						s[i - distance][j] = 0;
						gmw_draw_block(pCGI, i - distance, j, s[i - distance][j], bdi);
						gmw_draw_block(pCGI, i - distance - 1, j, s[i - distance - 1][j], bdi);
						distance++; // �ϲ������Ӿ���
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
	else if (direction == KB_ARROW_DOWN) {  // ����
		for (int j = 0; j < pCGI->col_num; j++) {
			distance = 0;
			for (int i = pCGI->row_num - 1; i >= 0; i--) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// �ƶ�����
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, UP_TO_DOWN, distance);
					s[i + distance][j] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// �ϲ�������ͬ�ķ���
					if (i + distance < pCGI->row_num - 1 && s[i + distance][j] == s[i + distance + 1][j]) {
						s[i + distance + 1][j] *= 2;
						s[i + distance][j] = 0;
						gmw_draw_block(pCGI, i + distance, j, s[i + distance][j], bdi);
						gmw_draw_block(pCGI, i + distance + 1, j, s[i + distance + 1][j], bdi);
						distance++;
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
	else if (direction == KB_ARROW_LEFT) {  // ����
		for (int i = 0; i < pCGI->row_num; i++) {
			distance = 0;
			for (int j = 0; j < pCGI->col_num; j++) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// �ƶ�����
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, RIGHT_TO_LEFT, distance);
					s[i][j - distance] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// �ϲ�������ͬ�ķ���
					if (j - distance > 0 && s[i][j - distance] == s[i][j - distance - 1]) {
						s[i][j - distance - 1] *= 2;
						s[i][j - distance] = 0;
						gmw_draw_block(pCGI, i, j - distance, s[i][j - distance], bdi);
						gmw_draw_block(pCGI, i, j - distance - 1, s[i][j - distance - 1], bdi);
						distance++;
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
	else if (direction == KB_ARROW_RIGHT) {  // ����
		for (int i = 0; i < pCGI->row_num; i++) {
			distance = 0;
			for (int j = pCGI->col_num - 1; j >= 0; j--) {
				if (s[i][j] == 0) {
					distance++;
				}
				else {
					// �ƶ�����
					gmw_move_block(pCGI, i, j, s[i][j], s[i][j], bdi, LEFT_TO_RIGHT, distance);
					s[i][j + distance] = s[i][j];
					if (distance > 0) s[i][j] = 0;

					// �ϲ�������ͬ�ķ���
					if (j + distance < pCGI->col_num - 1 && s[i][j + distance] == s[i][j + distance + 1]) {
						s[i][j + distance + 1] *= 2;
						s[i][j + distance] = 0;
						gmw_draw_block(pCGI, i, j + distance, s[i][j + distance], bdi);
						gmw_draw_block(pCGI, i, j + distance + 1, s[i][j + distance + 1], bdi);
						distance++;
					}
					*check_change = distance > 0 ? 1 : *check_change;
				}
			}
		}
	}
}





void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t)
{
	int loop = 1;
	int maction, mrow = -1, mcol = -1;
	int keycode1, keycode2;
	int ret;
	int score = 0;

	while (loop) {
		int distance = 0;
		int check_change = 0;
		cct_disable_mouse();
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);
		if (keycode1 == 0xe0) {
			// ���ݲ�ͬ�ķ������ͨ�ú�������
			switch (keycode2) {
				case KB_ARROW_UP:
					move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_UP, &check_change);
					break;

				case KB_ARROW_DOWN:
					move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_DOWN, &check_change);
					break;

				case KB_ARROW_LEFT:
					move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_LEFT, &check_change);
					break;

				case KB_ARROW_RIGHT:
					move_and_merge_blocks(pCGI, s, bdi, KB_ARROW_RIGHT, &check_change);
					break;
			}
		}

		// ����з����ƶ��������µ��������
		if (check_change) {
			new_one(pCGI, s, bdi);
		}

		// ����״̬����ʾ���ֵ
		for (int i = 0; i < pCGI->row_num; i++) {
			for (int j = 0; j < pCGI->col_num; j++) {
				score = s[i][j] > score ? s[i][j] : score;
			}
		}
		char temp[30];
		sprintf(temp, "Ŀǰ���ֵ��%d Ŀ��ֵ��%d", score, t);
		gmw_status_line(pCGI, 0, temp);

		loop = check(pCGI, s);

	}
}

int check(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL])
{
	// ����
	int d_x[4] = { 1, -1, 0, 0 };
	int d_y[4] = { 0, 0, 1, -1 };
	int num = 0; // �ǿ���

	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			if (s[i][j] != 0)
				num++;

			// �������
			for (int k = 0; k < 4; k++) {
				int s_i = i + d_x[k];
				int s_j = j + d_y[k];

				// ���߽�
				if (s_i >= 0 && s_i < pCGI->row_num && s_j >= 0 && s_j < pCGI->col_num) {
					//û������
					if (s[i][j] != 0 && s[i][j] == s[s_i][s_j])
						return 1;
				}
			}
		}
	}

	// ����
	if (num == pCGI->row_num * pCGI->col_num)
		return 0;
	else
		return 1;
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

