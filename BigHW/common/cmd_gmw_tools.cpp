/* 2152131 ����� �ƿ� */
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
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


/* --------------------------------------------------
		�˴����Ը�����Ҫ���ڲ��������ߺ���
		1�����������ޣ�����Ϊ gmw_inner_* 
		2����������
		3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
   -------------------------------------------------- */
/* �������Ҫ���2�ֽڸ�ʽ */
static void gmw_inner_set_frame_linetyper(char* dest, const char* src); 
/* ����Ĭ�ϱ߿� */
static void gmw_inner_simple_showstr(const char* str, int rpt, int maxlen);
/* �����ַ��������һ���ַ�����������ĵĵ�һ���ֽ� ��ʾ�нض� */
static void gmw_inner_set_half_chinesechar(char* dest, const char* src, int restrict_len);
/* ���ݸ���������x, y������ɫ�� */
static void gmw_inner_draw_block_at_position(const CONSOLE_GRAPHICS_INFO* const pCGI, const int x, const int y, const BLOCK_DISPLAY_INFO* pbdi, bool has_border, bool need_delay);
/* ������XY����ת��Ϊ��Ϸ��������к� */
static int gmw_inner_convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol);
/* �����ƶ���ɫ�� */
static int gmw_inner_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int BX, const int BY, const int bdi_value_catchy, const BLOCK_DISPLAY_INFO* const bdi);

