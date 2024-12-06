/* 2152131 计科 吴洪蕊 */
#pragma once

#define MAX_ROW 9
#define MAX_COL 11

/* 定义1-9的数字用何种形式显示在界面上（正常状态） */
const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLACK, -1, "〇"},
	{2, COLOR_YELLOW, -1, "〇"},
	{3, COLOR_HGREEN, -1, "〇"},
	{4, COLOR_HCYAN, -1, "〇"},
	{5, COLOR_HRED, -1, "〇"},
	{6, COLOR_HPINK, -1, "〇"},
	{7, COLOR_HYELLOW, -1, "〇"},
	{8, COLOR_CYAN, -1, "〇"},
	{9, COLOR_WHITE, -1, "〇"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};
/* 定义1-9的数字用何种形式显示在界面上（选中状态） */
const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "●"},
	{2, COLOR_YELLOW, -1, "●"},
	{3, COLOR_HGREEN, -1, "●"},
	{4, COLOR_HCYAN, -1, "●"},
	{5, COLOR_HRED, -1, "●"},
	{6, COLOR_HPINK, -1, "●"},
	{7, COLOR_HYELLOW, -1, "●"},
	{8, COLOR_CYAN, -1, "●"},
	{9, COLOR_WHITE, -1, "●"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};
/* 定义1-9的数字用何种形式显示在界面上（可消除提示状态） */
const BLOCK_DISPLAY_INFO bdi_prompt[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "◎"},
	{2, COLOR_YELLOW, -1, "◎"},
	{3, COLOR_HGREEN, -1, "◎"},
	{4, COLOR_HCYAN, -1, "◎"},
	{5, COLOR_HRED, -1, "◎"},
	{6, COLOR_HPINK, -1, "◎"},
	{7, COLOR_HYELLOW, -1, "◎"},
	{8, COLOR_CYAN, -1, "◎"},
	{9, COLOR_WHITE, -1, "◎"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};
/* 定义1-9的数字用何种形式显示在界面上（爆炸/消除状态） */
const BLOCK_DISPLAY_INFO bdi_exploded[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "¤"},
	{2, COLOR_YELLOW, -1, "¤"},
	{3, COLOR_HGREEN, -1, "¤"},
	{4, COLOR_HCYAN, -1, "¤"},
	{5, COLOR_HRED, -1, "¤"},
	{6, COLOR_HPINK, -1, "¤"},
	{7, COLOR_HYELLOW, -1, "¤"},
	{8, COLOR_CYAN, -1, "¤"},
	{9, COLOR_WHITE, -1, "¤"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};

/* 输入行列数 */
void input(CONSOLE_GRAPHICS_INFO* const pCGI, int min, int max);
/* 初始化基本设置 */
void init(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL]);
/* 绘制初始的色块 */
void fill(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int num);
/* 补充被消除的色块 */
void fill_new(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int num);
/*移除绘制空白色块 */
void remove(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* 实际数组的合并消除的数字置为0 为后面下落效果提供定位信息*/
void remove_zero(int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* 初始化伴随矩阵 */
void ini_h(int (*h)[MAX_COL]);
/* 下落效果 */
void fall(const CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c);
/* 检查是否存在可消除的选项*/
int check(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* 检查是否存在通过合法移动可以消除的色块 */
int hint(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL]);
/* 游戏模式 */
int game(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);
/* 判断此处交换是否可以1促成消除*/
int game_swap(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int x31, int y31, int x32, int y32, int* SCORE);
/* 计分函数 */
int game_score(CONSOLE_GRAPHICS_INFO* const pCGI, int (*s)[MAX_COL], int r, int c, int (*h)[MAX_COL], int* SCORE);
/* 连接main的函数 包含tool中函数的使用组合顺序*/
void h9(CONSOLE_GRAPHICS_INFO* const pCGI, int(*s)[MAX_COL]);