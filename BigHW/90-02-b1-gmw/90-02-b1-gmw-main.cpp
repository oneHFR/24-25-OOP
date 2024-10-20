/* 2152131 计科 吴洪蕊 */
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
		cout << "A.命令行找出可消除项并标识" << endl;
		cout << "B.命令行完成一次消除（分步骤显示）" << endl;
		cout << "C.命令行完成一关（分步骤显示）" << endl;
		cout << "D.伪图形界面下用鼠标选择一个色块（无分隔线）" << endl;
		cout << "E.伪图形界面下用鼠标选择一个色块（有分隔线）" << endl;
		cout << "F.伪图形界面完成一次消除（分步骤）" << endl;
		cout << "G.伪图形界面完整版（只有这个有效）" << endl;
		cout << "Q.退出" << endl;
		cout << "--------------------------------------------" << endl;
		cout << "[请选择:] ";

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
		int l = strlen("本小题结束，请输入End继续...");
		cct_gotoxy((PopStar_CGI.cols - l) / 2, PopStar_CGI.SLI.lower_start_y + 2);//光标设到指定位置
		cout << "本小题结束，请输入End继续...";
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
				cout << "输入错误，请重新输入";
				cct_gotoxy(x, y);
				cout << setw(50) << " ";
				cct_gotoxy(x, y);
			}
		}
	}
	return 0;
}