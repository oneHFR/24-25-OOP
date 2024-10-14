/* 2152131 吴洪蕊 计科 */
#pragma once

#define MAX_ROW 9
#define MAX_COL 11

	 /* 定义1-5的数字用何种形式显示在界面上（正常状态） */
const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{0, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLUE, COLOR_BLACK, "★"},
	{2, COLOR_HGREEN, COLOR_BLACK, "★"},
	{3, COLOR_HCYAN, COLOR_BLACK, "★"},
	{4, COLOR_HRED, COLOR_BLACK, "★"},
	{5, COLOR_HPINK, COLOR_BLACK, "★"},
	{-999, -1, -1, NULL} //判断结束条件为-999
};

	/* 定义1-5的数字用何种形式显示在界面上（当前选择项状态+选中后关联项状态） */
const BLOCK_DISPLAY_INFO bdi_related[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLUE, COLOR_WHITE, "★"},
	{2, COLOR_HGREEN, COLOR_WHITE, "★"},
	{3, COLOR_HCYAN, COLOR_WHITE, "★"},
	{4, COLOR_HRED, COLOR_WHITE, "★"},
	{5, COLOR_HPINK, COLOR_WHITE, "★"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};

/* 定义1-5的数字用何种形式显示在界面上（选中状态） */
const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLUE, COLOR_HWHITE, "★"},
	{2, COLOR_HGREEN, COLOR_HWHITE, "★"},
	{3, COLOR_HCYAN, COLOR_HWHITE, "★"},
	{4, COLOR_HRED, COLOR_HWHITE, "★"},
	{5, COLOR_HPINK, COLOR_HWHITE, "★"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};

int H7(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL]);

void input(CONSOLE_GRAPHICS_INFO* const pCGI);

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL]);

void fill(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL]);

int Game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);