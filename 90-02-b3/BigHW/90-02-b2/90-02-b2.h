/* 2152131 计科 吴洪蕊 */
#pragma once

#define MAX_ROW 8
#define MAX_COL 11

const BLOCK_DISPLAY_INFO bdi[] = {
{BDI_VALUE_BLANK, -1, -1, NULL},  // 0空格
{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
{4, COLOR_HRED, COLOR_BLACK, NULL},
{8, COLOR_GREEN, COLOR_BLACK, NULL},
{16, COLOR_RED, COLOR_BLACK, NULL},
{32, COLOR_HGREEN, COLOR_BLACK, NULL},
{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
{128, COLOR_HRED, COLOR_BLACK, NULL},
{256, COLOR_HWHITE, COLOR_BLACK, NULL},
{512, COLOR_HBLACK, COLOR_BLACK, NULL},
{1024, COLOR_HPINK, COLOR_BLACK, NULL},
{2048, COLOR_WHITE, COLOR_BLACK, NULL},
{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
{BDI_VALUE_END, -1, -1, NULL} // NULL
};

/* 函数功能解释详见 tool.cpp*/
void To_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi);

void input_parameter(CONSOLE_GRAPHICS_INFO* const pCGI, int* t);

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int mode);

int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi);

void move_and_merge_blocks(CONSOLE_GRAPHICS_INFO* const pCGI, int s[][MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int direction, int* check_change);

void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t);

int check(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);


