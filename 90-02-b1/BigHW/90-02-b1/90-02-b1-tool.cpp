/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_functions.h"
#include <Windows.h>

using namespace std;

void get_move(int (*s)[11], int r, int c, int* ir, int* ic)
{
	cout << "������ĸ+������ʽ[����c2]����������꣺";
	int x0, y0;
	cct_getxy(x0, y0);
	while (1) {
		cct_gotoxy(x0, y0);
		char input[100];
		cin.getline(input, 99); //һ���Ϸ���
		if ((input[0] >= 'a' && input[0] <= ('a' + r - 1)
			|| input[0] >= 'A' && input[0] <= ('A' + r - 1))
			&& (input[1] >= '0' && input[1] <= '0' + c - 1)) {
			if (input[0] >= 'A' && input[0] <= ('A' + r - 1))
				*ir = input[0] - 'A';
			else
				*ir = input[0] - 'a' + 1;
			*ic = input[1] - '0' + 1;
			break;
		}
		cct_gotoxy(0, y0 + 1);
		cout << "�����������������.";
		cct_showch(x0, y0, ' ', black_bg, 40);
		cct_setcolor(default_bg);
	}
	// ����ΪD��1��
	cct_showch(0, y0 + 1, ' ', black_bg, 60);
	cct_setcolor(default_bg);
	cct_gotoxy(0, y0 + 1);
	cout << "����Ϊ" << char(*ir + 'A' - 1) << "��" << *ic - 1 << "��" << endl;
}

char choice_3(int (*s)[11], int r, int c, int (*h)[11], int mode)
{
	/*
	Y �ҳ����п��������ȷ���Ƿ���Ҫ�ϲ�
	N �������β����������������겢������ɱ�������
	Q ������Ϸ
	*/
	char option;
	int x0, y0;
	cct_getxy(x0, y0);
	while (1) {
		cct_showch(x0, y0, ' ', black_bg, 40);
		cct_setcolor(default_bg);
		cct_gotoxy(x0, y0);
		option = _getche();
		if (strchr("yYnNqQ", option) != NULL) {
			if (option >= 'a') {
				option = option - ('a' - 'A');
			}
			break;
		}
	}

	cout << endl << endl;

	if (option == 'Y') {
		cout << "��ֵͬ�鲢�������(��ͬɫ��ʶ)��" << endl;
		print_out(s, r, c, 622, h, 2);
		return 'Y';
	}
	else if (option == 'N') {
		H2(s, r, c, 0);
		return 'N';
	}
	else if (option == 'Q') {
		return 'Q';
	}
	else
		return '1';
}

