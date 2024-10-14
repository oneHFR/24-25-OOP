/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


/* --------------------------------------------------
		此处可以给出需要的内部辅助工具函数
		1、函数名不限，建议为 gmw_inner_* 
		2、个数不限
		3、必须是static函数，确保只在本源文件中使用
   -------------------------------------------------- */
/* 处理符合要求的2字节格式 */
static void gmw_inner_set_frame_linetyper(char* dest, const char* src); 
/* 处理默认边框 */
static void gmw_inner_simple_showstr(const char* str, int rpt, int maxlen);
/* 处理字符串的最后一个字符，如果是中文的第一个字节 表示有截断 */
static void gmw_inner_set_half_chinesechar(char* dest, const char* src, int restrict_len);
/* 根据给定的坐标x, y，绘制色块 */
static void gmw_inner_draw_block_at_position(const CONSOLE_GRAPHICS_INFO* const pCGI, const int x, const int y, const BLOCK_DISPLAY_INFO* pbdi, bool has_border, bool need_delay);
/* 将鼠标的XY坐标转换为游戏区域的行列号 */
static int gmw_inner_convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol);
/* 绘制移动的色块 */
static int gmw_inner_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int BX, const int BY, const int bdi_value_catchy, const BLOCK_DISPLAY_INFO* const bdi);

