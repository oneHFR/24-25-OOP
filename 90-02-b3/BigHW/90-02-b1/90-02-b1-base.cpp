/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_functions.h"
#include <Windows.h>

using namespace std;

void H1(int s[11][11], int r, int c)
{
	int h[11][11] = { -12 };
	int helper_valid = 0;
	initializa_input(s, r, c);
	fill(s, r, c, 5);
	print_out(s, r, c, 61, h, 2);
	while (!helper_valid) {
		int ir = -1, ic = -1;
		get_move(s, r, c, &ir, &ic);
		helper_valid = check_2(s, r, c, h, ir, ic);
		if (!helper_valid)
			cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
	}
}

void H2(int s[11][11], int r, int c, int mode)
{
	initializa_input(s, r, c, mode);
	fill(s, r, c, 5, mode);
	int h[11][11] = { -12 };
	int num = 0;
	int total = 0;
	int rest = 0;
	print_out(s, r, c, 61, h, 2);
	int ir = -1, ic = -1;
	int helper_valid = 0;
	while (!helper_valid) {
		ir = -1, ic = -1;
		get_move(s, r, c, &ir, &ic);
		helper_valid = check_2(s, r, c, h, ir, ic);
		if (!helper_valid)
			cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
	}
	cout << endl << "���ҽ�����飺" << endl;
	print_out(h, r, c, 621, h, 2);
	cout << endl << "��ǰ����(��ͬɫ��ʶ)��" << endl;
	print_out(s, r, c, 10, h, 2);
	cout << "��ȷ���Ƿ��" << char(ir + 'A' - 1) << ic - 1 << "����Χ����ֵͬ����(Y/N/Q)��";
	int choice3;
	choice3 = choice_3(s, r, c, h);
	if (choice3 == 'Y') {
		score(r, c, h, &num, &total, &rest);
		cout << "���ε÷֣�" << num << " �ܵ÷֣�" << total << endl << endl;
		enter_option(631);
		cout << "���������飺" << endl;
		remove_0(s, r, c, h, 1);
		remove_0(s, r, c, h, 0);
		remove_0(s, r, c, h, 2);
		print_out(s, r, c, 11, h, 2);
	}
}

void H3(int s[11][11], int r, int c, int mode)
{
	initializa_input(s, r, c, mode);
	fill(s, r, c, 5, mode);
	int h[11][11] = { -12 };
	print_out(s, r, c, 61, h, 2);
	int ir = -1, ic = -1;
	int helper_3 = 1;
	int rest = r * c;
	int total = 0;
	while (helper_3) {
		int num = 0;
		int helper_valid = 0;
		while (!helper_valid) {
			in_h(h);
			ir = -1, ic = -1;
			get_move(s, r, c, &ir, &ic);
			helper_valid = check_2(s, r, c, h, ir, ic);
			if (!helper_valid)
				cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
		}
		cout << endl << "���ҽ�����飺" << endl;
		print_out(h, r, c, 621, h, 2);
		cout << endl << "��ǰ����(��ͬɫ��ʶ)��" << endl;
		print_out(s, r, c, 10, h, 2);

		/*---------------------------------------------------------------------------*/

		cout << "��ȷ���Ƿ��" << char(ir + 'A' - 1) << ic - 1 << "����Χ����ֵͬ����(Y/N/Q)��";
		int choice3;
		choice3 = choice_3(s, r, c, h);
		if (choice3 == 'Y') {
			score(r, c, h, &num, &total, &rest);
			cout << "���ε÷֣�" << num << " �ܵ÷֣�" << total << endl << endl;
			enter_option(631);
			cout << "���������飺" << endl;
			remove_0(s, r, c, h, 1);
			remove_0(s, r, c, h, 0);
			remove_0(s, r, c, h, 2);
			print_out(s, r, c, 11, h, 2);
		}
		enter_option(632);
		cout << endl << "��ǰ���飺" << endl;
		print_out(s, r, c, 11, h, 2);

		in_h(h);
		helper_3 = check_3(s, r, c, h);
		if (!helper_3) {
			cout << "ʣ��" << rest << "��������������ؽ���" << endl;
			break;
		}
	}
}

void H4(int s[11][11], int r, int c)
{
	int h[11][11] = { -12 };
	initializa_input(s, r, c);
	fill(s, r, c, 5);

	set_cmd(r, c, 2);
	print_canvas(s, r, c, 0, 0, 2);
	print_star(s, r, c, 0);
	location_2(s, r, c, 0);
	cct_gotoxy(0, 3 * r + 4);
}

void H5(int s[11][11], int r, int c)
{
	int h[11][11] = { -12 };
	initializa_input(s, r, c);
	fill(s, r, c, 5);

	set_cmd(r, c, 3);
	print_canvas(s, r, c, 1, 0, 2);
	print_star(s, r, c, 1);
	location_2(s, r, c, 1);
	cct_gotoxy(0, 4 * r + 3);
}

void H6(int s[11][11], int r, int c)
{
	int h[11][11] = { -12 };
	int x0, y0;

	initializa_input(s, r, c);
	fill(s, r, c, 5);

	set_cmd(r, c, 2);
	print_canvas(s, r, c, 0, 0, 2);
	print_star(s, r, c, 0);
	cct_getxy(x0, y0);
	cct_gotoxy(0, 3 * r + 4);
	cout << "��ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����" << endl;
	cct_gotoxy(x0, y0);
	location_2(s, r, c, 0, 2);

	cct_setcolor(0);
	cct_gotoxy(0, 3 * r + 4);

}

void H7(int s[11][11], int r, int c)
{
	int helper = 27;
	while (helper == 27) {
		int h[11][11] = { -12 };
		int x0, y0;

		initializa_input(s, r, c);
		fill(s, r, c, 5);

		set_cmd(r, c, 3);
		print_canvas(s, r, c, 1, 0, 2);
		print_star(s, r, c, 1);
		cct_getxy(x0, y0);
		cct_gotoxy(0, 4 * r + 3);
		cout << "��ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����" << endl;
		cct_gotoxy(x0, y0);
		helper = location_2(s, r, c, 1, 2);
	}

	cct_gotoxy(0, 4 * r + 3);

}