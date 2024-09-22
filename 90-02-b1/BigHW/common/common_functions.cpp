/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_functions.h"
#include "../90-01-b2/90-01-b2-magic_ball.h"
#include "../90-02-b1/90-02-b1.h"
#include <Windows.h>

using namespace std;

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
void input(int* r, int* c, int min, int max)
{
	while (1) {
		cout << "����������(" << min << "-" << max << ")��" << endl;
		cin >> *r;
		if (*r >= min && *r <= max && cin.good()) {
			cin.ignore(1024, '\n');
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
		}
	}
	while (1) {
		cout << "����������(" << min << "-" << max << ")��" << endl;
		cin >> *c;
		if (*c >= min && *c <= max && cin.good()) {
			cin.ignore(1024, '\n');
			break;
		}
		else {
			cin.clear();
			cin.ignore(1024, '\n');
		}
	}
	cout << endl;
}

void initializa_input(int s[11][11], int r, int c, int mode) // ���鶨������ǳ���
{
	if (!mode)
		return ;
	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= 10; j++) {
			s[i][j] = -1;
		}
	}
}

void fill(int s[11][11], int r, int c, int option, int mode)
{
	if (!mode)
		return ;
	srand((unsigned)time(NULL));
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			s[i][j] = rand() % option + 1;
		}
	}
	//s[9][1] = 2;
	//s[9][2] = 2;

}

/*------------------------------ �ƶ���----------------------------------*/
void remove_0(int (*s)[11], int r, int c, int(*h)[11], int mode, char timu)
{
	// 2 ֻ����
	// 1 ֻ���0
	// 0 ����
	if (mode != 2) {
		for (int j = 1; j <= c; j++) {
			for (int i = 1; i <= r; i++) {
				if (h[i][j] == 1) {
					s[i][j] = 0;
					if (!mode)
						if (timu == 'm')
							h[i][j] = 0; //��ʼ��Ϊ�������λ�����Ǻ���׼��
				}
			}
		}

		if (mode != 1) {
			for (int j = 1; j <= c; j++) {
				for (int i = r; i > 0; i--) {
					if (s[i][j] == 0) {
						for (int w = i; w > 1; w--) {
							s[w][j] = s[w - 1][j];
						}
						s[1][j] = 0;
						int helper_i = 0;
						for (int w = i; w > 0; w--) {
							if (s[w][j] == 0) {
								helper_i = 0;
							}
							else {
								helper_i = 1;
								break;
							}
						}
						if (helper_i) {
							i++;
						}
					}
				}
			}
		}
	}


	else if (mode == 2){
		for (int j = 1; j <= c; j++) {
			int helper_ceyi = 1;
			for (int i = 1; i <= r; i++) {
				if (s[i][j] != 0) {
					helper_ceyi = 0;
					break;
				}
			}
			if (helper_ceyi) {
				for (int j1 = j + 1; j1 <= c; j1++) {
					for (int i1 = 1; i1 <= r; i1++) {
						s[i1][j1 - 1] = s[i1][j1];
					}
				}
				for (int i1 = 1; i1 <= r; i1++) {
					s[i1][c] = 0;
				}
			}
		}
		for (int j = 1; j <= c; j++) {
			int helper_ceyi = 1;
			for (int i = 1; i <= r; i++) {
				if (s[i][j] != 0) {
					helper_ceyi = 0;
					break;
				}
			}
			if (helper_ceyi) {
				for (int j1 = j + 1; j1 <= c; j1++) {
					for (int i1 = 1; i1 <= r; i1++) {
						s[i1][j1 - 1] = s[i1][j1];
					}
				}
				for (int i1 = 1; i1 <= r; i1++) {
					s[i1][c] = 0;
				}
			}
		}
		for (int j = 1; j <= c; j++) {
			int helper_ceyi = 1;
			for (int i = 1; i <= r; i++) {
				if (s[i][j] != 0) {
					helper_ceyi = 0;
					break;
				}
			}
			if (helper_ceyi) {
				for (int j1 = j + 1; j1 <= c; j1++) {
					for (int i1 = 1; i1 <= r; i1++) {
						s[i1][j1 - 1] = s[i1][j1];
					}
				}
				for (int i1 = 1; i1 <= r; i1++) {
					s[i1][c] = 0;
				}
			}
		}
		// ������һ��bug ���ͬʱ��������������Ҫ��һ������ flag
	}

}


/*------------------------------ prompt��----------------------------------*/
void enter_option(int option, int r)
{
	if (option == 6 || option == 7 || option == 5) {
		clear_instrcution(r);
	}
	if (option == 1) {
		cout << "���س�������Ѱ�ҳ�ʼ��������Ĳ���..." << endl; // 1
	}
	else if (option == 2 || option == 5) {
		cout << "���س����������������0����..." << endl;
		if (option == 2)
			cout << "�����0�������(��ͬɫ��ʶ)��" << endl;
	}
	else if (option == 3 || option == 6) {
		cout << "���س���������ֵ���..." << endl;
		if (option == 3)
			cout << "��ֵ���������(��ͬɫ��ʶ)��" << endl;
	}
	else if (option == 4) {
		cout << "���س�����ʾͼ��..." << endl;
	}
	else if (option == 7) {
		cout << "���س�����ʾ������ʾ..." << endl;
	} // 			cout << "���س������������������..." << endl;
	else if (option == 631) {
		cout << "���س������������������..." << endl;
	}
	else if (option == 632) {
		cout << "�����������������س���������һ�ε�����..." << endl;
	}

	int x;
	while (1)
	{
		x = _getch();
		if (x == 13)
			break;
	}
}

