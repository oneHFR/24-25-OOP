/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cstring>
#include "90-01-b2-magic_ball.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "../include/common_functions.h"
#include <Windows.h>

using namespace std; 

int main()
{
	while (1) {
		reset_cmd();
		int r, c;
		int s[11][11] = { -1 };

		cct_cls();
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << " 1.�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������" << endl;
		cout << " 2.�ڲ����飬������ʼ����������0�����䲢��0���" << endl;
		cout << " 3.�ڲ����飬������ʼ������������������ʾ" << endl;
		cout << " 4.n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬" << endl;
		cout << " 5.n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬" << endl;
		cout << " 6.n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬����ʼ��������" << endl;
		cout << " 7.n*n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ" << endl;
		cout << " 8.cmdͼ�ν���������(�зָ��ߣ�����ƶ�ʱ��ʾ���꣬�Ҽ��˳�)" << endl;
		cout << " 9.cmdͼ�ν���������" << endl;
		cout << " 0.�˳�" << endl;
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << "[��ѡ��:] ";

		const char* choice = "1234567890";
		int option = menu(choice);
		option = option - '0';

		if (!option) {
			return 0;
		}
		cct_cls();
		input(&r, &c, 5, 9);
		switch (option) {
			case 1:
				h1(s, r, c);
				break;
			case 2:
				h2(s, r, c);
				break;
			case 3:
				h3(s, r, c);
				break;
			case 4:
				h4(s, r, c);
				break;
			case 5:
				h5(s, r, c);
				break;
			case 6:
				h6(s, r, c);
				break;
			case 7:
				h7(s, r, c);
				break;
			case 8:
				h8(s, r, c);
				break;
			case 9:
				h9(s, r, c);
				break;
		}

		int x, y;
		cout << "\n��С�������������End����...";
		cct_getxy(x, y);
		while (1) {

			char keepon[1024] = { 0 };
			cin.getline(keepon, 1024);
			for (int i = 0; keepon[i] != 0; i++) {
				if (keepon[i] <= 'z' && keepon[i] >= 'a') {
					keepon[i] = keepon[i] - ('a' - 'A');
				}
			}
			if (!strcmp(keepon, "END")) {
				break;
			}
			else {
				cct_gotoxy(0, y + 1);
				cout << "�����������������";
				cct_gotoxy(x, y);
				cout << setw(50) << " ";
				cct_gotoxy(x, y);
			}
		}
	}

	return 0;
}