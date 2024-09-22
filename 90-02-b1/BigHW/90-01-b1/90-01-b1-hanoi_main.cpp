/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-01-b1-hanoi.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include <Windows.h>

using namespace std;
/* ----------------------------------------------------------------------------------
	 ���ļ����ܣ�
	1����main����
	2����ʼ����Ļ
	3�����ò˵�������hanoi_menu.cpp�У�������ѡ��
	4������ѡ����ò˵������Ӧ��ִ�к�����hanoi_multiple_solutions.cpp�У�

	 ���ļ�Ҫ��
	1����������ȫ�ֱ��������ⲿȫ�ֺ;�̬ȫ�֣�const��#define�������Ʒ�Χ�ڣ�
	2����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	3���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��
   ----------------------------------------------------------------------------------- */

   /* demo������ִ�д˾䣬��cmd��������Ϊ40��x120�У����������120�У�����9000�У���cmd�����Ҳ���д�ֱ�����ˣ�*/
int main()
{
	cct_setconsoleborder(120, 40, 120, 9000);
	while (1) {
		reset_cmd();
		cout << setfill('-') << setw(33) << "-" << setfill(' ') << endl;
		cout << "1.������" << endl;
		cout << "2.������(������¼)" << endl;
		cout << "3.�ڲ�������ʾ(����)" << endl;
		cout << "4.�ڲ�������ʾ(����+����)" << endl;
		cout << "5.ͼ�ν�-Ԥ��-������Բ��" << endl;
		cout << "6.ͼ�ν�-Ԥ��-����ʼ���ϻ�n������" << endl;
		cout << "7.ͼ�ν�-Ԥ��-��һ���ƶ�" << endl;
		cout << "8.ͼ�ν�-�Զ��ƶ��汾" << endl;
		cout << "9.ͼ�ν�-��Ϸ��" << endl;
		cout << "0.�˳�" << endl;
		cout << setfill('-') << setw(33) << "-" << setfill(' ') << endl;
		cout << "[��ѡ��:] ";

		const char* choice = "1234567890";
		int option = menu(choice);
		option = option - '0';
		cout << option << endl;
		cout << endl << endl;

		if (!option)
			break;
		switch (option) {
			case 1:
				h1();
				break;
			case 2:
				h2();
				break;
			case 3:
				h3();
				break;
			case 4:
				h4();
				break;
			case 5:
				h5();
				break;
			case 6:
				h6();
				break;
			case 7:
				h7();
				break;
			case 8:
				h8();
				break;
			case 9:
				h9();
				break;
		}

		cout << "���س�������" << endl;
		while (1) {
			int next_option = _getch();
			if (next_option == 13) {
				cct_cls();
				break;
			}
		}
	}

	return 0;
}
