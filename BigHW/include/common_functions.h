/* 2152131 �ƿ� ����� */
#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"

#define sh 3
#define sw 6
/*
* BASIC
1���˵��ѵ�����ʾ >>> common_menu.cpp
2������ָ������λ�ü���Χ��ͬ��ĺ��� ����search���߼���һ���޷��ϲ� >>> check_1/2
3���ж�������Ϸ(����)/����(��������)�Ƿ�����ĺ��� >>> hint
4���ڲ��������λ���ƶ��ĺ��� >>> falling_down
5���������´�ӡ����ĺ���(�������ԡ�����λ�ñ�ע��) >>> print_out
6����������״̬����������ʾ���� >>> ����ʲô��
7����ʾ��ʼ���ܵĺ��� >>> print_canvas
8������һ��ɫ��ĺ��� >>> print_circle
9���ƶ�һ��ɫ��ĺ��� >>> remove_circle

*ADVANCE
1�������ֵ������ĺ�����������ĿҪ������ֵ��ʽ������ͬ�������������? >>>  fill / input / initializa_input
2�����ܲ��� if-else ��ʽ��������ȫ��ͬ�Ĺ��ܺϲ�Ϊһ������,��Ӧ�ý����ܲ�����ȡ��������ķ���(��������ռ�� 50%+���кϲ��ı�Ҫ)
3���������ж��壬�޾���Ҫ�����δ�����κι��ú�����Ҳ��Ҫ�ύһ���ܼ�������Ŀ�б���ɹ����ļ�(������һЩͷ�ļ��������ռ伴��
4������������������Ը������ӷ֣������ֵ����˵��:����Ҫ��ĺ�������common �У����������������궨����� include ��
*/

/*------------------------------ ��ʼ����----------------------------------*/
void input(int* r, int* c, int min, int max);
void initializa_input(int s[11][11], int r, int c, int mode = 1);
void fill(int s[11][11], int r, int c, int option, int mode = 1);


/*------------------------------ �ƶ���----------------------------------*/
void remove_0(int (*s)[11], int r, int c, int(*h)[11], int mode = 0, char timu = 'm');


/*------------------------------ prompt��----------------------------------*/
void enter_option(int option, int r = -5);


/*------------------------------ ��ӡ��----------------------------------*/
void print_out(int (*s)[11], int r, int c, int option, int (*h)[11], int mode = 1);
void clear_instrcution(int r);


/*------------------------------ �ж���----------------------------------*/
int check_1(int(*s)[11], int r, int c, int(*h)[11], int option);
int hint(int(*s)[11], int r, int c, int(*h)[11], int print_hint_circle);

int check_2(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int store =1);
void search(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int target, int(*pool)[12], int option);

int check_3(int (*s)[11], int r, int c, int (*h)[11]);



/*------------------------------ ͼ����----------------------------------*/
void set_cmd(int r, int c = -1, int mode = 1);
void print_canvas(int(*s)[11], int r, int c, int grid, int speed, int mode = 1);

void print_circle(int(*s)[11], int r, int c, int grid, int(*h)[11], int speed, int option);
void remove_circle(int(*s)[11], int r, int c, int(*h)[11]);

void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);

/*��̬����*/
void falling_down(int(*s)[11], int r, int c, int(*h)[11], int mode = 1, int grid = -1);
void lan(int(*s)[11], int j, int w, int lie, int hang, int helper_down);
//void drop(int(*s)[11], int ir, int ic, int grid);

/*------------------------------ ������----------------------------------*/
void location_2(int (*s)[11], int r, int c, int grid, int mode =1);

/*magic_ball*/
void enter_option(int option, int r);
void clear_instrcution(int r);
void fill_new(int(*s)[11], int r, int c, int(*h)[11], int print_new_circle);


/*pop_star*/
void score(int r, int c, int (*h)[11], int* num, int* total, int* rest, int mode = 1);
void in_h(int (*h)[11]);