/* 2152131 �ƿ� ����� */
#pragma once

#define MAX_ROW 8
#define MAX_COL 11

void input_parameter(CONSOLE_GRAPHICS_INFO* const pCGI, int* t);

void init(CONSOLE_GRAPHICS_INFO* const pCGI, int mode);

int new_one(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi);

void game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL], const BLOCK_DISPLAY_INFO* const bdi, int t);

int check(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);

static void to_be_continued(const char* prompt, const CONSOLE_GRAPHICS_INFO* const bgi);