/* ----------------------------------------------- 
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */
/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ����ܵ�������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
  �� �� ֵ��
  ˵    ����1��ָ��������Ϸ�ľ������������ֵ
            2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = (row >= 0) ? row : 0;
	pCGI->col_num = (col >= 0) ? col : 0;

	/* ��������ֵ���¸߶ȺͿ�� */
	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + (pCGI->row_num - 1) * pCGI->CFI.separator + 2;
	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + (pCGI->col_num - 1) * 2 * pCGI->CFI.separator + 4;

	/* ������ʾ������������� */
	//int lines;		
	// Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬
	// Ҫ���ڼ���õ��Ľ�������ϣ��϶���ռ�+��״̬��+�б���ʾ+������+��״̬����
	// + 4��1�������뷨��ʾ��+3Ԥ�����У�

	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line + pCGI->SLI.is_lower_status_line + pCGI->draw_frame_with_col_no + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth + pCGI->draw_frame_with_row_no * 2 + 1;

	/* ״̬������ʼy���� */
	pCGI->SLI.lower_start_y = pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
				    ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* ���ڵ�bg&fgɫ */
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;

	/* ͬ���޸���Ϸ����״̬����ɫ */
	if (cascade) {
		/* ���ڵ�bg&fgɫ */
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;

		/* ���� */
		pCGI->SLI.top_normal_bgcolor = bgcolor;   
		pCGI->SLI.top_normal_fgcolor = fgcolor;   
		pCGI->SLI.top_catchy_bgcolor = bgcolor;    

		/* ���� */
		pCGI->SLI.lower_normal_bgcolor = bgcolor;  
		pCGI->SLI.lower_normal_fgcolor = fgcolor; 
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;  
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
            2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (strcmp(fontname, "Terminal") != 0 && strcmp(fontname, "������") != 0)
		return -1;  
	strcpy(pCGI->CFT.font_type, fontname);
	pCGI->CFT.font_size_high = (fs_high <= 0) ? 16 : fs_high;
	pCGI->CFT.font_size_width = (fs_width <= 0) ? 8 : fs_width; 

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*  ���� 
		#define DELAY_OF_DRAW_FRAME		0	//����Ϸ�����ʱ����ʱ
		#define DELAY_OF_DRAW_BLOCK		1	//��ɫ��ʱ����ʱ
		#define DELAY_OF_BLOCK_MOVED		2	//ɫ���ƶ�ʱ����ʱ
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines			���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*
	����ͼ�ν��������������Ҫ�Ķ��������� 
		int extern_up_lines;
		int extern_down_lines;
		int extern_left_cols;
		int extern_right_cols;
	*/
	pCGI->extern_up_lines = (up_lines < 0) ? 0 : up_lines;
	pCGI->extern_down_lines = (down_lines < 0) ? 0 : down_lines;
	pCGI->extern_left_cols = (left_cols < 0) ? 0 : left_cols;
	pCGI->extern_right_cols = (right_cols < 0) ? 0 : right_cols;

	/* ������Ϸ��ܵ���ز��� */
		/* ����ͼ�ν�������������Ĳο�������ʼλ��(���Ͻǣ�
	   - ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ�� */
	//int start_x;
	//int start_y;
	pCGI->start_x = pCGI->extern_left_cols;  
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line;  

	/* �����������������������������Ҷ��������״̬�� */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh +
							pCGI->SLI.is_top_status_line + pCGI->SLI.is_lower_status_line +
							pCGI->draw_frame_with_col_no + 4;  // 4 �����ڶ����UIԤ�� flag
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth +
							pCGI->draw_frame_with_row_no * 2 + 1;  // �����������Ҷ�������ͱ߿�

	/* ����������ڵĿ�ĸ߶ȺͿ�� */
	// int lines;		
	// Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬
	// Ҫ���ڼ���õ��Ľ�������ϣ��϶���ռ�+��״̬��+�б���ʾ+������+��״̬����
	// + 4��1�������뷨��ʾ��+3Ԥ�����У�

	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num +
									pCGI->CFI.separator * (pCGI->row_num - 1) + 2;
	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num +
										pCGI->CFI.separator * 2 * (pCGI->col_num - 1) + 4;

	/* ״̬������ʼλ�� */
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;
	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*
		char top_left[CFI_LEN];			// "�X"
		char lower_left[CFI_LEN];		// "�^"
		char top_right[CFI_LEN];			// "�["
		char lower_right[CFI_LEN];		// "�a"
		char h_normal[CFI_LEN];			// "�T"	//Horizontal
		char v_normal[CFI_LEN];			// "�U"	//Vertical
		char h_top_separator[CFI_LEN];	// "�h"
		char h_lower_separator[CFI_LEN];	// "�k"
		char v_left_separator[CFI_LEN];	// "�c"
		char v_right_separator[CFI_LEN];	// "�f"
		char mid_separator[CFI_LEN];		// "��"
	*/
	
	switch (type) {
		case 1: // ȫ˫��
			strcpy(pCGI->CFI.top_left, "�X");
			strcpy(pCGI->CFI.lower_left, "�^");
			strcpy(pCGI->CFI.top_right, "�[");
			strcpy(pCGI->CFI.lower_right, "�a");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�j");
			strcpy(pCGI->CFI.h_lower_separator, "�m");
			strcpy(pCGI->CFI.v_left_separator, "�d");
			strcpy(pCGI->CFI.v_right_separator, "�g");
			strcpy(pCGI->CFI.mid_separator, "�p");
			break;
		case 2: // ȫ����
			strcpy(pCGI->CFI.top_left, "��");
			strcpy(pCGI->CFI.lower_left, "��");
			strcpy(pCGI->CFI.top_right, "��");
			strcpy(pCGI->CFI.lower_right, "��");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "��");
			strcpy(pCGI->CFI.h_lower_separator, "��");
			strcpy(pCGI->CFI.v_left_separator, "��");
			strcpy(pCGI->CFI.v_right_separator, "��");
			strcpy(pCGI->CFI.mid_separator, "��");
			break;
		case 3: // ��˫����
			strcpy(pCGI->CFI.top_left, "�V"); 
			strcpy(pCGI->CFI.lower_left, "�\");
			strcpy(pCGI->CFI.top_right, "�Y");
			strcpy(pCGI->CFI.lower_right, "�_");
			strcpy(pCGI->CFI.h_normal, "�T");
			strcpy(pCGI->CFI.v_normal, "��");
			strcpy(pCGI->CFI.h_top_separator, "�h");
			strcpy(pCGI->CFI.h_lower_separator, "�k");
			strcpy(pCGI->CFI.v_left_separator, "�b");
			strcpy(pCGI->CFI.v_right_separator, "�e");
			strcpy(pCGI->CFI.mid_separator, "�n");
			break;
		case 4: // �ᵥ��˫
			strcpy(pCGI->CFI.top_left, "�W");
			strcpy(pCGI->CFI.lower_left, "�]");
			strcpy(pCGI->CFI.top_right, "�Z");
			strcpy(pCGI->CFI.lower_right, "�`");
			strcpy(pCGI->CFI.h_normal, "��");
			strcpy(pCGI->CFI.v_normal, "�U");
			strcpy(pCGI->CFI.h_top_separator, "�i");
			strcpy(pCGI->CFI.h_lower_separator, "�l");
			strcpy(pCGI->CFI.v_left_separator, "�c");
			strcpy(pCGI->CFI.v_right_separator, "�f");
			strcpy(pCGI->CFI.mid_separator, "�o");
			break;
		default:
			return -1;
	}

	if (type == 1 || type == 2 || type == 3 || type == 4)
		return 1; //�˾�ɸ�����Ҫ�޸�
	else
		return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
static void gmw_inner_set_frame_linetyper(char* dest, const char* src)
{
	if (src == NULL) {
		// ������� NULL���������ո����
		strcpy(dest, "  ");
	}
	else {
		int src_len = strlen(src);

		// �����1�ֽ��ַ�����һ���ո�
		if (src_len == 1) {
			dest[0] = src[0];
			dest[1] = ' ';
			dest[2] = '\0';
		}
		// ���ǡ����2�ֽڣ�ֱ�Ӹ���
		else if (src_len == 2) {
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = '\0';
		}
		// �������2�ֽڣ���ȡǰ2�ֽ�
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
	/* ��ֹ��δ���� gmw_init ǰ������������ */
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

	return 1; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high			   			    ���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.separator = separator;

	/* ��ȵ���Ϊ2���� + 1  �߶ȵ���*/
	pCGI->CFI.block_width = (block_width < 0) ? 2 : (block_width % 2 == 1) ? block_width + 1 : block_width;
	pCGI->CFI.block_high = (block_high <= 0) ? 1 : block_high;

	/*���¼���*/
	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + pCGI->CFI.separator * (pCGI->row_num - 1) + 2;
	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + pCGI->CFI.separator * 2 * (pCGI->col_num - 1) + 4;

	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->CFI.bhigh
							+ pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->CFI.bwidth
						+ pCGI->draw_frame_with_row_no * 2 + 1;

	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*
		����ͼ�ν��棨������ܡ�����״̬�����к��бꡢ�������У��ı���ɫ��ǰ��ɫ 
		int area_bgcolor;
		int area_fgcolor;
	*/
	pCGI->CFI.bgcolor = bgcolor < 0 ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = fgcolor < 0 ? pCGI->area_fgcolor : fgcolor;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type) {
		case 1: // ȫ˫��
			strcpy(pCGI->CBI.top_left, "�X");     
			strcpy(pCGI->CBI.lower_left, "�^");   
			strcpy(pCGI->CBI.top_right, "�[");   
			strcpy(pCGI->CBI.lower_right, "�a");  
			strcpy(pCGI->CBI.h_normal, "�T");     
			strcpy(pCGI->CBI.v_normal, "�U");     
			break;
		case 2: // 2 - ȫ����
			strcpy(pCGI->CBI.top_left, "��");
			strcpy(pCGI->CBI.lower_left, "��");
			strcpy(pCGI->CBI.top_right, "��");
			strcpy(pCGI->CBI.lower_right, "��");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 3: // 3 - ��˫����
			strcpy(pCGI->CBI.top_left, "�V");
			strcpy(pCGI->CBI.lower_left, "�\");
			strcpy(pCGI->CBI.top_right, "�Y");
			strcpy(pCGI->CBI.lower_right, "�_");
			strcpy(pCGI->CBI.h_normal, "�T");
			strcpy(pCGI->CBI.v_normal, "��");
			break;
		case 4: //  4 - �ᵥ��˫
			strcpy(pCGI->CBI.top_left, "�W");
			strcpy(pCGI->CBI.lower_left, "�]");
			strcpy(pCGI->CBI.top_right, "�Z");
			strcpy(pCGI->CBI.lower_right, "�`");
			strcpy(pCGI->CBI.h_normal, "��");
			strcpy(pCGI->CBI.v_normal, "�U");
			break;
		default:
			return -1;
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	gmw_inner_set_frame_linetyper(pCGI->CBI.top_left, top_left);
	gmw_inner_set_frame_linetyper(pCGI->CBI.lower_left, lower_left);
	gmw_inner_set_frame_linetyper(pCGI->CBI.top_right, top_right);
	gmw_inner_set_frame_linetyper(pCGI->CBI.lower_right, lower_right);
	gmw_inner_set_frame_linetyper(pCGI->CBI.h_normal, h_normal);
	gmw_inner_set_frame_linetyper(pCGI->CBI.v_normal, v_normal);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	pCGI->CBI.block_border = on_off;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/*
	����״̬����Ϣ 
		#define TOP_STATUS_LINE			    0	 //��״̬����λ�ù̶�����Ϸ�������������Ϸ���������б꣬�����б�֮�ϣ�
		#define LOWER_STATUS_LINE		1   //��״̬����λ�ù̶�����Ϸ�������������·�
	*/
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.is_top_status_line = on_off;  
		pCGI->top_status_line = on_off;  

		/* ������ʼλ�� */
		pCGI->start_y = pCGI->extern_up_lines + (on_off ? 1 : 0);  
		pCGI->SLI.top_start_x = pCGI->start_x;
		pCGI->SLI.top_start_y = pCGI->start_y - 1;  
		/* �ײ���ʼλ�� */
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

	/* ���²��� */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines +
							pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh +
							pCGI->SLI.is_top_status_line + pCGI->SLI.is_lower_status_line + 4;

	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols +
							pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.bwidth + 1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/* ���� */
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor < 0 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor < 0 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor < 0 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor < 0 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	/* �ײ� */
	else if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor < 0 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor < 0 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor < 0 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor < 0 ? COLOR_HYELLOW : catchy_fgcolor;
	}
	else {
		return -1;  // ��Ч��״̬�����ͣ����ش���
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_row_no = on_off;

	/* ���²��� */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines +
							pCGI->draw_frame_with_col_no + pCGI->CFI.bhigh +
							pCGI->top_status_line + pCGI->lower_status_line + 4;

	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols +
							(pCGI->draw_frame_with_row_no ? 2 : 0) +  /* flag �к���Ҫ�������� flag*/
							pCGI->CFI.bwidth + 1;

	/* �ײ���ʼλ�� */
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + pCGI->draw_frame_with_col_no;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
            2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->draw_frame_with_col_no = on_off;

	/* ���²��� */
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines +
							pCGI->CFI.bhigh + pCGI->SLI.is_top_status_line +
							pCGI->SLI.is_lower_status_line +
							(on_off ? 1 : 0) + 4;  /* �б�����1��flag */

	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols +
							pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.bwidth + 1;

	/* �ײ���ʼλ�� */
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.bhigh + (on_off ? 1 : 0);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
            2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* �����ñ�� */
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

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
static void gmw_inner_simple_showstr(const char* str, int rpt, int maxlen)
{
	const char* p;
	int i, rpt_count = 0;

	/* ���ȿ���str==NULL / str="" �����
	   1�����maxlen��-1/0����ֱ�ӷ��أ�ʲô������ӡ
	   2�����maxlen>0������maxlen���ո���� */
	if (str == NULL || str[0] == 0) {
		for (i = 0; i < maxlen; i++) //���maxlen��-1��0��ѭ����ִ�У�ֱ�ӷ���
			putchar(' ');
		return;
	}

	/* ֮�е��ˣ���str��NULL/str!=""�����(��strlenһ��>0) */
	if (rpt <= 0)
		rpt = 1; //��ֹ�������

	if (maxlen < 0)
		maxlen = strlen(str) * rpt; //δ����maxlen��Ϊԭʼ����

	/*	˫�߿�ܣ�"�X", "�^", "�[", "�a", "�T", "�U", "�j", "�m", "�d", "�g", "�p"
		���߿�ܣ�"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"
		��˫����: "�V", "�\", "�Y", "�_", "�T", "��", "�h", "�k", "�b", "�e", "�n"
		�ᵥ��˫��"�W", "�]", "�Z", "�`", "��", "�U", "�i", "�l", "�c", "�f", "�o"
		*/
	const char* special[] = { "�X", "�^", "�[", "�a", "�T", "�U", "�j", "�m", "�d", "�g", "�p",
							"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
							"�V", "�\", "�Y", "�_", "�T", "��", "�h", "�k", "�b", "�e", "�n",
							"�W", "�]", "�Z", "�`", "��", "�U", "�i", "�l", "�c", "�f", "�o",
							"��",  "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",

							NULL
	};
	/* û����Ч����ߵķ��������˱Ƚ����׶����ķ��� */
	for (i = 0, p = str; i < maxlen; i++, p++) {	//�ظ�rpt�Σ�ÿ������ַ������������ڻ��߿�ʱ������ɸ�"�T"�����
		if (*p == 0) {
			p = str; //���p�Ѿ���\0����ص�ͷ���˴��ѱ�֤strlen(str)>0����һ�������ݣ�
			rpt_count++;
		}

		if (rpt_count >= rpt) { //���������rpt�������ÿո����
			putchar(' ');
			continue;
		}

		/* ���δ���ַ���β���Ҳ������һ���ַ������ж��Ƿ�߿��� */
		if ((*p != '\0') && (*(p + 1) != '\0')) {
			bool found = false;
			int sp_no;
			for (sp_no = 0; special[sp_no] != NULL; sp_no++)
				if (strncmp(p, special[sp_no], strlen(special[sp_no])) == 0) {
					found = true;
					break;
				}

			/* �ж��Ƿ�߿� */
			if (found) {
				/* �Ǳ߿��� */
				putchar(*p);
				++i; //���һ��
				++p; //���һ��
				putchar(*p);
				putchar(' '); //�ಹһ���ո�
			}
			else { //���Ǳ߿�
				putchar(*p);
			}
		}//end of if
		else if ((*p != '\0') && (*(p + 1) == '\0')) {
			/* ��������һ���ַ� */
			putchar(*p);
		}
	}//end of for
}

int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high);
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	cct_setcolor((*pCGI).area_bgcolor, (*pCGI).area_fgcolor);
	cct_cls();

	//�б�
	if (pCGI->draw_frame_with_col_no) {
		cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2 + pCGI->CFI.block_width / 2 + 1, pCGI->extern_up_lines + pCGI->SLI.is_top_status_line);
		for (int j = 0; j < pCGI->col_num; j++)
			if (j >= 100)
				cout << setw(pCGI->CFI.block_width + 2 * pCGI->CFI.separator) << setiosflags(ios::left) << '*';
			else
				cout << setw(pCGI->CFI.block_width + 2 * pCGI->CFI.separator) << setiosflags(ios::left) << j;
	}

	//����
	cct_setcolor((*pCGI).CFI.bgcolor, (*pCGI).CFI.fgcolor);
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line);
	//cout << pCGI->CFI.top_left;
	gmw_inner_simple_showstr(pCGI->CFI.top_left, 1, strlen(pCGI->CFI.top_left));  // ������Ͻ��ַ�
	for (int j = 0; j < pCGI->col_num; j++) {
		for (int c = 0; c < pCGI->CFI.block_width; c += 2)
			//cout << pCGI->CFI.h_normal;
			gmw_inner_simple_showstr(pCGI->CFI.h_normal, 1, strlen(pCGI->CFI.h_normal));  // �������
		if (pCGI->CFI.separator)
			//cout << pCGI->CFI.h_top_separator;
			gmw_inner_simple_showstr(pCGI->CFI.h_top_separator, 1, strlen(pCGI->CFI.h_top_separator));    // ����ָ���
		Sleep(pCGI->delay_of_draw_frame);
	}
	if (pCGI->CFI.separator)
		//	cout << "\b\b";
		//cout << pCGI->CFI.top_right << endl;
		gmw_inner_simple_showstr("\b\b", 1, 2);
	gmw_inner_simple_showstr(pCGI->CFI.top_right, 1, strlen(pCGI->CFI.top_right));  // ������Ͻ��ַ�
	Sleep(pCGI->delay_of_draw_frame);


	//�м���
	for (int i = 0; i < pCGI->row_num; i++) {
		for (int r = 0; r < pCGI->CFI.block_high; r++) {
			//�б�
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
			gmw_inner_simple_showstr(pCGI->CFI.v_normal, 1, strlen(pCGI->CFI.v_normal));  // �����߿�
			for (int j = 0; j < pCGI->col_num; j++) {
				for (int c = 0; c < pCGI->CFI.block_width; c += 2)
					cout << "  ";
				if (pCGI->CFI.separator)
					//cout << pCGI->CFI.v_normal;
					gmw_inner_simple_showstr(pCGI->CFI.v_normal, 1, strlen(pCGI->CFI.v_normal));  // ����ָ���
				Sleep(pCGI->delay_of_draw_frame);
			}
			if (!pCGI->CFI.separator)
				//cout << pCGI->CFI.v_normal;
				gmw_inner_simple_showstr(pCGI->CFI.v_normal, 1, strlen(pCGI->CFI.v_normal));  // ����ұ߿�
			Sleep(pCGI->delay_of_draw_frame);
		}

		//+------+
		if (pCGI->CFI.separator) {
			cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->SLI.is_top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + 1 + (i + 1) * pCGI->CFI.block_high + i * pCGI->CFI.separator);
			//cout << pCGI->CFI.v_left_separator;
			gmw_inner_simple_showstr(pCGI->CFI.v_left_separator, 1, strlen(pCGI->CFI.v_left_separator));  // �����ָ���
			for (int j = 0; j < pCGI->col_num; j++) {
				for (int c = 0; c < pCGI->CFI.block_width; c += 2)
					//	cout << pCGI->CFI.h_normal;
					//cout << pCGI->CFI.mid_separator;
					gmw_inner_simple_showstr(pCGI->CFI.h_normal, 1, strlen(pCGI->CFI.h_normal));  // ���ˮƽ��
				gmw_inner_simple_showstr(pCGI->CFI.mid_separator, 1, strlen(pCGI->CFI.mid_separator));  // ����м�ָ���
				Sleep(pCGI->delay_of_draw_frame);
			}
			//cout << "\b\b" << pCGI->CFI.v_right_separator;
			gmw_inner_simple_showstr("\b\b", 1, 2);  // ���˹������
			gmw_inner_simple_showstr(pCGI->CFI.v_right_separator, 1, strlen(pCGI->CFI.v_right_separator));   // ����ҷָ���
			Sleep(pCGI->delay_of_draw_frame);
		}
	}



	//β��
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh - 1);
	cct_gotoxy(pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2, pCGI->extern_up_lines + pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->CFI.bhigh - 1);
	gmw_inner_simple_showstr(pCGI->CFI.lower_left, 1, strlen(pCGI->CFI.lower_left));  // ������½��ַ�
	for (int j = 0; j < pCGI->col_num; j++) {
		for (int c = 0; c < pCGI->CFI.block_width; c += 2)
			gmw_inner_simple_showstr(pCGI->CFI.h_normal, 1, strlen(pCGI->CFI.h_normal));  // �������
		if (pCGI->CFI.separator)
			gmw_inner_simple_showstr(pCGI->CFI.h_lower_separator, 1, strlen(pCGI->CFI.h_lower_separator));  // ����ײ��ָ���
	}
	if (pCGI->CFI.separator)
		//cout << "\b\b";
		gmw_inner_simple_showstr("\b\b", 1, 2);
	//cout << pCGI->CFI.lower_right << endl;
	gmw_inner_simple_showstr(pCGI->CFI.lower_right, 1, strlen(pCGI->CFI.lower_right));  // ������½��ַ�
	Sleep(pCGI->delay_of_draw_frame);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
            2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
