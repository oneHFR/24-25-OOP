/* 2152131 �ƿ� ����� */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "../include/common_menu.h"
#include "../include/common_functions.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"

using namespace std;

int main()
{
	while (1) {
		srand((unsigned int)time(0));
		CONSOLE_GRAPHICS_INFO MagicBall_CGI;
		gmw_init(&MagicBall_CGI);

		int s[MAX_ROW][MAX_COL] = { -1 };
		reset_cmd();
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << " 1.�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������" << endl;
		cout << " 2.�ڲ����飬������ʼ����������0�����䲢��0���" << endl;
		cout << " 3.�ڲ����飬������ʼ������������������ʾ" << endl;
		cout << " 4.n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬" << endl;
		cout << " 5.n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬" << endl;
		cout << " 6.n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬����ʼ��������" << endl;
		cout << " 7.n*n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ" << endl;
		cout << " 8.cmdͼ�ν���������(�зָ��ߣ�����ƶ�ʱ��ʾ���꣬�Ҽ��˳�)" << endl;
		cout << " 9.cmdͼ�ν��������桾ֻ��������ã���" << endl;
		cout << " 0.�˳�" << endl;
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << "[��ѡ��:] ";

		const char* choice = "1234567890";
		//int option = menu(choice);
		//option = option - '0';

		//if (!option) {
		//	return 0;
		//}
		cct_cls();
		MagicBall_CGI.row_num = 7;
		MagicBall_CGI.col_num = 7;
		h9(&MagicBall_CGI, s);
		//input(&MagicBall_CGI, 5, 7);

		//switch (option) {
		//	case 9:
		//		h9(&MagicBall_CGI, s);
		//		break;
		//}

		int x, y;
		cout << endl;
		int l = strlen("��С�������������End����...");
		cct_gotoxy((MagicBall_CGI.cols - l) / 2, MagicBall_CGI.SLI.lower_start_y + 1);//����赽ָ��λ��
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