/*------------------------------ ��ӡ��----------------------------------*/
void print_out(int (*s)[11], int r, int c, int option, int (*h)[11], int mode)
{
	if (option == 1) {
		cout << "��ʼ���飺" << endl;
	}
	if (option == 61) {
		cout << "��ǰ���飺" << endl;
	}
	for (int j = 0; j <= c; j++) {
		cct_setcolor(default_bg);
		if (j <= 0)
			cout << setw(3) << "|";
		else {
			if (mode == 2) {
				cout << setw(3) << j - 1;
			}
			else
				cout << setw(3) << j;
		}
	}
	cout << endl << "--+" << setfill('-') << setw(3 * c + 1) << "-" << setfill(' ') << endl;

	if (option == 1 || option == 61 || option == 621 || option == 622 || option == 10 || option == 11 || option == 12) {
		cct_setcolor(default_bg);
		for (int i = 1; i <= r; i++) {
			for (int j = 0; j <= c; j++) {
				if (!j) {
					cout << char('A' + i - 1) << " |";
				}
				else {
					cout << "  "; // 2 space
					if (option == 1 || option == 61) {
						cout << s[i][j];
					}
					else if (option == 621) {
						if (h[i][j] == 1 && s[i][j] != 0)
							cout << '*';
						else
							cout << h[i][j];
					}
					else if (option == 10 || option == 622) {
						if (h[i][j] == 1) {
							cct_setcolor(14, 1);
							if (option == 10)
								cout << s[i][j];
							else if (option == 622)
								cout << 0;
							cct_setcolor(0);
						}
						else {
							cct_setcolor(0);
							cout << s[i][j];
						}
					}
					else if (option == 11) {
						if (s[i][j] == 0) {
							cct_setcolor(14, 1);
							cout << s[i][j];
							h[i][j] = 1; // flag
							cct_setcolor(0);
						}
						else {
							cct_setcolor(0);
							cout << s[i][j];
						}
					}
					else if (option == 12) {
						if (h[i][j] == 1) {
							cct_setcolor(14, 1);
							cout << s[i][j];
							cct_setcolor(0);
							h[i][j] = 0;
						}
						else {
							cct_setcolor(0);
							cout << s[i][j];
						}
					}
				}
			}
			cout << endl;
		}
	}
	cout << endl;
}

void clear_instrcution(int r)
{
	cct_setcolor(0);
	cct_gotoxy(0, 2 * r + 2);
	cout << "                                                      ";
	cct_gotoxy(0, 2 * r + 2);
}


/*------------------------------ �ж���----------------------------------*/
int check_1(int (*s)[11], int r, int c, int (*h)[11], int option)
{
	int helper = 0;
	int helper_check_1 = 0;
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (s[i][j] == s[i][j + 1] && s[i][j] == s[i][j + 2] && s[i][j] != -1) {
				h[i][j] = h[i][j + 1] = h[i][j + 2] = 1;
				helper = 1;
			}
		}
	}
	for (int j = 1; j <= c; j++) {
		for (int i = 1; i <= r; i++) {
			if (s[i][j] == s[i + 1][j] && s[i][j] == s[i + 2][j] && s[i][j] != -1) {
				h[i][j] = h[i + 1][j] = h[i + 2][j] = 1;
				helper = 1;
			}
		}
	}
	helper_check_1 = helper;
	if (option != 9) {
		if (helper == 0) {
			cout << "��ʼ���޿�������" << endl;
			return helper_check_1;
		}
		else {
			cout << "��ʼ���������ͬɫ��ʶ����" << endl;  // 1
		}
	}

	return helper_check_1;
}

int hint(int(*s)[11], int r, int c, int(*h)[11], int print_hint_circle)
{
	int ts = 0;
	int helper_hint = 0;
	// ����h�᲻���ͻ �����Ű�
	// �ܷ�����б߽�����
	// ���ܲ��ܶ�������һ����λ������
	int ds[20][20] = { 0 };
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			ds[i][j] = 0;
		}
	}
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			ds[i + 1][j + 1] = s[i][j];
			h[i][j] = 0;
		}
	}

	for (int i = 2; i <= r; i++) {
		for (int j = 2; j <= c; j++) {
			// up
			if (ds[i - 1][j]) {
				ts = ds[i - 1][j];
				ds[i - 1][j] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j - 1])
					|| (ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j + 2])
					|| (ds[i][j] == ds[i][j - 1] && ds[i][j] == ds[i][j - 2])
					|| (ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i + 2][j]) && ds[i][j] != 0) {
					// ��Ǽӻ�ԭ
					h[i - 1][j - 1] = 1;
					h[i - 2][j - 1] = 1;
					helper_hint = 1;
				}
				ts = ds[i - 1][j];
				ds[i - 1][j] = ds[i][j];
				ds[i][j] = ts;
			}

			// down
			if (ds[i + 1][j]) {
				ts = ds[i + 1][j];
				ds[i + 1][j] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j - 1])
					|| (ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j + 2])
					|| (ds[i][j] == ds[i][j - 1] && ds[i][j] == ds[i][j - 2])
					|| (ds[i][j] == ds[i - 1][j] && ds[i][j] == ds[i - 2][j]) && ds[i][j]) {
					// ��Ǽӻ�ԭ
					h[i - 1][j - 1] = 1;
					h[i][j - 1] = 1;
					helper_hint = 1;
				}
				ts = ds[i + 1][j];
				ds[i + 1][j] = ds[i][j];
				ds[i][j] = ts;
			}

			// left
			if (ds[i][j - 1]) {
				ts = ds[i][j - 1];
				ds[i][j - 1] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i - 1][j])
					|| (ds[i][j] == ds[i - 1][j] && ds[i][j] == ds[i - 2][j])
					|| (ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i + 2][j])
					|| (ds[i][j] == ds[i][j + 1] && ds[i][j] == ds[i][j + 2]) && ds[i][j]) {
					// ��Ǽӻ�ԭ
					h[i - 1][j - 1] = 1;
					h[i - 1][j - 2] = 1;
					helper_hint = 1;
				}
				ts = ds[i][j - 1];
				ds[i][j - 1] = ds[i][j];
				ds[i][j] = ts;
			}

			// right
			if (ds[i][j + 1]) {
				ts = ds[i][j + 1];
				ds[i][j + 1] = ds[i][j];
				ds[i][j] = ts;
				if ((ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i - 1][j])
					|| (ds[i][j] == ds[i - 1][j] && ds[i][j] == ds[i - 2][j])
					|| (ds[i][j] == ds[i + 1][j] && ds[i][j] == ds[i + 2][j])
					|| (ds[i][j] == ds[i][j - 1] && ds[i][j] == ds[i][j - 2]) && ds[i][j]) {
					//// ��Ǽӻ�ԭ
					h[i - 1][j - 1] = 1;
					h[i - 1][j] = 1;
					helper_hint = 1;
				}
				ts = ds[i][j + 1];
				ds[i][j + 1] = ds[i][j];
				ds[i][j] = ts;
			}
		}
	}

	if (print_hint_circle) {
		int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		for (int i = 1; i <= r; i++) {
			for (int j = 1; j <= c; j++) {
				if (h[i][j] == 1) {
					cct_gotoxy(4 * j - 2, 2 * i);
					cct_setcolor(color[s[i][j]], 0);
					cout << "��"; // ���
				}
			}
		}
		clear_instrcution(r);
	}
	return helper_hint;
}

