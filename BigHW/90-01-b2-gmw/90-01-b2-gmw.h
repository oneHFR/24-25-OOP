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

void input(CONSOLE_GRAPHICS_INFO* const pCGI, int min, int max);

void h9(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL]);

void fill(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int num);

void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);