/* ----------------------------------------------- 
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */
/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
            2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = (row >= 0) ? row : 0;
	pCGI->col_num = (col >= 0) ? col : 0;

	/* 根据行列值更新高度和宽度 */
	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + (pCGI->row_num - 1) * pCGI->CFI.separator + 2;
	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + (pCGI->col_num - 1) * 2 * pCGI->CFI.separator + 4;

	/* 更新显示区域的总行列数 */
	//int lines;		
	// 为了给中文输入法提示行及运行结束的提示信息留空间，
	// 要求在计算得到的结果基础上（上额外空间+上状态栏+列标显示+主区域+下状态栏）
	// + 4（1中文输入法提示行+3预留空行）

	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;

	/* 状态栏的起始y坐标 */
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
				    前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* 窗口的bg&fg色 */
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;

	/* 同步修改游戏区和状态栏颜色 */
	if (cascade) {
		/* 窗口的bg&fg色 */
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;

		/* 上栏 */
		pCGI->SLI.top_normal_bgcolor = bgcolor;   
		pCGI->SLI.top_normal_fgcolor = fgcolor;   
		pCGI->SLI.top_catchy_bgcolor = bgcolor;    

		/* 下栏 */
		pCGI->SLI.lower_normal_bgcolor = bgcolor;  
		pCGI->SLI.lower_normal_fgcolor = fgcolor; 
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;  
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
            2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (strcmp(fontname, "Terminal") != 0 && strcmp(fontname, "新宋体") != 0)
		return -1;  
	strcpy(pCGI->CFT.font_type, fontname);
	pCGI->CFT.font_size_high = (fs_high <= 0) ? 16 : fs_high;
	pCGI->CFT.font_size_width = (fs_width <= 0) ? 8 : fs_width; 

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*  种类 
		#define DELAY_OF_DRAW_FRAME		0	//画游戏主框架时的延时
		#define DELAY_OF_DRAW_BLOCK		1	//画色块时的延时
		#define DELAY_OF_BLOCK_MOVED		2	//色块移动时的延时
	*/
	switch (type) {
		case DELAY_OF_DRAW_FRAME:  
			pCGI->delay_of_draw_frame = (delay_ms < 0) ? 0 : delay_ms;
			break;
		case DELAY_OF_DRAW_BLOCK:  
			pCGI->delay_of_draw_block = (delay_ms < 0) ? 0 : delay_ms;
			break;
		case DELAY_OF_BLOCK_MOVED:  
			pCGI->delay_of_block_moved = (delay_ms < BLOCK_MOVED_DELAY_MS) ? BLOCK_MOVED_DELAY_MS : delay_ms;
			break;
		default:
			return -1; 
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines			：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*
	整个图形界面的上下左右需要的额外行列数 
		int extern_up_lines;
		int extern_down_lines;
		int extern_left_cols;
		int extern_right_cols;
	*/
	pCGI->extern_up_lines = (up_lines < 0) ? 0 : up_lines;
	pCGI->extern_down_lines = (down_lines < 0) ? 0 : down_lines;
	pCGI->extern_left_cols = (left_cols < 0) ? 0 : left_cols;
	pCGI->extern_right_cols = (right_cols < 0) ? 0 : right_cols;

	/* 更新游戏框架的相关参数 */
		/* 整个图形界面的主框架区域的参考坐标起始位置(左上角）
	   - 注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的 */
	//int start_x;
	//int start_y;
	pCGI->start_x = pCGI->extern_left_cols;  
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line;  

	/* 计算总行数和列数，包括上下左右额外区域和状态栏 */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh +
							pCGI->SLI.is_top_status_line + pCGI->SLI.is_lower_status_line +
							pCGI->draw_frame_with_col_no + 4;  // 4 行用于额外的UI预留 flag
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth +
							pCGI->draw_frame_with_row_no * 2 + 1;  // 列数包含左右额外区域和边框

	/* 更新主框架内的块的高度和宽度 */
	// int lines;		
	// 为了给中文输入法提示行及运行结束的提示信息留空间，
	// 要求在计算得到的结果基础上（上额外空间+上状态栏+列标显示+主区域+下状态栏）
	// + 4（1中文输入法提示行+3预留空行）

	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num +
									pCGI->CFI.separator * (pCGI->row_num - 1) + 2;
	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num +
										pCGI->CFI.separator * 2 * (pCGI->col_num - 1) + 4;

	/* 状态栏的起始位置 */
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*
		char top_left[CFI_LEN];			// "╔"
		char lower_left[CFI_LEN];		// "╚"
		char top_right[CFI_LEN];			// "╗"
		char lower_right[CFI_LEN];		// "╝"
		char h_normal[CFI_LEN];			// "═"	//Horizontal
		char v_normal[CFI_LEN];			// "║"	//Vertical
		char h_top_separator[CFI_LEN];	// "╤"
		char h_lower_separator[CFI_LEN];	// "╧"
		char v_left_separator[CFI_LEN];	// "╟"
		char v_right_separator[CFI_LEN];	// "╢"
		char mid_separator[CFI_LEN];		// "┼"
	*/
	
	switch (type) {
		case 1: // 全双线
			strcpy(pCGI->CFI.top_left, "╔");
			strcpy(pCGI->CFI.lower_left, "╚");
			strcpy(pCGI->CFI.top_right, "╗");
			strcpy(pCGI->CFI.lower_right, "╝");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╦");
			strcpy(pCGI->CFI.h_lower_separator, "╩");
			strcpy(pCGI->CFI.v_left_separator, "╠");
			strcpy(pCGI->CFI.v_right_separator, "╣");
			strcpy(pCGI->CFI.mid_separator, "╬");
			break;
		case 2: // 全单线
			strcpy(pCGI->CFI.top_left, "┌");
			strcpy(pCGI->CFI.lower_left, "└");
			strcpy(pCGI->CFI.top_right, "┐");
			strcpy(pCGI->CFI.lower_right, "┘");
			strcpy(pCGI->CFI.h_normal, "─");
			strcpy(pCGI->CFI.v_normal, "│");
			strcpy(pCGI->CFI.h_top_separator, "┬");
			strcpy(pCGI->CFI.h_lower_separator, "┴");
			strcpy(pCGI->CFI.v_left_separator, "├");
			strcpy(pCGI->CFI.v_right_separator, "┤");
			strcpy(pCGI->CFI.mid_separator, "┼");
			break;
		case 3: // 横双竖单
			strcpy(pCGI->CFI.top_left, "╒"); 
			strcpy(pCGI->CFI.lower_left, "╘");
			strcpy(pCGI->CFI.top_right, "╕");
			strcpy(pCGI->CFI.lower_right, "╛");
			strcpy(pCGI->CFI.h_normal, "═");
			strcpy(pCGI->CFI.v_normal, "│");
			strcpy(pCGI->CFI.h_top_separator, "╤");
			strcpy(pCGI->CFI.h_lower_separator, "╧");
			strcpy(pCGI->CFI.v_left_separator, "╞");
			strcpy(pCGI->CFI.v_right_separator, "╡");
			strcpy(pCGI->CFI.mid_separator, "╪");
			break;
		case 4: // 横单竖双
			strcpy(pCGI->CFI.top_left, "╓");
			strcpy(pCGI->CFI.lower_left, "╙");
			strcpy(pCGI->CFI.top_right, "╖");
			strcpy(pCGI->CFI.lower_right, "╜");
			strcpy(pCGI->CFI.h_normal, "─");
			strcpy(pCGI->CFI.v_normal, "║");
			strcpy(pCGI->CFI.h_top_separator, "╥");
			strcpy(pCGI->CFI.h_lower_separator, "╨");
			strcpy(pCGI->CFI.v_left_separator, "╟");
			strcpy(pCGI->CFI.v_right_separator, "╢");
			strcpy(pCGI->CFI.mid_separator, "╫");
			break;
		default:
			return -1;
	}

	if (type == 1 || type == 2 || type == 3 || type == 4)
		return 1; //此句可根据需要修改
	else
		return 0;
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
static void gmw_inner_set_frame_linetyper(char* dest, const char* src)
{
	if (src == NULL) {
		// 如果传入 NULL，用两个空格替代
		strcpy(dest, "  ");
	}
	else {
		int src_len = strlen(src);

		// 如果是1字节字符，补一个空格
		if (src_len == 1) {
			dest[0] = src[0];
			dest[1] = ' ';
			dest[2] = '\0';
		}
		// 如果恰好是2字节，直接复制
		else if (src_len == 2) {
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = '\0';
		}
		// 如果超过2字节，截取前2字节
		else {
			strncpy(dest, src, 2);
			dest[2] = '\0';
		}
	}
}
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	gmw_inner_set_frame_linetyper(pCGI->CFI.top_left, top_left);
	gmw_inner_set_frame_linetyper(pCGI->CFI.lower_left, lower_left);
	gmw_inner_set_frame_linetyper(pCGI->CFI.top_right, top_right);
	gmw_inner_set_frame_linetyper(pCGI->CFI.lower_right, lower_right);
	gmw_inner_set_frame_linetyper(pCGI->CFI.h_normal, h_normal);
	gmw_inner_set_frame_linetyper(pCGI->CFI.v_normal, v_normal);
	gmw_inner_set_frame_linetyper(pCGI->CFI.h_top_separator, h_top_separator);
	gmw_inner_set_frame_linetyper(pCGI->CFI.h_lower_separator, h_lower_separator);
	gmw_inner_set_frame_linetyper(pCGI->CFI.v_left_separator, v_left_separator);
	gmw_inner_set_frame_linetyper(pCGI->CFI.v_right_separator, v_right_separator);
	gmw_inner_set_frame_linetyper(pCGI->CFI.mid_separator, mid_separator);

	return 1; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high			   			    ：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.separator = separator;

	/* 宽度调整为2或者 + 1  高度调整*/
	pCGI->CFI.block_width = (block_width < 0) ? 2 : (block_width % 2 == 1) ? block_width + 1 : block_width;
	pCGI->CFI.block_high = (block_high <= 0) ? 1 : block_high;

	/*重新计算*/
	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + pCGI->CFI.separator * (pCGI->row_num - 1) + 2;
	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + pCGI->CFI.separator * 2 * (pCGI->col_num - 1) + 4;

	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh
							+ pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth
						+ pCGI->draw_frame_with_row_no * 2 + 1;

	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*
		整个图形界面（含主框架、上下状态栏、行号列标、附加行列）的背景色和前景色 
		int area_bgcolor;
		int area_fgcolor;
	*/
	pCGI->CFI.bgcolor = bgcolor < 0 ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = fgcolor < 0 ? pCGI->area_fgcolor : fgcolor;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case 1: // 全双线
			strcpy(pCGI->CBI.top_left, "╔");     
			strcpy(pCGI->CBI.lower_left, "╚");   
			strcpy(pCGI->CBI.top_right, "╗");   
			strcpy(pCGI->CBI.lower_right, "╝");  
			strcpy(pCGI->CBI.h_normal, "═");     
			strcpy(pCGI->CBI.v_normal, "║");     
			break;
		case 2: // 2 - 全单线
			strcpy(pCGI->CBI.top_left, "┏");
			strcpy(pCGI->CBI.lower_left, "┗");
			strcpy(pCGI->CBI.top_right, "┓");
			strcpy(pCGI->CBI.lower_right, "┛");
			strcpy(pCGI->CBI.h_normal, "━");
			strcpy(pCGI->CBI.v_normal, "┃");
			break;
		case 3: // 3 - 横双竖单
			strcpy(pCGI->CBI.top_left, "╒");
			strcpy(pCGI->CBI.lower_left, "╘");
			strcpy(pCGI->CBI.top_right, "╕");
			strcpy(pCGI->CBI.lower_right, "╛");
			strcpy(pCGI->CBI.h_normal, "═");
			strcpy(pCGI->CBI.v_normal, "│");
			break;
		case 4: //  4 - 横单竖双
			strcpy(pCGI->CBI.top_left, "╓");
			strcpy(pCGI->CBI.lower_left, "╙");
			strcpy(pCGI->CBI.top_right, "╖");
			strcpy(pCGI->CBI.lower_right, "╜");
			strcpy(pCGI->CBI.h_normal, "─");
			strcpy(pCGI->CBI.v_normal, "║");
			break;
		default:
			return -1;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	gmw_inner_set_frame_linetyper(pCGI->CBI.top_left, top_left);
	gmw_inner_set_frame_linetyper(pCGI->CBI.lower_left, lower_left);
	gmw_inner_set_frame_linetyper(pCGI->CBI.top_right, top_right);
	gmw_inner_set_frame_linetyper(pCGI->CBI.lower_right, lower_right);
	gmw_inner_set_frame_linetyper(pCGI->CBI.h_normal, h_normal);
	gmw_inner_set_frame_linetyper(pCGI->CBI.v_normal, v_normal);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*
	定义状态栏信息 
		#define TOP_STATUS_LINE			    0	 //上状态栏，位置固定在游戏主框架区域的最上方（如果有列标，则在列标之上）
		#define LOWER_STATUS_LINE		1   //上状态栏，位置固定在游戏主框架区域的最下方
	*/
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.is_top_status_line = on_off;  
		pCGI->top_status_line = on_off;  

		/* 顶部起始位置 */
		pCGI->start_y = pCGI->extern_up_lines + (on_off ? 1 : 0);  
		pCGI->SLI.top_start_x = pCGI->start_x;
		pCGI->SLI.top_start_y = pCGI->start_y - 1;  
		/* 底部起始位置 */
		pCGI->SLI.lower_start_x = pCGI->start_x;
		pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;
	}

	else if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.is_lower_status_line = on_off;  
		pCGI->lower_status_line = on_off;  
	}

	else {
		return -1; 
	}

	/* 重新布局 */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines +
							pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh +
							pCGI->SLI.is_top_status_line + pCGI->SLI.is_lower_status_line + 4;

	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols +
							pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.bwidth + 1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/* 顶部 */
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor < 0 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor < 0 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor < 0 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor < 0 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	/* 底部 */
	else if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor < 0 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor < 0 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor < 0 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor < 0 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	else {
		return -1;  // 无效的状态栏类型，返回错误
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
            2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_row_no = on_off;

	/* 重新布局 */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines +
							pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh +
							pCGI->top_status_line + pCGI->lower_status_line + 4;

	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols +
							(pCGI->draw_frame_with_row_no ? 2 : 0) +  /* flag 行号需要额外两列 flag*/
							pCGI->CFI.bwidth + 1;

	/* 底部起始位置 */
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
            2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_col_no = on_off;

	/* 重新布局 */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines +
							pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line +
							pCGI->SLI.is_lower_status_line +
							(on_off ? 1 : 0) + 4;  /* 列标增加1行flag */

	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols +
							pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.bwidth + 1;

	/* 底部起始位置 */
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + (on_off ? 1 : 0);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
            2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记 */
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
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
static void gmw_inner_simple_showstr(const char* str, int rpt, int maxlen)
{
	const char* p;
	int i, rpt_count = 0;

	/* 首先考虑str==NULL / str="" 的情况
	   1、如果maxlen是-1/0，则直接返回，什么都不打印
	   2、如果maxlen>0，则用maxlen个空格填充 */
	if (str == NULL || str[0] == 0) {
		for (i = 0; i < maxlen; i++) //如果maxlen是-1、0，循环不执行，直接返回
			putchar(' ');
		return;
	}

	/* 之行到此，是str非NULL/str!=""的情况(既strlen一定>0) */
	if (rpt <= 0)
		rpt = 1; //防止错误参数

	if (maxlen < 0)
		maxlen = strlen(str) * rpt; //未给出maxlen则为原始长度

	/*	双线框架："╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬"
		单线框架："┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋"
		横双竖单: "╒", "╘", "╕", "╛", "═", "│", "╤", "╧", "╞", "╡", "╪"
		横单竖双："╓", "╙", "╖", "╜", "─", "║", "╥", "╨", "╟", "╢", "╫"
		*/
	const char* special[] = { "╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬",
							"┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋",
							"╒", "╘", "╕", "╛", "═", "│", "╤", "╧", "╞", "╡", "╪",
							"╓", "╙", "╖", "╜", "─", "║", "╥", "╨", "╟", "╢", "╫",
							"┌",  "┬", "└", "┐", "┘", "┴", "├", "┤", "┼", "─", "│",

							NULL
	};
	/* 没有用效率最高的方法，用了比较容易读懂的方法 */
	for (i = 0, p = str; i < maxlen; i++, p++) {	//重复rpt次，每次输出字符串，适用于在画边框时输出若干个"═"等情况
		if (*p == 0) {
			p = str; //如果p已经到\0，则回到头（此处已保证strlen(str)>0，即一定有内容）
			rpt_count++;
		}

		if (rpt_count >= rpt) { //如果超过了rpt次数则用空格填充
			putchar(' ');
			continue;
		}

		/* 如果未到字符串尾部且不是最后一个字符，则判断是否边框线 */
		if ((*p != '\0') && (*(p + 1) != '\0')) {
			bool found = false;
			int sp_no;
			for (sp_no = 0; special[sp_no] != NULL; sp_no++)
				if (strncmp(p, special[sp_no], strlen(special[sp_no])) == 0) {
					found = true;
					break;
				}

			/* 判断是否边框 */
			if (found) {
				/* 是边框线 */
				putchar(*p);
				++i; //多加一次
				++p; //多加一次
				putchar(*p);
				putchar(' '); //多补一个空格
			}
			else { //不是边框
				putchar(*p);
			}
		}//end of if
		else if ((*p != '\0') && (*(p + 1) == '\0')) {
			/* 如果是最后一个字符 */
			putchar(*p);
		}
	}//end of for
}

int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high);
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	cct_setcolor((*pCGI).area_bgcolor, (*pCGI).area_fgcolor);
	cct_cls();

	//列标
	if (pCGI->draw_frame_with_col_no) {
		cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.block_width / 2 + 1, pCGI->extern_up_lines + pCGI->SLI.is_top_status_line);
		for (int j = 0; j < pCGI->col_num; j++)
			if (j >= 100)
				cout << setw(pCGI->CFI.block_width + 2 * pCGI->CFI.separator) << setiosflags(ios::left) << '*';
			else
				cout << setw(pCGI->CFI.block_width + 2 * pCGI->CFI.separator) << setiosflags(ios::left) << j;
	}

	//首行
	cct_setcolor((*pCGI).CFI.bgcolor, (*pCGI).CFI.fgcolor);
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line);
	//cout << pCGI->CFI.top_left;
	gmw_inner_simple_showstr(pCGI->CFI.top_left, 1, strlen(pCGI->CFI.top_left));  // 输出左上角字符
	for (int j = 0; j < pCGI->col_num; j++) {
		for (int c = 0; c < pCGI->CFI.block_width; c += 2)
			//cout << pCGI->CFI.h_normal;
			gmw_inner_simple_showstr(pCGI->CFI.h_normal, 1, strlen(pCGI->CFI.h_normal));  // 输出横线
		if (pCGI->CFI.separator)
			//cout << pCGI->CFI.h_top_separator;
			gmw_inner_simple_showstr(pCGI->CFI.h_top_separator, 1, strlen(pCGI->CFI.h_top_separator));    // 输出分隔符
		Sleep(pCGI->delay_of_draw_frame);
	}
	if (pCGI->CFI.separator)
		//	cout << "\b\b";
		//cout << pCGI->CFI.top_right << endl;
		gmw_inner_simple_showstr("\b\b", 1, 2);
	gmw_inner_simple_showstr(pCGI->CFI.top_right, 1, strlen(pCGI->CFI.top_right));  // 输出右上角字符
	Sleep(pCGI->delay_of_draw_frame);


	//中间行
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int r = 0; r < pCGI->CFI.block_high; r++) {
			//行标
			cct_setcolor((*pCGI).area_bgcolor, (*pCGI).area_fgcolor);
			cct_gotoxy(pCGI->extern_left_cols, r + 1 + (i * (pCGI->CFI.separator + pCGI->CFI.block_high)) + pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no);
			if (pCGI->draw_frame_with_row_no && r == (pCGI->CFI.block_high - 1) / 2)
				if (i < 52)
					cout << (i >= 26 ? (char)('a' + i - 26) : (char)('A' + i));
				else
					cout << '*';
			cct_setcolor((*pCGI).CFI.bgcolor, (*pCGI).CFI.fgcolor);
			Sleep(pCGI->delay_of_draw_frame);

			//|   |
			cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, r + 1 + (i * (pCGI->CFI.separator + pCGI->CFI.block_high)) + pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no);
			//cout << pCGI->CFI.v_normal;
			gmw_inner_simple_showstr(pCGI->CFI.v_normal, 1, strlen(pCGI->CFI.v_normal));  // 输出左边框
			for (int j = 0; j < pCGI->col_num; j++) {
				for (int c = 0; c < pCGI->CFI.block_width; c += 2)
					cout << "  ";
				if (pCGI->CFI.separator)
					//cout << pCGI->CFI.v_normal;
					gmw_inner_simple_showstr(pCGI->CFI.v_normal, 1, strlen(pCGI->CFI.v_normal));  // 输出分隔符
				Sleep(pCGI->delay_of_draw_frame);
			}
			if (!pCGI->CFI.separator)
				//cout << pCGI->CFI.v_normal;
				gmw_inner_simple_showstr(pCGI->CFI.v_normal, 1, strlen(pCGI->CFI.v_normal));  // 输出右边框
			Sleep(pCGI->delay_of_draw_frame);
		}

		//+------+
		if (pCGI->CFI.separator) {
			cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + 1 + (i + 1) * pCGI->CFI.block_high + i * pCGI->CFI.separator);
			//cout << pCGI->CFI.v_left_separator;
			gmw_inner_simple_showstr(pCGI->CFI.v_left_separator, 1, strlen(pCGI->CFI.v_left_separator));  // 输出左分隔符
			for (int j = 0; j < pCGI->col_num; j++) {
				for (int c = 0; c < pCGI->CFI.block_width; c += 2)
					//	cout << pCGI->CFI.h_normal;
					//cout << pCGI->CFI.mid_separator;
					gmw_inner_simple_showstr(pCGI->CFI.h_normal, 1, strlen(pCGI->CFI.h_normal));  // 输出水平线
				gmw_inner_simple_showstr(pCGI->CFI.mid_separator, 1, strlen(pCGI->CFI.mid_separator));  // 输出中间分隔符
				Sleep(pCGI->delay_of_draw_frame);
			}
			//cout << "\b\b" << pCGI->CFI.v_right_separator;
			gmw_inner_simple_showstr("\b\b", 1, 2);  // 回退光标两格
			gmw_inner_simple_showstr(pCGI->CFI.v_right_separator, 1, strlen(pCGI->CFI.v_right_separator));   // 输出右分隔符
			Sleep(pCGI->delay_of_draw_frame);
		}
	}



	//尾行
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh - 1);
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh - 1);
	gmw_inner_simple_showstr(pCGI->CFI.lower_left, 1, strlen(pCGI->CFI.lower_left));  // 输出左下角字符
	for (int j = 0; j < pCGI->col_num; j++) {
		for (int c = 0; c < pCGI->CFI.block_width; c += 2)
			gmw_inner_simple_showstr(pCGI->CFI.h_normal, 1, strlen(pCGI->CFI.h_normal));  // 输出横线
		if (pCGI->CFI.separator)
			gmw_inner_simple_showstr(pCGI->CFI.h_lower_separator, 1, strlen(pCGI->CFI.h_lower_separator));  // 输出底部分隔符
	}
	if (pCGI->CFI.separator)
		//cout << "\b\b";
		gmw_inner_simple_showstr("\b\b", 1, 2);
	//cout << pCGI->CFI.lower_right << endl;
	gmw_inner_simple_showstr(pCGI->CFI.lower_right, 1, strlen(pCGI->CFI.lower_right));  // 输出右下角字符
	Sleep(pCGI->delay_of_draw_frame);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
            2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