static void gmw_inner_set_half_chinesechar(char* dest, const char* src, int restrict_len)
{  // flag
	// NULL
	if (src == NULL) {
		strcpy(dest, "");
		return;
	}

	// �������ض�
	strncpy(dest, src, restrict_len);
	dest[restrict_len] = '\0'; // ȷ���ַ�����β

	// �ж����һ���ַ��Ƿ�Ϊ���ĵĵ�һ���ֽ�
	char* last = dest + strlen(dest) - 1; // ָ�����һ���ַ�
	if ((unsigned char)*last > 127) { // ����Ǻ���
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
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE && pCGI->SLI.is_top_status_line == false)
		return 0;
	if (type == LOWER_STATUS_LINE && pCGI->SLI.is_lower_status_line == false)
		return 0;

	/* ������� */
	int restrict_width = 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.bwidth; 
	/*hh��ѧ����*/
	char* normal_msg = new(nothrow) char[restrict_width + 1]();
	char* catchy_display = new(nothrow) char[restrict_width + 1]();

	if (normal_msg == NULL || catchy_display == NULL) {
		if (normal_msg) 
			delete[] normal_msg;
		if (catchy_display) 
			delete[] catchy_display;
		return -1;  // ���
	}

	gmw_inner_set_half_chinesechar(normal_msg, msg, restrict_width);
	gmw_inner_set_half_chinesechar(catchy_display, catchy_msg, restrict_width);

	/* ���������������Ӧ��״̬�� */
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

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
            2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
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
			// ���Ʊ߿�
			if (has_border) {
				if (i == 0) {
					if (j == 0)
						gmw_inner_simple_showstr(pCGI->CBI.top_left, 1, strlen(pCGI->CBI.top_left));  // ���Ͻ�
					else if (j == pCGI->CFI.block_width / 2 - 1)
						gmw_inner_simple_showstr(pCGI->CBI.top_right, 1, strlen(pCGI->CBI.top_right));  // ���Ͻ�
					else
						gmw_inner_simple_showstr(pCGI->CBI.h_normal, 1, strlen(pCGI->CBI.h_normal));  // ˮƽ�߿�
				}
				else if (i == pCGI->CFI.block_high - 1) {
					if (j == 0)
						gmw_inner_simple_showstr(pCGI->CBI.lower_left, 1, strlen(pCGI->CBI.lower_left));  // ���½�
					else if (j == pCGI->CFI.block_width / 2 - 1)
						gmw_inner_simple_showstr(pCGI->CBI.lower_right, 1, strlen(pCGI->CBI.lower_right));  // ���½�
					else
						gmw_inner_simple_showstr(pCGI->CBI.h_normal, 1, strlen(pCGI->CBI.h_normal));  // ˮƽ�߿�
				}
				else {
					if (j == 0 || j == pCGI->CFI.block_width / 2 - 1)
						gmw_inner_simple_showstr(pCGI->CBI.v_normal, 1, strlen(pCGI->CBI.v_normal));  // ���ұ߿�
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

	/* ��ʾ���� */
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

	// ��ʼ����
	int block_start_x = pCGI->start_x + pCGI->draw_frame_with_row_no * 2 + 2 + col_no * (pCGI->CFI.block_width + pCGI->CFI.separator * 2);
	int block_start_y = pCGI->start_y + pCGI->draw_frame_with_col_no + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator);
	//int block_start_x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator); // 2
	//int block_start_y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator); // 1

	// �����µĻ��ƺ�����has_border ����ȡ���� pCGI->CBI.block_border
	gmw_inner_draw_block_at_position(pCGI, block_start_x, block_start_y, pbdi, pCGI->CBI.block_border, true);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static int gmw_inner_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int BX, const int BY, const int bdi_value_catchy, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* ����ƥ���ɫ�� */
	for (int i = 0; (bdi + i)->value != BDI_VALUE_END; i++){
		if (bdi_value_catchy == (bdi + i)->value) {
			// ��ȡ��ʼλ�� + ��ɫ
			int x = BX;
			int y = BY;
			int bgcolor, fgcolor;
			bgcolor = (bdi + i)->bgcolor < 0 ? pCGI->CFI.bgcolor : (bdi + i)->bgcolor;
			fgcolor = (bdi + i)->fgcolor < 0 ? pCGI->CFI.fgcolor : (bdi + i)->fgcolor;

			if (pCGI->CBI.block_border == false) {  //�ޱ߿�
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
			else {  //�б߿�
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
                        // ����հ�ֵ���������
                        else if ((bdi + i)->value == BDI_VALUE_BLANK) {
                            cct_showstr(x + col * 2, y + row, "  ", bgcolor, fgcolor);
                        } 
                        // ���ƶ����߿�
                        else if (row == 0) {
                            if (col == 0)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.top_left, bgcolor, fgcolor);
                            else if (col == pCGI->CFI.block_width / 2 - 1)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.top_right, bgcolor, fgcolor);
                            else
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.h_normal, bgcolor, fgcolor);
                        } 
                        // ���Ƶײ��߿�
                        else if (row == pCGI->CFI.block_high - 1) {
                            if (col == 0)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.lower_left, bgcolor, fgcolor);
                            else if (col == pCGI->CFI.block_width / 2 - 1)
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.lower_right, bgcolor, fgcolor);
                            else
                                cct_showstr(x + col * 2, y + row, pCGI->CBI.h_normal, bgcolor, fgcolor);
                        } 
						// �������ұ߿�
						else if (col == 0 || col == pCGI->CFI.block_width / 2 - 1)
							cct_showstr(x + col * 2, y + row, pCGI->CBI.v_normal, bgcolor, fgcolor);
						// �����������Ϊ�հ�
						else
							cct_showstr(x + col * 2, y + row, "  ", bgcolor, fgcolor);
					}
				}
			}
			return 0;
		}
	}

	return -1; //�˾�ɸ�����Ҫ�޸�
}

