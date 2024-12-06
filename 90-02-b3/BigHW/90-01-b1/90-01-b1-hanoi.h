/* 2152131 �ƿ� ����� */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     ���ļ����ܣ�
	1��Ϊ�˱�֤ hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp ���໥���ʺ����ĺ�������
	2��һ�����ϵ�cpp���õ��ĺ궨�壨#define����ȫ��ֻ����const����������������
	3�����Բο� cmd_console_tools.h ��д���������Ķ�����ᣩ
   ------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------------------------------- */
#define maxLayers 10
#define t_a 10
#define t_b 20
#define t_c 30
#define t_y 11
#define t_A 1
#define t_B 33
#define t_C 65
#define t_Y 15



/*---------------------------------------------------------------------------------------------------- */
/*hanoi_menu.cpp*/
/*�˵���ת*/
//int menu();
/*---------------------------------------------------------------------------------------------------- */
/*hanoi_multiple_solutions.cpp*/
/*��ʼ����ȫ�ֱ�������*/
void hanoi(int n, char src, char tmp, char dst, int option);
void initializa_input();
void initializa_tower(int n, char src);
void speed_delay(int speed);
/*���������ִ�ӡ*/
void out_3(char zimu, int k);
void print_4(char src, int option);
void out_4(char src, char dst, int option);
/*�ʴ�*/
void tower_print();
void tower_layer_print(int n, char src);
void tower_move(int n, char src, char dst);
/*��Ϸģʽ*/
int game_input_outcome(char* g_s, char* g_d);
/*�����������option��Ӧ��ִ������*/
void input(int* n, char* src, char* tmp, char* dst, int option);
void operation(int n, char src, char tmp, char dst, int option);
/*��main������Ӧ��ִ������*/
void h1();
void h2();
void h3();
void h4();
void h5();
void h6();
void h7();
void h8();
void h9();