static void gmw_inner_set_half_chinesechar(char* dest, const char* src, int restrict_len)
{  // flag
	// NULL
	if (src == NULL) {
		strcpy(dest, "");
		return;
	}

	// 拷贝并截断
	strncpy(dest, src, restrict_len);
	dest[restrict_len] = '\0'; // 确保字符串结尾

	// 判断最后一个字符是否为中文的第一个字节
	char* last = dest + strlen(dest) - 1; // 指向最后一个字符
	if ((unsigned char)*last > 127) { // 如果是汉字
		char* ps = dest;
		while (ps < last) {
			if ((unsigned char)*ps > 127)
				ps += 2; 
			else
				ps += 1;
		}
		if (ps > last + 1) {
			*last = '\0';
		}
	}
}

int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	/*flag*/
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE && pCGI->SLI.is_top_status_line == false)
		return 0;
	if (type == LOWER_STATUS_LINE && pCGI->SLI.is_lower_status_line == false)
		return 0;

	/* 宽度限制 */
	int restrict_width = 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.bwidth; 
	/*hh现学现用*/
	char* normal_msg = new(nothrow) char[restrict_width + 1]();
	char* catchy_display = new(nothrow) char[restrict_width + 1]();

	if (normal_msg == NULL || catchy_display == NULL) {
		if (normal_msg) 
			delete[] normal_msg;
		if (catchy_display) 
			delete[] catchy_display;
		return -1;  // 检查
	}

	gmw_inner_set_half_chinesechar(normal_msg, msg, restrict_width);
	gmw_inner_set_half_chinesechar(catchy_display, catchy_msg, restrict_width);

	/* 根据类型输出到相应的状态栏 */
	if (type == TOP_STATUS_LINE) {
		cct_gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
		cct_setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
		cout << catchy_display;
		cct_setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		cout << normal_msg;
		if (int(strlen(normal_msg) + strlen(catchy_display)) < restrict_width && !pCGI->SLI.top_start_x)
			cout << setw(restrict_width - strlen(normal_msg) - strlen(catchy_display)) << " ";
	}
	else if (type == LOWER_STATUS_LINE) {
		cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		cct_setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
		cout << catchy_display;
		cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		cout << normal_msg;
		if (int(strlen(normal_msg) + strlen(catchy_display)) < restrict_width && !pCGI->SLI.top_start_x)
			cout << setw(restrict_width - strlen(normal_msg) - strlen(catchy_display)) << " ";
	}

	delete[] normal_msg;
	delete[] catchy_display;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
            2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
