/* 2152131 �ƿ� ����� */
#pragma once

/*---------------------------------------------------------------------------------------------------- */
/*magic_ball_menu.cpp*/
/*�˵���ת*/
//int menu();
/*---------------------------------------------------------------------------------------------------- */
/*magic_ball_tool.cpp*/
/*��������*/
//void enter_option(int option, int r);
//void clear_instrcution(int r);
//void print_out(int(*s)[11], int r, int c, int option, int(*h)[11]);
//int check_1(int(*s)[11], int r, int c, int(*h)[11], int option);
//void remove_0(int(*s)[11], int r, int c, int(*h)[11]);
void fill_new(int(*s)[11], int r, int c, int(*h)[11], int print_new_circle);
/*��ʾ*/
//int hint(int(*s)[11], int r, int c, int(*h)[11], int print_hint_circle);

/*ת�Ƶ�graph.cpp�ļ����� ԭ����д��tool�еģ���Ϊд��ʱ�����˻���gragh��������ļ�*/
/*����*/
//void set_cmd(int r);
//void reset_cmd();
//void print_canvas(int(*s)[11], int r, int c, int grid, int speed);
//void print_circle(int(*s)[11], int r, int c, int grid, int(*h)[11], int speed, int option);
//void remove_circle(int(*s)[11], int r, int c, int(*h)[11]);
///*��̬����*/
//void falling_down(int(*s)[11], int r, int c, int(*h)[11]);
//void lan(int(*s)[11], int j, int w, int lie, int hang, int helper_down);
/*������������Ϸģʽ*/
int location(int(*s)[11], int r, int c, int(*h)[11], int option);
int game_swap(int(*s)[11], int r, int c, int(*h)[11], int x31, int y31, int x32, int y32, int* SCORE);
int game_score(int(*s)[11], int r, int c, int(*h)[11], int* SCORE);

/*---------------------------------------------------------------------------------------------------- */
/*magic_ball_base.cpp*/
/*׼������*/
//void input(int* r, int* c);
//void initializa_input(int s[10][10], int r, int c);
//void fill(int s[10][10], int r, int c);
/*��main������Ӧ��ִ������*/
void h1(int s[11][11], int r, int c);
void h2(int s[11][11], int r, int c);
void h3(int s[11][11], int r, int c);
void h4(int s[11][11], int r, int c);
void h5(int s[11][11], int r, int c);
void h6(int s[11][11], int r, int c);
void h7(int s[11][11], int r, int c);
void h8(int s[11][11], int r, int c);
void h9(int s[11][11], int r, int c);