void search(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int target, int(*pool)[12], int option) {
	int i = 0, j = 0;
	//����
	if (option != 1) {
		for (i = ir - 1; i >= 0; i--) {
			if (pool[i][ic] != target) {
				break;
			}
			else if (h[i][ic] != 1) {
				h[i][ic] = 1;
				search(s, r, c, h, i, ic, target, pool, 2);
			}
		}
	}

	//����
	if (option != 2) {
		for (i = ir + 1; i <= r; i++) {
			if (pool[i][ic] != target) {
				break;
			}
			else if (h[i][ic] != 1) {
				h[i][ic] = 1;
				search(s, r, c, h, i, ic, target, pool, 1);
			}
		}
	}

	//����
	if (option != 3) {
		for (j = ic - 1; j >= 0; j--) {
			if (pool[ir][j] != target) {
				break;
			}
			else if (h[ir][j] != 1) {
				h[ir][j] = 1;
				search(s, r, c, h, ir, j, target, pool, 4);
			}
		}
	}

	//����
	if (option != 4) {
		for (j = ic + 1; j <= c; j++) {
			if (pool[ir][j] != target) {
				break;
			}
			else if (h[ir][j] != 1) {
				h[ir][j] = 1;
				search(s, r, c, h, ir, j, target, pool, 3);
			}
		}
	}
}

int check_2(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int store)
{
	// ��Ҫ�ݹ鶯̬����
	// �����Ϸ���
	/*����ΪD��1��
����ľ�������λ�ô���������ֵͬ������������*/
	h[ir][ic] = 1;
	// ���ǵ�֮ǰ������ֵ����⣬�߿���������︴�����䳢��һ��
	int pool[12][12] = { 0 };
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			pool[i][j] = s[i][j];
		}
	}
	int target = s[ir][ic];
	search(s, r, c, h, ir, ic, target, pool, 0);

	for (int j = 1; j <= c; j++) {
		for (int i = 1; i <= r; i++) {
			if (h[i][j] == 1 && !(i == ir && j == ic))
				return 1;
		}
	}
	if (!store) {
		h[ir][ic] = 0;
	}
	return 0;
}

int check_3(int (*s)[11], int r, int c, int (*h)[11]) 
{
	int check_keep = 1;
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (s[i][j]) {
				check_keep = check_2(s, r, c, h, i, j);
				if (check_keep)
					return 1;
			}
		}
	}
	return 0;
}

/*------------------------------ ������----------------------------------*/


/*------------------------------ ͼ����----------------------------------*/
void set_cmd(int r, int c, int mode)
{
	cct_cls();
	int hang, lie;
	if (mode == 3) {
		cct_setfontsize("������", 24);
		hang = 7 + 4 * r;
		lie = 5 + 8 * c;
	}
	else if (mode == 2) {
		cct_setfontsize("������", 24);
		hang = 8 + 3 * r; // 3 * r + 4
		lie = 7 + 6 * c;
	}
	else {
		cct_setfontsize("������", 32);
		hang = 6 + r;
		lie = 40;
	}
	cct_setconsoleborder(lie, hang, lie, 1000);
	cout << "��Ļ��" << hang << "��" << lie << "�� ";
}

