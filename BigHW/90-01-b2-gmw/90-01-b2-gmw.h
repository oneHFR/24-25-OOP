/* 2152131 �ƿ� ����� */
#pragma once

#define MAX_ROW 9
#define MAX_COL 11

/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�����״̬�� */
const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���������ʾ״̬�� */
const BLOCK_DISPLAY_INFO bdi_prompt[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���ը/����״̬�� */
const BLOCK_DISPLAY_INFO bdi_exploded[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};

/* ���������� */
void input(CONSOLE_GRAPHICS_INFO* const pCGI, int min, int max);
/* ��ʼ���������� */
void init(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL]);
/* ���Ƴ�ʼ��ɫ�� */
void fill(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int num);
/* ���䱻������ɫ�� */
void fill_new(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int num);
/*�Ƴ����ƿհ�ɫ�� */
void remove(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* ʵ������ĺϲ�������������Ϊ0 Ϊ��������Ч���ṩ��λ��Ϣ*/
void remove_zero(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* ��ʼ��������� */
void ini_h(int (*h)[MAX_COL]);
/* ����Ч�� */
void fall(const CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c);
/* ����Ƿ���ڿ�������ѡ��*/
int check(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* ����Ƿ����ͨ���Ϸ��ƶ�����������ɫ�� */
int hint(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* ��Ϸģʽ */
int game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);
/* �жϴ˴������Ƿ����1�ٳ�����*/
int game_swap(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int x31, int y31, int x32, int y32, int* SCORE);
/* �Ʒֺ��� */
int game_score(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int* SCORE);
/* ����main�ĺ��� ����tool�к�����ʹ�����˳��*/
void h9(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);