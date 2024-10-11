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
/* �����ַ��������һ���ַ�����������ĵĵ�һ���ֽ� ��ʾ�нض� */
static void gmw_inner_set_half_chinesechar(char* dest, const char* src, int restrict_len);
/* ���ݸ���������x, y������ɫ�� */
static void gmw_inner_draw_block_at_position(const CONSOLE_GRAPHICS_INFO* const pCGI, const int x, const int y, const BLOCK_DISPLAY_INFO* pbdi, bool has_border, bool need_delay);
/*������XY����ת��Ϊ��Ϸ��������к�*/
static int gmw_inner_convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol);
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
	return 0; //�˾�ɸ�����Ҫ�޸�
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
		// �����1�ֽ��ַ������ո�
		if (strlen(src) == 1) {
			dest[0] = src[0];
			dest[1] = ' ';
			dest[2] = '\0';
		}
		// �������2�ֽڣ���ȡǰ2�ֽ�
		else {
			strncpy(dest, src, 2);
			dest[2] = '\0';  // ȷ���ַ����� NULL ��β
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

	return 0; //�˾�ɸ�����Ҫ�޸�
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
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* ����flag */
	cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	cct_cls();
	cct_setconsoleborder(pCGI->cols, pCGI->lines);
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);

	/* �б� */
	if (pCGI->draw_frame_with_col_no) {
		cct_gotoxy(pCGI->start_x + 1 + pCGI->CFI.block_width / 2, pCGI->start_y - 1);
		for (int i = 0; i < pCGI->col_num; i++) {
			if (i < 100)
				cout << setw(2 * pCGI->CFI.separator + pCGI->CFI.block_width) << i << resetiosflags(ios::left);
			else
				cout << setw(2 * pCGI->CFI.separator + pCGI->CFI.block_width) << "**" << resetiosflags(ios::left);
			Sleep(pCGI->delay_of_draw_frame); 
		}
	}

	/* �к� */
	if (pCGI->draw_frame_with_row_no) {
		for (int i = 0; i < pCGI->row_num; i++) {
			cct_gotoxy(pCGI->extern_left_cols, pCGI->start_y + pCGI->CFI.separator + pCGI->CFI.block_high / 2 +
				i * (1 * pCGI->CFI.separator + pCGI->CFI.block_high));
			if (i < 26)
				cout << char(i + 'A') << ' ';
			else if (i < 52)
				cout << char(i - 26 + 'a') << ' ';
			else
				cout << '*' << ' ';
			Sleep(pCGI->delay_of_draw_frame);
		}
	}

	/* ����� */
	cct_setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	if (pCGI->CFI.separator) {  // �зָ���
		for (int i = 0; i < pCGI->CFI.bhigh; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + i);
			for (int j = 0; j < pCGI->CFI.bwidth / 2; j++) {
				if (i == 0) { //����
					if (j == 0)
						cout << pCGI->CFI.top_left;
					else if (j == pCGI->CFI.bwidth / 2 - 1)
						cout << pCGI->CFI.top_right; 
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)
						cout << pCGI->CFI.h_top_separator; 
					else
						cout << pCGI->CFI.h_normal;
				}
				else if (i == pCGI->CFI.bhigh - 1) {  // β��
					if (j == 0)
						cout << pCGI->CFI.lower_left; 
					else if (j == pCGI->CFI.bwidth / 2 - 1)
						cout << pCGI->CFI.lower_right;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)
						cout << pCGI->CFI.h_lower_separator; 
					else
						cout << pCGI->CFI.h_normal; 
				}
				else if (i % (pCGI->CFI.block_high + 1) == 0) {  // �м�ָ���
					if (j == 0)
						cout << pCGI->CFI.v_left_separator;
					else if (j == pCGI->CFI.bwidth / 2 - 1)
						cout << pCGI->CFI.v_right_separator;
					else if (j % (pCGI->CFI.block_width / 2 + 1) == 0)
						cout << pCGI->CFI.mid_separator;
					else
						cout << pCGI->CFI.h_normal;
				}
				else {  // ��ͨɫ����
					if (j % (pCGI->CFI.block_width / 2 + 1) == 0)
						cout << pCGI->CFI.v_normal; 
					else
						cout << "  ";
				}
				Sleep(pCGI->delay_of_draw_frame); 
			}
		}
	}
	else {  // �޷ָ���
		for (int i = 0; i < pCGI->CFI.bhigh; i++) {
			cct_gotoxy(pCGI->start_x, pCGI->start_y + i);
			for (int j = 0; j < pCGI->CFI.bwidth / 2; j++) {
				if (i == 0 && j == 0)
					cout << pCGI->CFI.top_left;
				else if (i == pCGI->CFI.bhigh - 1 && j == 0)
					cout << pCGI->CFI.lower_left;
				else if (i == 0 && j == pCGI->CFI.bwidth / 2 - 1)
					cout << pCGI->CFI.top_right; 
				else if (i == pCGI->CFI.bhigh - 1 && j == pCGI->CFI.bwidth / 2 - 1)
					cout << pCGI->CFI.lower_right;
				else if (i == 0 || i == pCGI->CFI.bhigh - 1)
					cout << pCGI->CFI.h_normal; 
				else if (j == 0 || j == pCGI->CFI.bwidth / 2 - 1)
					cout << pCGI->CFI.v_normal; 
				else
					cout << "  "; 
				Sleep(pCGI->delay_of_draw_frame);
			}
		}
	}  

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
{
	// NULL
	if (src == NULL) {
		strcpy(dest, "");
	}
	else {
		strncpy(dest, src, restrict_len);
		dest[restrict_len] = '\0'; 
		// ������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
		int len = strlen(dest);
		if (len > 0 && (unsigned char)dest[len - 1] > 127) {
			// �������һ����Ч�ַ�����㣨�ӵ����ڶ����ַ���ʼ��飩
			for (int i = len - 1; i >= 0; i--) {
				if ((unsigned char)dest[i] <= 127) {// �ҵ�һ���������ַ���˵��û�нض�
					return;
				}
				else if ((unsigned char)dest[i] > 127 && (i % 2 == 0)) {
					dest[i] = '\0';
					return;
				}
			}
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
	char* normal_msg = new char[restrict_width + 1]();
	char* catchy_display = new char[restrict_width + 1]();

	if (normal_msg == NULL || catchy_display == NULL) {
		if (normal_msg) delete[] normal_msg;
		if (catchy_display) delete[] catchy_display;
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
		if (int(strlen(normal_msg) + strlen(catchy_display)) < restrict_width)
			cout << setw(restrict_width - strlen(normal_msg) - strlen(catchy_display)) << " ";
	}
	else if (type == LOWER_STATUS_LINE) {
		cct_gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		cct_setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
		cout << catchy_display;
		cct_setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		cout << normal_msg;
		if (int(strlen(normal_msg) + strlen(catchy_display)) < restrict_width)
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
	int block_bg_color = (pbdi->bgcolor == -1) ? pCGI->CFI.bgcolor : pbdi->bgcolor;
	int block_fg_color = (pbdi->fgcolor == -1) ? pCGI->CFI.fgcolor : pbdi->fgcolor;
	cct_setcolor(block_bg_color, block_fg_color);

	for (int i = 0; i < pCGI->CFI.block_high; i++) {
		cct_gotoxy(x, y + i);
		for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
			// ���Ʊ߿�
			if (has_border) {
				if (i == 0) {
					if (j == 0)
						cout << pCGI->CBI.top_left;
					else if (j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.top_right;
					else
						cout << pCGI->CBI.h_normal;
				}
				else if (i == pCGI->CFI.block_high - 1) {
					if (j == 0)
						cout << pCGI->CBI.lower_left;
					else if (j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.lower_right;
					else
						cout << pCGI->CBI.h_normal;
				}
				else {
					if (j == 0 || j == pCGI->CFI.block_width / 2 - 1)
						cout << pCGI->CBI.v_normal;
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
	int block_start_x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
	int block_start_y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator);

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
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;

	/* ��ʼ�� �����Ͻ� */
	int block_start_x = pCGI->start_x + 2 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
	int block_start_y = pCGI->start_y + 1 + row_no * (pCGI->CFI.block_high + pCGI->CFI.separator);

	/* �յ����� */
	int block_end_x = block_start_x;
	int block_end_y = block_start_y;

	/* �ж��ƶ����� ���λ�� */
	switch (direction) {
		case UP_TO_DOWN:
			block_end_y = block_start_y + distance * (pCGI->CFI.block_high + pCGI->CFI.separator);
			break;
		case DOWN_TO_UP:
			block_end_y = block_start_y - distance * (pCGI->CFI.block_high + pCGI->CFI.separator);
			break;
		case LEFT_TO_RIGHT:
			block_end_x = block_start_x + distance * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
			break;
		case RIGHT_TO_LEFT:
			block_end_x = block_start_x - distance * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
			break;
	}

	/* move */
	if (pCGI->CFI.separator) { // �зָ�
		for (int step = 0; step < distance; step++) {
			/* �հ� */
			gmw_inner_draw_block_at_position(pCGI, block_start_x, block_start_y, &bdi[bdi_value], pCGI->CBI.block_border, false);
			Sleep(pCGI->delay_of_block_moved);

			switch (direction) {
				case UP_TO_DOWN:
					block_start_y += (pCGI->CFI.block_high + pCGI->CFI.separator);
					break;
				case DOWN_TO_UP:
					block_start_y -= (pCGI->CFI.block_high + pCGI->CFI.separator);
					break;
				case LEFT_TO_RIGHT:
					block_start_x += (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
					break;
				case RIGHT_TO_LEFT:
					block_start_x -= (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);
					break;
			}

			gmw_inner_draw_block_at_position(pCGI, block_start_x, block_start_y, &bdi[bdi_value], pCGI->CBI.block_border, false);
			Sleep(pCGI->delay_of_block_moved);
		}
	}
	else { // �޷ָ�
		for (int step = 0; step < distance; step++) {
			/* �հ� */
			gmw_inner_draw_block_at_position(pCGI, block_start_x, block_start_y, &bdi[bdi_value], pCGI->CBI.block_border, false);
			Sleep(pCGI->delay_of_block_moved);

			switch (direction) {
				case UP_TO_DOWN:
					block_start_y += pCGI->CFI.block_high;
					break;
				case DOWN_TO_UP:
					block_start_y -= pCGI->CFI.block_high;
					break;
				case LEFT_TO_RIGHT:
					block_start_x += pCGI->CFI.block_width;
					break;
				case RIGHT_TO_LEFT:
					block_start_x -= pCGI->CFI.block_width;
					break;
			}

			gmw_inner_draw_block_at_position(pCGI, block_start_x, block_start_y, &bdi[bdi_value], pCGI->CBI.block_border, false);
			Sleep(pCGI->delay_of_block_moved);
		}
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

int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	/* ��ֹ��δ���� gmw_init ǰ������������ */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int MX, MY, prev_MRow, prev_MCol, event;

	while (1) {
		prev_MRow = MRow; // ��¼��ǰ���кź��к�
		prev_MCol = MCol;

		event = cct_read_keyboard_and_mouse(MX, MY, MAction, KeyCode1, KeyCode2); // ��ȡ���̺�����¼�

		// ��������¼�
		if (event == CCT_MOUSE_EVENT) {
			// ����ƶ��¼�
			if (MAction == MOUSE_ONLY_MOVED) {
				int valid = gmw_inner_convert_xy_to_rc(pCGI, MX, MY, MRow, MCol); // �ж�����Ƿ��ںϷ�����
				if (valid == -1) { // ���λ�÷Ƿ�
					if (update_lower_status_line) gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
				}
				else if (MRow != prev_MRow || MCol != prev_MCol) { // ������λ���б䶯
					if (update_lower_status_line) {
						char status_msg[50];
						sprintf(status_msg, "[��ǰ���] %c��%d��", (char)(MRow + 'A'), MCol);
						gmw_status_line(pCGI, LOWER_STATUS_LINE, status_msg);
					}
					return CCT_MOUSE_EVENT; // ��������ƶ��¼�
				}
			}

			// ����������¼�
			else if (MAction == MOUSE_LEFT_BUTTON_CLICK) {
				int valid = gmw_inner_convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (valid == -1) { // �Ƿ�λ��
					if (update_lower_status_line) gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] λ�÷Ƿ�");
				}
				else { // �Ϸ�λ��
					return CCT_MOUSE_EVENT; // �����������¼�
				}
			}

			// ����Ҽ�����¼�
			else if (MAction == MOUSE_RIGHT_BUTTON_CLICK) {
				return CCT_MOUSE_EVENT; // �Ҽ����ֱ�ӷ��أ������ж�λ��
			}
		}

		// ��������¼�
		else if (event == CCT_KEYBOARD_EVENT) {
			return CCT_KEYBOARD_EVENT; // ���ؼ����¼�
		}
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}