void print_canvas(int(*s)[11], int r, int c, int grid, int speed, int mode)
{ //     print_canvas(s, r, c, 1, 0);
	// grid 0 meiyou
	// grid 1 youbianjie
	// speed 1 ���٣�
	// speed 0 �ж�����
	int x, y;
	int m;
	if (speed) 
		m = 0;
	else
		m = 0; // flag
	cct_gotoxy(0, 1);
	if (mode == 2) {
		cct_setcolor();
		if(!grid)
			cout << setw(2) << ' '; //
		for (int j = 0; j < c; j++) {
			if(!j && grid)
				cout << setw(6 + 1 * grid) << j;
			else
				cout << setw(6 + 2 * grid) << j;
		}

		cout << endl;
	}
	cct_setcolor(15, 0);
	if (mode == 2) {
		cct_gotoxy(2, 2);
		cout << "�X�T";
	}
	else
		cout << "�X";
	for (int i = 1; i < c; i++) {
		Sleep(m);
		if (grid) {
			if (mode == 2)
				cout << "�T�T�j�T";
			else
				cout << "�T�j"; // "�T�m";
		}
		else {
			if (mode == 2)
				cout << "�T�T�T";
			else
				cout << "�T";
		}
	}
	if (mode == 2)
		cout << "�T�T�[" << endl;
	else
		cout << "�T�[" << endl;
	cct_setcolor(0); 
	/*---------------------------------------------------------------------------------------*/
	if (mode == 1) {
		for (int i = 1; i <= r; i++) {
			for (int j = 0; j <= c + 1; j++) {
				if (j == 0) {
					cct_setcolor(15, 0);
					cout << "�U";
					if (grid) {
						for (int w = 1; w < c; w++) {
							cout << "  �U";
						}
						cout << "  ";
					}
					else {
						cct_setcolor(0);
						cout << setfill(' ') << setw(2 * c) << ' ';
					}
					cct_setcolor(15, 0);
					cout << "�U";
					Sleep(m);
				}
			}
			cout << endl;

			if (i != r && grid) {
				for (int j = 0; j <= 2 * c; j++) {
					if (j == 0) {
						cout << "�d";
					}
					else if (j == 2 * c) {
						cout << "�g";
					}
					else if (j % 2 == 0) {
						cout << "�p";
					}
					else {
						cout << "�T";
					}
				}
				cout << endl;
			}
		}
	}


	else if (mode == 2) {
		if (grid) {
			for (int i = 0; i < r; i++) {
				cout << "  ";
				cct_setcolor(15, 0);
				cout << "�U";
				for (int j = 0; j < c; j++) {
					cout << "      �U";
					Sleep(m);
				}
				cct_setcolor();
				cout << endl;

				cout << (char)(i + 'A') << ' ';
				cct_setcolor(15, 0);
				cout << "�U";
				for (int j = 0; j < c; j++) {
					cout << "      �U";
					Sleep(m);
				}
				cct_setcolor();
				cout << endl;

				cout << "  ";
				cct_setcolor(15, 0);
				cout << "�U";
				for (int j = 0; j < c; j++) {
					cout << "      �U";
					Sleep(m);
				}
				cct_setcolor();
				cout << endl;

				if (i != r - 1) {
					cout << "  ";
					cct_setcolor(15, 0);
					cout << "�d";
					for (int j = 0; j < c - 1; j++) {
						cout << "�T�T�T�p";
						Sleep(m);
					}
					cout << "�T�T�T�g";
					cct_setcolor();
					cout << endl;
				}
			}
		}
		else
			for (int i = 0; i < r; i++) {
				cout << "  ";
				cct_setcolor(15, 0);
				cout << "�U";
				for (int j = 0; j < c; j++) {
					cout << "      ";
					Sleep(m);
				}
				cout << "�U";
				cct_setcolor();
				cout << endl;

				cout << (char)(i + 'A') << ' ';
				cct_setcolor(15, 0);
				cout << "�U";
				for (int j = 0; j < c; j++) {
					cout << "      ";
					Sleep(m);
				}
				cout << "�U";
				cct_setcolor();
				cout << endl;

				cout << "  ";
				cct_setcolor(15, 0);
				cout << "�U";
				for (int j = 0; j < c; j++) {
					cout << "      ";
					Sleep(m);
				}
				cout << "�U";
				cct_setcolor();
				cout << endl;
			}

	}

	/*---------------------------------------------------------------------------------------*/
	cct_setcolor(15, 0);
	if (mode == 2) {
		cct_getxy(x, y);
		cct_gotoxy(2, y);
		cout << "�^�T";
	}
	else
		cout << "�^";
	for (int i = 1; i < c; i++) {
		Sleep(m);
		if (grid) {
			if (mode == 2)
				cout << "�T�T�m�T";
			else
				cout << "�T�m"; // "�T�m";
		}
		else {
			if (mode == 2)
				cout << "�T�T�T";
			else
				cout << "�T";
		}
	}
	if (mode == 2) {
		cout << "�T�T�a" << endl;
		//cct_getxy(x, y);
		//cout << x << endl;
		//cout << y << endl;
	}
	else
		cout << "�T�a" << endl;
	cct_setcolor(0);
}


void print_circle(int(*s)[11], int r, int c, int grid, int(*h)[11], int speed, int option)
{
	int m = 5;
	if (speed) {
		m = 0;
	}
	else {
		m = 5;
	}
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	if (grid) {
		for (int i = 1; i <= r; i++) {
			cct_gotoxy(0, 2 * i);
			for (int j = 1; j <= c; j++) {
				cct_setcolor(color[s[i][j]], 0);
				int x, y;
				cct_getxy(x, y);
				cct_gotoxy(x + 2, y);
				if (h[i][j] == 1) {
					cout << "��"; // 
				}
				else {
					cout << "��"; // ��
				}
				if (option != 9) {
					Sleep(5);
				}
			}
			cout << endl;
			cct_setcolor(0);
		}
	}
	else {
		for (int i = 1; i <= r; i++) {
			cct_gotoxy(2, 1 + i);
			for (int j = 1; j <= c; j++) {
				cct_setcolor(color[s[i][j]], 0);
				if (h[i][j] == 1) {
					cout << "��"; // 
				}
				else {
					cout << "��"; // ��
				}
				Sleep(m);
			}
			cout << endl;
			cct_setcolor(0);
		}
	}
}

