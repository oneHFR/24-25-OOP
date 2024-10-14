/* 2152131 �ƿ� ����� */
#pragma once

#define MAX_ROW 8
#define MAX_COL 11

void To_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi);

void input_parameter(CONSOLE_GRAPHICS_INFO* const pCGI, int* t);

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int mode);

int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi);

void move_and_merge_blocks(CONSOLE_GRAPHICS_INFO* const pCGI, int s[][MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int direction, int* check_change);

void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t);

int check(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);