static void gmw_inner_draw_block_at_position(const CONSOLE_GRAPHICS_INFO* const pCGI, const int x, const int y, const BLOCK_DISPLAY_INFO* pbdi, bool has_border, bool need_delay)
{
	int block_fg_color;
	int block_bg_color;

	if (pbdi->value == BDI_VALUE_BLANK) {
		block_bg_color = (pbdi->bgcolor == -1) ? pCGI->CFI.bgcolor : pbdi->bgcolor;
		block_fg_color = block_bg_color;
	}
	else {
		block_bg_color = (pbdi->bgcolor == -1) ? pCGI->CFI.bgcolor : pbdi->bgcolor;
		block_fg_color = (pbdi->fgcolor == -1) ? pCGI->CFI.fgcolor : pbdi->fgcolor;
	}

	cct_setcolor(block_bg_color, block_fg_color);

	for (int i = 0; i < pCGI->CFI.block_high; i++) {
		cct_gotoxy(x, y + i);
		for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
			// 绘制边框
			if (has_border) {
				if (i == 0) {
					if (j == 0)
						gmw_inner_simple_showstr(pCGI->CBI.top_left, 1, strlen(pCGI->CBI.top_left));  // 左上角
					else if (j == pCGI->CFI.block_width / 2 - 1)
						gmw_inner_simple_showstr(pCGI->CBI.top_right, 1, strlen(pCGI->CBI.top_right));  // 右上角
					else
						gmw_inner_simple_showstr(pCGI->CBI.h_normal, 1, strlen(pCGI->CBI.h_normal));  // 水平边框
				}
				else if (i == pCGI->CFI.block_high - 1) {
					if (j == 0)
						gmw_inner_simple_showstr(pCGI->CBI.lower_left, 1, strlen(pCGI->CBI.lower_left));  // 左下角
					else if (j == pCGI->CFI.block_width / 2 - 1)
						gmw_inner_simple_showstr(pCGI->CBI.lower_right, 1, strlen(pCGI->CBI.lower_right));  // 右下角
					else
						gmw_inner_simple_showstr(pCGI->CBI.h_normal, 1, strlen(pCGI->CBI.h_normal));  // 水平边框
				}
				else {
					if (j == 0 || j == pCGI->CFI.block_width / 2 - 1)
						gmw_inner_simple_showstr(pCGI->CBI.v_normal, 1, strlen(pCGI->CBI.v_normal));  // 左右边框
					else
						cout << "  ";
				}
			}
			else {
				cout << "  ";
			}
			if (need_delay)
				Sleep(pCGI->delay_of_draw_block);
		}
	}

	/* 显示内容 */
	int content_x = x + (pCGI->CFI.block_width - 1) / 2;
	int content_y = y + (pCGI->CFI.block_high - 1) / 2;
	if (pbdi->content == NULL && pbdi->value != BDI_VALUE_BLANK) {
		cct_gotoxy(content_x, content_y);
		cout << pbdi->value;
	}
	else if (pbdi->content != NULL) {
		cct_gotoxy(content_x, content_y);
		cout << pbdi->content;
	}
}

