/* 2152131 �ƿ� ����� */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "../include/common_menu.h"
#include "../include/common_functions.h"
#include "90-02-b1.h"

using namespace std;

int main()
{
	while (1)
	{
		int r = 10, c = 10;
		int s[11][11] = { -1 };
		reset_cmd();
		cout << "--------------------------------------------" << endl;
		cout << "A.�������ҳ����������ʶ" << endl;
		cout << "B.���������һ���������ֲ�����ʾ��" << endl;
		cout << "C.���������һ�أ��ֲ�����ʾ��" << endl;
		cout << "D.αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�" << endl;
		cout << "E.αͼ�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�" << endl;
		cout << "F.αͼ�ν������һ���������ֲ��裩" << endl;
		cout << "G.αͼ�ν���������" << endl;
		cout << "Q.�˳�" << endl;
		cout << "--------------------------------------------" << endl;
		cout << "[��ѡ��:] ";

		const char* choice = "AaBbCcDdEeFfGgQq";
		int option = menu(choice); 
		if (option >= 'a' && option <= 'z')
			option =  int(option - 'a' + 'A');
		option = option - 'A' + 1;
		if (option == 'Q' - 'A' + 1)
			break;
		cct_cls();
		//input(&r, &c);
		input(&r, &c, 8, 10);
		switch (option) {
			case 1:
				H1(s, r, c);
				break;
			case 2:
				H2(s, r, c);
				break;
			case 3:
				H3(s, r, c);
				break;
			case 4:
				H4(s, r, c);
				break;
			case 5:
				H5(s, r, c);
				break;
			case 6:
				H6(s, r, c);
				break;
			case 7:
				H7(s, r, c);
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

	//cct_showstr(10, 10, "Hello, world!", COLOR_HYELLOW, COLOR_HBLUE);
	//cct_setcolor();
	//cct_gotoxy(0, 24);
	return 0;
}
