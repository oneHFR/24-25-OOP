/* 2152131 �ƿ� ����� */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "../include/common_menu.h"
//#include "../include/common_functions.h"
#include "../include/cmd_gmw_tools.h"
#include "90-02-b1-gmw.h"

using namespace std;

int main()
{
	while (1)
	{
		srand((unsigned int)time(0));
		CONSOLE_GRAPHICS_INFO PopStar_CGI;
		gmw_init(&PopStar_CGI);

		int s[MAX_ROW][MAX_COL] = { -1 };
		reset_cmd();
		cout << "--------------------------------------------" << endl;
		cout << "A.�������ҳ����������ʶ" << endl;
		cout << "B.���������һ���������ֲ�����ʾ��" << endl;
		cout << "C.���������һ�أ��ֲ�����ʾ��" << endl;
		cout << "D.αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�" << endl;
		cout << "E.αͼ�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�" << endl;
		cout << "F.αͼ�ν������һ���������ֲ��裩" << endl;
		cout << "G.αͼ�ν��������棨ֻ�������Ч��" << endl;
		cout << "Q.�˳�" << endl;
		cout << "--------------------------------------------" << endl;
		cout << "[��ѡ��:] ";

		const char* choice = "AaBbCcDdEeFfGgQq";
		//int option = 'g';
		int option = menu(choice);
		if (option >= 'a' && option <= 'z')
			option = int(option - 'a' + 'A');
		option = option - 'A' + 1;
		if (option == 'Q' - 'A' + 1)
			break;
		cct_cls();
		switch (option) {
			case 7:
				H7(&PopStar_CGI, s);
				break;
		}

		int x, y;
		cout << endl;
		int l = strlen("��С�������������End����...");
		cct_gotoxy((PopStar_CGI.cols - l) / 2, PopStar_CGI.SLI.lower_start_y + 2);//����赽ָ��λ��
		cout << "��С�������������End����...";
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