int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{

	if (pCGI->inited != CGI_INITED)
		return -1;

	const BLOCK_DISPLAY_INFO* pbdi = bdi;
	while (pbdi->value != bdi_value && pbdi->value != BDI_VALUE_END)
		pbdi++;

	// 起始坐标
	int block_start_x = pCGI->start_x + pCGI->draw_frame_with_row_no * 2 + 2 + col_no * (pCGI->CFI.block_width + pCGI->CFI.separator * 2);
	int block_start_y = pCGI->start_y + pCGI->draw_frame_with_col_no + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator);
	//int block_start_x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator); // 2
	//int block_start_y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator); // 1

	// 调用新的绘制函数，has_border 参数取决于 pCGI->CBI.block_border
	gmw_inner_draw_block_at_position(pCGI, block_start_x, block_start_y, pbdi, pCGI->CBI.block_border, true);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
static int gmw_inner_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int BX, const int BY, const int bdi_value_catchy, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* 查找匹配的色块 */
	for (int i = 0; (bdi + i)->value != BDI_VALUE_END; i++){
		if (bdi_value_catchy == (bdi + i)->value) {
			// 获取起始位置 + 颜色
			int x = BX;
			int y = BY;
			int bgcolor, fgcolor;
			bgcolor = (bdi + i)->bgcolor < 0 ? pCGI->CFI.bgcolor : (bdi + i)->bgcolor;
			fgcolor = (bdi + i)->fgcolor < 0 ? pCGI->CFI.fgcolor : (bdi + i)->fgcolor;

			if (pCGI->CBI.block_border == false) {  //无边框
				for (int row = 0; row < pCGI->CFI.block_high; row++) {
					for (int col = 0; col < pCGI->CFI.block_width / 2; col++) {
						if (row == pCGI->CFI.block_high / 2 && col == pCGI->CFI.block_width / 4) {
							if ((bdi + i)->content == NULL) {
								cct_showint(x + col * 2, y + row, (bdi + i)->value, bgcolor, fgcolor);
								if ((bdi + i)->value < 10)
									cct_showch(x + col * 2 + 1, y + row, ' ', bgcolor, fgcolor);
							}
							else
								cct_showstr(x + col * 2, y + row, (bdi + i)->content, bgcolor, fgcolor);
						}
						else
							cct_showstr(x + col * 2, y + row, "  ", bgcolor, fgcolor);
					}
				}
			}
			else {  //有边框
				for (int row = 0; row < pCGI->CFI.block_high; row++) {
					for (int col = 0; col < pCGI->CFI.block_width / 2; col++) {
						if (row == pCGI->CFI.block_high / 2 && col == pCGI->CFI.block_width / 4) {
							if ((bdi + i)->content == NULL) {
								cct_showint(x + col * 2, y + row, (bdi + i)->value, bgcolor, fgcolor);
								if ((bdi + i)->value < 10)
									cct_showch(x + col * 2 + 1, y + row, ' ', bgcolor, fgcolor);
							}
							else
								cct_showstr(x + col * 2, y + row, (bdi + i)->content, bgcolor, fgcolor);
						}
                        // 处理空白值的特殊情况
                        else if ((bdi + i)->value == BDI_VALUE_BLANK) {
                            cct_showstr(x + col * 2, y + row, "  ", bgcolor, fgcolor);
                        } 
                        // 绘制顶部边框
                        else if (row == 0) {
                            if (col == 0)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.top_left, bgcolor, fgcolor);
                            else if (col == pCGI->CFI.block_width / 2 - 1)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.top_right, bgcolor, fgcolor);
                            else
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.h_normal, bgcolor, fgcolor);
                        } 
                        // 绘制底部边框
                        else if (row == pCGI->CFI.block_high - 1) {
                            if (col == 0)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.lower_left, bgcolor, fgcolor);
                            else if (col == pCGI->CFI.block_width / 2 - 1)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.lower_right, bgcolor, fgcolor);
                            else
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.h_normal, bgcolor, fgcolor);
                        } 
						// 绘制左右边框
						else if (col == 0 || col == pCGI->CFI.block_width / 2 - 1)
							cct_showstr(x + col * 2, y + row, pCGI->CBI.v_normal, bgcolor, fgcolor);
						// 填充其余区域为空白
						else
							cct_showstr(x + col * 2, y + row, "  ", bgcolor, fgcolor);
					}
				}
			}
			return 0;
		}
	}

	return -1; //此句可根据需要修改
}