void remove_circle(int(*s)[11], int r, int c, int(*h)[11])
{
	// ͨ�����������ҵ������ڿ���֤��demo�Ǻ���һ��һ��ɨ��������
	int x, y;
	int m = 200;
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	for (int i = 1; i <= r; i++) {
		cct_gotoxy(0, 2 * i);
		for (int j = 1; j <= c; j++) {
			cct_setcolor(color[s[i][j]], 0);
			cct_getxy(x, y);
			cct_gotoxy(x + 2, y);
			if (h[i][j] == 1) {
				cout << "��"; // �� �� ��
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "��"; // �� �� ��
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "��"; // �� �� ��
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "��"; // �� �� ��
				cct_gotoxy(x + 2, y);
				Sleep(m / 2);
				cout << "��"; // �� �� ��
				cct_setcolor(15, 0);
				cct_gotoxy(x + 2, y);
				cout << "  ";
			}
			else {
				cct_gotoxy(x + 4, y);
			}
		}
	}
	clear_instrcution(r);
}

void print_star(int(*s)[11], int r, int c, int grid, int mode)
{
	// 1 ��ɫ 2 ��ʧ 3 �ָ�
	int color[6] = { -1, 6, 7, 8, 9, 10};
	int x, y;
	int change = 0;
	if (mode) {
		x = (2 * grid + 6) * c - 2 - 2 * grid, y = (3 + grid) * r - grid;
		if (mode == 1)
			change = 15;
		cct_setcolor(color[s[r][c]], change);
		if (mode == 2)
			cct_setcolor(15, 15);
		cct_gotoxy(x, y);
		cout << "�X�T�[" << endl;
		cct_gotoxy(x, y + 1);
		cout << "�U  �U" << endl;
		cct_gotoxy(x, y + 2);
		cout << "�^�T�a" << endl;
		cct_gotoxy(x + 2, y + 1);
		cout << "��";
		cct_setcolor();
		if (mode == 3)
			Sleep(3);
	}
	else
		for (int i = 1; i <= r; i++) {
			for (int j = 1; j <= c; j++) {
				x = (2 * grid + 6) * j - 2 - 2 * grid, y = (3 + grid) * i - grid;
				cct_setcolor(color[s[i][j]], 0);
				//cct_setcolor(15, 15);
				cct_gotoxy(x, y);
				cct_gotoxy(x, y);
				cout << "�X�T�[" << endl;
				cct_gotoxy(x, y + 1);
				cout << "�U  �U" << endl;
				cct_gotoxy(x, y + 2);
				cout << "�^�T�a" << endl;
				cct_gotoxy(x + 2, y + 1);
				cout << "��";
				cct_setcolor();
			}
		}
}

void n_star(int x, int y, int e, int change)
{
	int color[6] = { -1, 6, 7, 8, 9, 10 };
	cct_setcolor(color[e], change);
	cct_gotoxy(x - 2, y - 1);
	cct_getxy(x, y);
	cout << "�X�T�[" << endl;
	cct_gotoxy(x, y + 1);
	cout << "�U  �U" << endl;
	cct_gotoxy(x, y + 2);
	cout << "�^�T�a" << endl;
	cct_gotoxy(x + 2, y + 1);
	cout << "��";
	cct_setcolor(default_bg);
}
void lan(int(*s)[11], int j, int w, int lie, int hang, int helper_down)
{
	int m = 5;
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	cct_setcolor(color[s[w][j]], 0);
	cct_gotoxy(lie, hang);
	cout << "��";
	Sleep(m);
	if (hang - 1 > 0) {
		cct_setcolor(15, 0);
		cct_gotoxy(lie, hang - 1);
		cout << "  ";
		Sleep(m);
	}

	cct_setcolor(color[s[w][j]], 0);
	cct_gotoxy(lie, hang + 1);
	cout << "��";
	Sleep(m);
	cct_setcolor(15, 0);
	cct_gotoxy(lie, hang);
	cout << "�T";
}
void drop(int(*s)[11], int ir, int ic, int grid, int mode, int step = -1)
{
	int color[6] = { -1, 6, 7, 8, 9, 10 };
	int x, y;
	if (mode == 1) {
		for (int n = 0; n < step; n++) {
			for (int j = 1; j <= 3 + grid; j++) {
				x = (2 * grid + 6) *ic - 2 - 2 * grid, y = (3 + grid) * (ir + n) - grid + j;
				cct_setcolor(color[s[ir][ic]], 0);
				cct_gotoxy(x, y);
				cout << "�X�T�[" << endl;
				cct_gotoxy(x, y + 1);
				cout << "�U  �U" << endl;
				cct_gotoxy(x, y + 2);
				cout << "�^�T�a" << endl;
				cct_gotoxy(x + 2, y + 1);
				cout << "��";
				cct_setcolor(15, 15);
				cct_gotoxy(x, y - 1);
				cout << "�X�T�[" << endl;
				cct_setcolor();
				if (j == 3 + grid) {
					cct_setcolor(15, 0);
					cct_gotoxy(x, y - 1);
					cout << "�T�T�T" << endl;
					cct_setcolor();
				}
				Sleep(3);
			}
		}

	}

	else if (mode == 2) {
		int m[11] = { -1 };
		for (int j = 1; j <= ic; j++) {
			for (int o = 1; o <= ir; o++) {
				if (s[o][j] != 0) {
					m[j] = 0;
					break;
				}
				m[j] = 1;

			}
	
		}
		for (int j = 1; j <= ic; j++) {
			if (m[j] == 1) {
				//for()
				for (int i = 1; i <= 2 * grid + 6; i++) {
					x = (2 * grid + 6) * ic - 2 - 2 * grid - i, y = (3 + grid) * j - grid;
					cct_setcolor(color[s[ir][ic]], 0);
					cct_gotoxy(x, y);
					cout << "�X�T�[" << endl;
					cct_gotoxy(x, y + 1);
					cout << "�U  �U" << endl;
					cct_gotoxy(x, y + 2);
					cout << "�^�T�a" << endl;
					cct_gotoxy(x + 2, y + 1);
					cout << "��";
					cct_setcolor(15, 15);
					cct_gotoxy(x + 6, y + 0);
					cout << ' ';
					cct_gotoxy(x + 6, y + 1);
					cout << ' ';
					cct_gotoxy(x + 6, y + 2);
					cout << ' ';
					cct_setcolor();
					Sleep(1);
				}
			}
		}

	}
}

