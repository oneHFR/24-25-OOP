/* 2152131 �ƿ� ����� */
#pragma once

#define default_bg 0,7 // 0�� 7��
#define black_bg 0,0 

/*��ʼ������*/
//void input(int* r, int* c);
//void initializa_input(int s[11][11], int r, int c);
//void fill(int s[10][10], int r, int c);

/*�ƶ�������*/
//int check_2(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic);
//void search(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int target, int(*pool)[12], int option);
void get_move(int (*s)[11], int r, int c, int *ir, int *ic);
char choice_3(int (*s)[11], int r, int c, int (*h)[11], int mode = 1);

/*��main������Ӧ��ִ������*/
void H1(int s[11][11], int r, int c); // A
void H2(int s[11][11], int r, int c, int mode = 1); // B
void H3(int s[11][11], int r, int c, int mode = 1); // C
void H4(int s[11][11], int r, int c); // D
void H5(int s[11][11], int r, int c); // E
void H6(int s[11][11], int r, int c); // F
void H7(int s[11][11], int r, int c); // G