int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value_catchy, const int blank_bdi_value_catchy, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* 确保在初始化后才能调用其他函数 */
	if (pCGI->inited != CGI_INITED)
	return -1;

	// 计算起始色块的左上角位置
	int x = pCGI->start_x + pCGI->draw_frame_with_row_no * 2 + 2 + col_no * (pCGI->CFI.block_width + pCGI->CFI.separator * 2);
	int y = pCGI->start_y + pCGI->draw_frame_with_col_no + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator);

	// 获取背景和前景颜色
	int bgcolor = bdi->bgcolor < 0 ? pCGI->CFI.bgcolor : bdi->bgcolor;
	int fgcolor = bdi->fgcolor < 0 ? pCGI->CFI.fgcolor : bdi->fgcolor;

	// 处理不同方向的移动
	int total_steps = distance * (pCGI->CFI.block_width / 2 + pCGI->CFI.separator);

	// 根据不同的方向进行处理
	if (direction == UP_TO_DOWN) {
		// 从上到下移动
		for (int i = 0; i < total_steps; i++) {
			if ((i + 1) % (pCGI->CFI.block_width / 2 + pCGI->CFI.separator) == 0 && pCGI->CFI.separator) {
				for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
					cct_showstr(x + j * 2, y, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else { 
				for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
					cct_showstr(x + j * 2, y, "  ", bgcolor, fgcolor);
			}
			y += 1; // 更新y坐标（向下移动）
			gmw_inner_move_block(pCGI, x, y, bdi_value_catchy, bdi);
			Sleep(pCGI->delay_of_block_moved); // 延时
		}
	}

	else if (direction == DOWN_TO_UP) {
		// 从下到上移动
		for (int i = 0; i < total_steps; i++) {
			if ((i + 1) % (pCGI->CFI.block_width / 2 + pCGI->CFI.separator) == 0 && pCGI->CFI.separator) {
				for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
					cct_showstr(x + j * 2, y + pCGI->CFI.block_high - 1, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else {
				for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
					cct_showstr(x + j * 2, y + pCGI->CFI.block_high - 1, "  ", bgcolor, fgcolor);
			}
			y -= 1; 
			gmw_inner_move_block(pCGI, x, y, bdi_value_catchy, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
	}

	else if (direction == LEFT_TO_RIGHT) {
		// 从左到右移动
		for (int i = 0; i < total_steps; i++) {
			if ((i + 1) % (pCGI->CFI.block_width / 2 + pCGI->CFI.separator) == 0 && pCGI->CFI.separator) {
				for (int j = 0; j < pCGI->CFI.block_high; j++)
					cct_showstr(x, y + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else {
				for (int j = 0; j < pCGI->CFI.block_high; j++)
					cct_showstr(x, y + j, "  ", bgcolor, fgcolor);
			}
			x += 2;
			gmw_inner_move_block(pCGI, x, y, bdi_value_catchy, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
	}

	else if (direction == RIGHT_TO_LEFT) {
		// 从右到左移动
		for (int i = 0; i < total_steps; i++) {
			if ((i + 1) % (pCGI->CFI.block_width / 2 + pCGI->CFI.separator) == 0 && pCGI->CFI.separator) {
				for (int j = 0; j < pCGI->CFI.block_high; j++)
					cct_showstr(x + pCGI->CFI.block_width - 2, y + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else {
				for (int j = 0; j < pCGI->CFI.block_high; j++)
					cct_showstr(x + pCGI->CFI.block_width - 2, y + j, "  ", bgcolor, fgcolor);
			}
			x -= 2;
			gmw_inner_move_block(pCGI, x, y, bdi_value_catchy, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
	}
	else {
		return -1;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
		                                               如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
		                          且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值（因为消灭星星的右键标记需要坐标）
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
static int gmw_inner_convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol)
{

	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;

	MRow = MCol = -1024; // 初始化为非法位置

	// 遍历
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int j = 0; j < pCGI->col_num; j++) {
			int block_start_x = block_start_x0 + j * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
			int block_start_y = block_start_y0 + i * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);

			if (block_start_x <= MX && MX < block_start_x + pCGI->CFI.block_width &&
				block_start_y <= MY && MY < block_start_y + pCGI->CFI.block_high) {
				MRow = i;
				MCol = j;
				return 0; 
			}
		}
	}
	return -1; 
}

int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_enable_mouse();

	int MX = 0, MY = 0;  //记录鼠标位置
	int type;            //事件类型
	int MRow_new = MRow, MCol_new = MCol;    //行列新值
	int sign = 0, old_sign = 0;

	while (1) {
		type = cct_read_keyboard_and_mouse(MX, MY, MAction, KeyCode1, KeyCode2);
		if (type == CCT_KEYBOARD_EVENT)
			return type;
		else {
			//判断右键
			if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				MRow = MRow_new;
				MCol = MCol_new;
				return CCT_MOUSE_EVENT;
			}
			old_sign = sign;
			//判断鼠标位置是否合法
			sign = 0;
			if (MX < pCGI->start_x + pCGI->draw_frame_with_row_no * 2 + 2)
				sign = 1;
			if (MX >= pCGI->start_x + pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.bwidth - 2)
				sign = 1;
			if (MY < pCGI->start_y + pCGI->draw_frame_with_col_no + 1)
				sign = 1;
			if (MY >= pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh - 1)
				sign = 1;
			if (pCGI->CFI.separator) {
				if ((MX - pCGI->start_x - pCGI->draw_frame_with_row_no * 2 - 2) % (pCGI->CFI.block_width + 2) >= pCGI->CFI.block_width)
					sign = 1;
				if ((MY - pCGI->start_y - pCGI->draw_frame_with_col_no - 1) % (pCGI->CFI.block_high + 1) >= pCGI->CFI.block_high)
					sign = 1;
			}

			//判断左键
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				if (sign) {
					if (update_lower_status_line && !old_sign) {
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
					}
				}
				else {
					return CCT_MOUSE_EVENT;
				}
			}

			//剩下鼠标移动
			if (sign) {
				if (update_lower_status_line && !old_sign) {
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
				}
			}
			else {
				MCol_new = (MX - pCGI->start_x - pCGI->draw_frame_with_row_no * 2 - 2) / (pCGI->CFI.block_width + pCGI->CFI.separator * 2);
				MRow_new = (MY - pCGI->start_y - pCGI->draw_frame_with_col_no - 1) / (pCGI->CFI.block_high + pCGI->CFI.separator);
				if (MRow_new != MRow || MCol_new != MCol || old_sign) {
					if (update_lower_status_line) {
						char temp[256];
						sprintf(temp, "[当前光标] %c行%d列", char('A' + MRow_new), MCol_new);
						gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
					}
					MRow = MRow_new;
					MCol = MCol_new;
					return CCT_MOUSE_EVENT;
				}
			}
		}
	}

	return CCT_MOUSE_EVENT; //此句可根据需要修改
}