void falling_down(int(*s)[11], int r, int c, int(*h)[11], int mode, int grid)
{
	int x, y, w;
	int lie = 2*c, hang = 2 * r;  // c r
	int already_up = 2 * r; // ���һ�����������
	int already_bottom = 2 * r; // ���һ�ε��ײ��������� ������Ĳ�һ��
	int color[10] = { -1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	int helper_down = 0; // �Ѿ����������
	int step = 0; // ��Ҫ�½��ĸ߲�

	for (int j = 1; j <= c; j++) {
		helper_down = 0;
		if (mode == 1) {
			lie = j * 4 - 2;
			hang = 2 * r;  // c r
			already_up = 2 * r; // ���һ�����������
			already_bottom = 2 * r; // ���һ�ε��ײ��������� ������Ĳ�һ��
		}
		else {
			lie = (j * (3 + grid) - 1 + 3 * grid) * 2;
			hang = (3 + grid) * r - grid;
			already_up = (3 + grid) * r - grid; // ���һ�����������
			already_bottom = (3 + grid) * r - grid; // ���һ�ε��ײ��������� ������Ĳ�һ��
			int color[6] = { -1, 6, 7, 8, 9, 10 };
		}

		cct_gotoxy(lie, hang); //����ײ����ϱ���
		for (int i = r; i > 0; i--) {
			
			int helper_star = 0;
			if (h[i][j] == 1) {
				int helper_star = 0;
				step = 0;
				for (w = i; w > 0; w--) {

					if (h[w][j] == 1) {
						step++;
					}
					else {
						break;
					}
				}
				// ��ʱw�����ƵĲ������
				if (mode == 1) {
					hang = w * 2 + 1;
					already_up = 2 * w;
				}
				else {
					hang = (3 + grid) * w - grid +1;
					already_up = (3 + grid) * w - grid;
				}

				for (int step_i = 1; step_i < 10; step_i++) {
					if (hang < already_bottom && hang > 1) { // flag
						if (mode == 1)
							lan(s, j, w, lie, hang, helper_down);
						else if(!helper_star) {
							helper_star = 1;
							drop(s, w, j, grid, 1, step);
						}

						if (mode == 1)
							hang += 2;
						else
							hang += (3 + grid);
						helper_down = 1;
					}
					else {
						cct_getxy(x, y);
						already_bottom = min(y, already_bottom);
						break;
					}
				}
			}
			else if (helper_down) {
				if (mode == 1) {
					hang = i * 2 + 1;
					already_up = i * 2;
				}
				else {
					hang = i * (3+grid) + 1;
					already_up = i * (3+grid);
				}
				for (int step_i = 1; step_i < 10; step_i++) {
					if (hang < already_bottom && hang > 1) {
						if (mode == 1)
							lan(s, j, i, lie, hang, helper_down);
						else if (!helper_star) {
							helper_star = 1;
							drop(s, i, j, grid, 1, step);

						}
						if (mode == 1)
							hang += 2;
						else
							hang += 3 + grid;
					}
					else {
						cct_getxy(x, y);
						already_bottom = min(y, already_bottom);
						break;
					}
				}
			}
			else {
				if (mode == 1)
					already_bottom -= 2;
				else
					already_bottom -= (3+grid);

			}
			if (mode == 1)
				i = min(i, already_up / 2);
			else
				i = min(i, already_up / (3 + grid));
			
		}
	}
	if (mode == 1)
		clear_instrcution(r);
}


void location_2(int (*s)[11], int r, int c, int grid, int mode)
{
	int first = 0, prompt = 0;
	// ��ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����
	// ���κϳɽ����� ��C/�������������һ�κϳɣ�
	// ��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�                                                                                    
	// ���ε÷�:125 �ܷ�:125
	// ��Χ����ֵͬ����ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����
	int x = 6, y = 4, X = 0, Y = 0;
	int ret, maction, keycode1, keycode2;
	int loop = 1;
	cct_enable_mouse();

	int sel = 0;
	int inval = 0;
	int total = 0;
	int rest = r * c;
	int h[11][11] = { -12 };
	int sx = 0, sy = 0;
	while (loop) {

		int num = 0;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		cct_setcursor(CURSOR_INVISIBLE);
		if (ret == CCT_MOUSE_EVENT) {
			if (sx == X && sy == Y) {

			}
			else {
				//�ָ�
				for (int i = 1; i <= r; i++) {
					for (int j = 1; j <= c; j++) {
						// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
						if (h[i][j]) {
							print_star(s, i, j, grid, 3);
						}
					}
				}
				in_h(h);
			}
			sx = X;
			sy = Y;



			int check = 1;
			if (grid) {
				if (Y > 2 && Y < 4 + (r - 1) * (3 + grid) + 2 && X>2 && X < 6 + (c - 1) * (6 + 2 * grid) + 4) {
					for (int i = 0; i < c - 1; i++)
						if (X == 6 + i * (6 + 2 * grid) + 4 || X == 6 + i * (6 + 2 * grid) + 5)
							check = 0;
					for (int i = 0; i < r - 1; i++)
						if (Y == 4 + i * (3 + grid) + 2)
							check = 0;
				}
				else
					check = 0;
			}
			if (check) {
				if (Y > 2 && Y < 4 + (r - 1) * (3 + grid) + 2
					&& X >2 && X < 6 + (c - 1) * (6 + 2 * grid) + 4) {
					int x15 = 6 + ((X - 4) / (sw + 2 * grid)) * (6 + 2 * grid);
					int y15 = 4 + ((Y - 3) / (grid + sh)) * (3 + grid);
					n_star(x, y, s[((y - 3) / (sh + grid)) + 1][(x - 4) / (sw + 2 * grid) + 1], 0);
					n_star(x15, y15, s[((Y - 3) / (grid + sh)) + 1][(X - 4) / (2 * grid + sw) + 1], 15);
					// r2 = (Y - 3) / (grid + sh) + 1
					if ((x != x15 || y != y15) && !first) {
						if (mode == 2) {
							//�ָ�
							for (int i = 1; i <= r; i++) {
								for (int j = 1; j <= c; j++) {
									// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
									if (h[i][j]) {
										print_star(s, i, j, grid, 3);
									}
								}
							}
							in_h(h);
						}
						n_star(x, y, s[((y - 3) / (grid + sh)) + 1][(x - 4) / (2 * grid + sw) + 1], 0);

					}
					if (first)
						first = false;
					if (inval && (x != x15 || y != y15)) {
						inval = 0;
						cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
						cct_setcolor(default_bg);
					}
					if (sel && (x != x15 || y != y15)) {
						sel = 0;
						cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
						cct_setcolor(default_bg);
					}

					x = x15;
					y = y15;

					if (!inval && !sel) {
						cct_gotoxy(0, (3 + grid) * r + (4 - grid));
						cout << "[��ǰ���] " << (char)('A' + (Y - 3) / (grid + sh)) << "��" << (X - 4) / (2 * grid + sw) << "��" << "                                                      ";
					}


					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));

					if (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_RIGHT_BUTTON_CLICK) {
						if (mode == 2) {
							int ir = (y - 3) / (grid + sh) + 1;
							int ic = (x - 4) / (2 * grid + sw) + 1;
							in_h(h);
							if (check_2(s, r, c, h, ir, ic, 1)) {
								if (!sel && !inval) {
									// �����ɫ // �����ʧ��
									for (int i = 1; i <= r; i++) {
										for (int j = 1; j <= c; j++) {
											// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
											if (h[i][j]) {
												print_star(s, i, j, grid, 1);
											}
										}
									}
									cct_gotoxy(0, (3 + grid) * r + (4 - grid));
									//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
									cout << "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�";
									sel = 1;
								}

								else if (sel && !inval) {
									//��ʧ
									for (int i = 1; i <= r; i++) {
										for (int j = 1; j <= c; j++) {
											if (h[i][j]) {
												print_star(s, i, j, grid, 2);
											}
										}
									}

									sel = 0;
									score(r, c, h, &num, &total, &rest);
									cct_gotoxy(0, 0);
									cout << "                                                              ";
									cct_gotoxy(0, 0);
									cout << "���ε÷֣�" << num << " �ܵ÷֣�" << total;

									//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
									cct_gotoxy(0, (3 + grid) * r + (4 - grid));
									cout << "�ϳ���ɣ��س���/�����������0" << "                                                              ";
									// flag
									while (1) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

										if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
											break;
										else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
											break;
									}
									// ����
									// ������
									remove_0(s, r, c, h, 1);
									falling_down(s, r, c, h, 2, grid);
									// ������
									remove_0(s, r, c, h, 0, 's');
									drop(s, r, c, grid, 2);
									remove_0(s, r, c, h, 2);

									//in_h(h);

									//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
									cct_gotoxy(0, (3 + grid) * r + (4 - grid));
									cout << "���κϳɽ����� ��C/�������������һ�κϳɣ�" << "                                                              ";
									while (1) {
										ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
										if (ret == CCT_MOUSE_EVENT)
											break;
										else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 'c' || keycode1 == 'C')
											break;
									}
									if (!grid)
										loop = 0;
								}
								//break;
							}

							else {
								cout << "��Χ����ֵͬ����ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����";
								inval = true;
							}
						}
						else {
							cout << "ѡ����" << (char)('A' + (y - 3) / (grid + sh)) << "��" << (x - 4) / (2 * grid + sw) << "��" << "                                                      ";
							loop = 0;
						}
					}
				}

			}

			else {
				n_star(x, y, s[((y - 3) / (sh + grid)) + 1][(x - 4) / (sw + 2 * grid) + 1], 0);
				if (!first) {
					n_star(x, y, s[((y - 3) / (sh + grid)) + 1][(x - 4) / (sw + 2 * grid) + 1], 0);
					first = true;
				}
				if (inval) {
					inval = 0;
					first = 1;
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
				}
				if (sel) {
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					sel = 0;
					first = 1;
				}
				cct_gotoxy(0, (3 + grid) * r + (4 - grid));
				// 	cct_gotoxy(0, 4 * r + 3);
				// 	cct_gotoxy(0, 3 * r + 4);
				cout << "[��ǰ���] λ�÷Ƿ�" << "                                                      ";
			}
		}


		else if (ret == CCT_KEYBOARD_EVENT) {
			if ((keycode1 == 224 &&
				(keycode2 == KB_ARROW_UP || keycode2 == KB_ARROW_DOWN || keycode2 == KB_ARROW_LEFT || keycode2 == KB_ARROW_RIGHT))
				|| keycode1 == 13 || keycode1 == 'q' || keycode1 == 'Q') {
				cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
				cct_setcolor(default_bg);
			}

			//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
			cct_gotoxy(0, (3 + grid) * r + (4 - grid));
			//
			//
			//

			switch (keycode1) {
				case 224:
					n_star(x, y, s[((y - 3) / (grid + sh)) + 1][(x - 4) / (2 * grid + sw) + 1], 0);
					//�ָ�
					for (int i = 1; i <= r; i++) {
						for (int j = 1; j <= c; j++) {
							// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
							if (h[i][j]) {
								print_star(s, i, j, grid, 3);
							}
						}
					}
					in_h(h);
					sel = 0;

					switch (keycode2) {
						case KB_ARROW_UP:

							if (y == 4)
								y = 4 + (r - 1) * (3 + grid);
							else
								y -= sh + grid;

							break;
						case KB_ARROW_DOWN:
							if (y == 4 + (r - 1) * (3 + grid))
								y = 4;
							else
								y += sh + grid;
							break;
						case KB_ARROW_LEFT:
							if (x == 6)
								x = 6 + (c - 1) * (6 + 2 * grid);
							else
								x -= sw + 2 * grid;

							break;
						case KB_ARROW_RIGHT:
							if (x == 6 + (c - 1) * (6 + 2 * grid))
								x = 6;
							else
								x += sw + 2 * grid;

							break;
					}
					n_star(x, y, s[((y - 3) / (grid + sh)) + 1][(x - 4) / (2 * grid + sw) + 1], 15);
					//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					cout << "[��ǰ����]" << (char)('A' + (y - 3) / (grid + sh)) << "��" << (x - 4) / (2 * grid + sw) << "��" << "                                                      ";
					break;


				case 13:
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					if (mode == 2) {
						int ir = (y - 3) / (grid + sh) + 1;
						int ic = (x - 4) / (2 * grid + sw) + 1;
						in_h(h);
						if (check_2(s, r, c, h, ir, ic, 1)) {
							if (!sel) {
								// �����ɫ // �����ʧ��
								for (int i = 1; i <= r; i++) {
									for (int j = 1; j <= c; j++) {
										// void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);
										if (h[i][j]) {
											print_star(s, i, j, grid, 1);
										}
									}
								}
								//cct_gotoxy(0, (3 + grid)* r + (4 - grid));
								cct_gotoxy(0, (3 + grid) * r + (4 - grid));
								cout << "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/��������ϳ�";
								sel = 1;
							}
							else if (sel) {
								//��ʧ
								for (int i = 1; i <= r; i++) {
									for (int j = 1; j <= c; j++) {
										if (h[i][j]) {
											print_star(s, i, j, grid, 2);
										}
									}
								}

								sel = 0;
								score(r, c, h, &num, &total, &rest);
								cct_gotoxy(0, 0);
								cout << "                                                              ";
								cct_gotoxy(0, 0);
								cout << "���ε÷֣�" << num << " �ܵ÷֣�" << total;

								cct_gotoxy(0, (3 + grid) * r + (4 - grid));
								cout << "�ϳ���ɣ��س���/�����������0" << "                                                              ";
								// flag
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

									if (ret == CCT_MOUSE_EVENT && (maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_LEFT_BUTTON_DOUBLE_CLICK || maction == MOUSE_LEFTRIGHT_BUTTON_CLICK || maction == FROM_LEFT_2ND_BUTTON_PRESSED))
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 13)
										break;
								}
								// ����
								// ������
								remove_0(s, r, c, h, 1);
								falling_down(s, r, c, h, 2, grid);
								// ������
								remove_0(s, r, c, h, 0, 's');
								drop(s, r, c, grid, 2);
								remove_0(s, r, c, h, 2);

								//in_h(h);

								cct_gotoxy(0, (3 + grid) * r + (4 - grid));
								cout << "���κϳɽ����� ��C/�������������һ�κϳɣ�" << "                                                              ";
								while (1) {
									ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
									if (ret == CCT_MOUSE_EVENT)
										break;
									else if (ret == CCT_KEYBOARD_EVENT && keycode1 == 'c' || keycode1 == 'C')
										break;
								}
								if (!grid)
									loop = 0;
							}
							break;
						}

						else {
							cout << "��Χ����ֵͬ����ͷ��/����ƶ����س���/�������ѡ��, Q/�����Ҽ�����";
							inval = true;
						}
					}

					else {
						cout << "ѡ����" << (char)('A' + (y - 3) / (grid + sh)) << "��" << (x - 4) / (2 * grid + sw) << "��" << "                                                      ";
						loop = 0;
					}
					break;

				case 'q':
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					loop = 0;
					break;


				case'Q':
					cct_showch(0, (3 + grid) * r + (4 - grid), ' ', 0, 80);
					cct_setcolor(default_bg);
					cct_gotoxy(0, (3 + grid) * r + (4 - grid));
					loop = 0;
					break;
			}
		}
	}

	cct_disable_mouse();
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
}

void score(int r, int c, int (*h)[11], int* num, int* total, int* rest, int mode)
{
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= c; j++) {
			if (h[i][j] == 1)
				(*num)++;
		}
	}
	*rest -= *num;
	*num = (*num) * (*num) * 5;
	if (*rest < 10 && mode == 0) {
		*total = *num + 180 * (10 - *rest);
	}
	else
		*total += *num;
}

void in_h(int (*h)[11])
{
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			h[i][j] = 0;
		}
	}
}