int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value_catchy, const int blank_bdi_value_catchy, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* ȷ���ڳ�ʼ������ܵ����������� */
	if (pCGI->inited != CGI_INITED)
	return -1;

	// ������ʼɫ������Ͻ�λ��
	int x = pCGI->start_x + pCGI->draw_frame_with_row_no * 2 + 2 + col_no * (pCGI->CFI.block_width + pCGI->CFI.separator * 2);
	int y = pCGI->start_y + pCGI->draw_frame_with_col_no + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator);

	// ��ȡ������ǰ����ɫ
	int bgcolor = bdi->bgcolor < 0 ? pCGI->CFI.bgcolor : bdi->bgcolor;
	int fgcolor = bdi->fgcolor < 0 ? pCGI->CFI.fgcolor : bdi->fgcolor;

	// ����ͬ������ƶ�
	int total_steps = distance * (pCGI->CFI.block_width / 2 + pCGI->CFI.separator);

	// ���ݲ�ͬ�ķ�����д���
	if (direction == UP_TO_DOWN) {
		// ���ϵ����ƶ�
		for (int i = 0; i < total_steps; i++) {
			if ((i + 1) % (pCGI->CFI.block_width / 2 + pCGI->CFI.separator) == 0 && pCGI->CFI.separator) {
				for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
					cct_showstr(x + j * 2, y, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			else { 
				for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
					cct_showstr(x + j * 2, y, "  ", bgcolor, fgcolor);
			}
			y += 1; // ����y���꣨�����ƶ���
			gmw_inner_move_block(pCGI, x, y, bdi_value_catchy, bdi);
			Sleep(pCGI->delay_of_block_moved); // ��ʱ
		}
	}

	else if (direction == DOWN_TO_UP) {
		// ���µ����ƶ�
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
		// �������ƶ�
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
		// ���ҵ����ƶ�
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
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
		                                               ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��/λ�÷Ƿ�"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
		   1�����������ƶ����õ���MRow/MCol�봫�����ͬ(���ָ��΢С���ƶ�)���򲻷��أ�������
							  �õ����зǷ�λ�ã��򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
							  �õ���MRow/MCol�봫��Ĳ�ͬ(��������һ���仯)������ update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"���ٷ���MOUSE_ONLY_MOVED����Щ��Ϸ���غ�Ҫ����ɫ��Ĳ�ͬ��ɫ��ʾ��
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
		                          �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_RIGHT_BUTTON_CLICK, MRow��MColȡ��ǰֵ����Ϊ�������ǵ��Ҽ������Ҫ���꣩
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
static int gmw_inner_convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol)
{

	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;

	MRow = MCol = -1024; // ��ʼ��Ϊ�Ƿ�λ��

	// ����
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
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	cct_enable_mouse();

	int MX = 0, MY = 0;  //��¼���λ��
	int type;            //�¼�����
	int MRow_new = MRow, MCol_new = MCol;    //������ֵ
	int sign = 0, old_sign = 0;

	while (1) {
		type = cct_read_keyboard_and_mouse(MX, MY, MAction, KeyCode1, KeyCode2);
		if (type == CCT_KEYBOARD_EVENT)
			return type;
		else {
			//�ж��Ҽ�
			if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				MRow = MRow_new;
				MCol = MCol_new;
				return CCT_MOUSE_EVENT;
			}
			old_sign = sign;
			//�ж����λ���Ƿ�Ϸ�
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

			//�ж����
			if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				if (sign) {
					if (update_lower_status_line && !old_sign) {
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
					}
				}
				else {
					return CCT_MOUSE_EVENT;
				}
			}

			//ʣ������ƶ�
			if (sign) {
				if (update_lower_status_line && !old_sign) {
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
				}
			}
			else {
				MCol_new = (MX - pCGI->start_x - pCGI->draw_frame_with_row_no * 2 - 2) / (pCGI->CFI.block_width + pCGI->CFI.separator * 2);
				MRow_new = (MY - pCGI->start_y - pCGI->draw_frame_with_col_no - 1) / (pCGI->CFI.block_high + pCGI->CFI.separator);
				if (MRow_new != MRow || MCol_new != MCol || old_sign) {
					if (update_lower_status_line) {
						char temp[256];
						sprintf(temp, "[��ǰ���] %c��%d��", char('A' + MRow_new), MCol_new);
						gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
					}
					MRow = MRow_new;
					MCol = MCol_new;
					return CCT_MOUSE_EVENT;
				}
			}
		}
	}

	return CCT_MOUSE_EVENT; //�˾�ɸ�����Ҫ�޸�
}
