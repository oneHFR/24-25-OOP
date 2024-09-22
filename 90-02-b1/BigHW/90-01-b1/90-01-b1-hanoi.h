/* 2152131 计科 吴洪蕊 */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------------------------------- */
#define maxLayers 10
#define t_a 10
#define t_b 20
#define t_c 30
#define t_y 11
#define t_A 1
#define t_B 33
#define t_C 65
#define t_Y 15



/*---------------------------------------------------------------------------------------------------- */
/*hanoi_menu.cpp*/
/*菜单跳转*/
//int menu();
/*---------------------------------------------------------------------------------------------------- */
/*hanoi_multiple_solutions.cpp*/
/*初始化与全局变量设置*/
void hanoi(int n, char src, char tmp, char dst, int option);
void initializa_input();
void initializa_tower(int n, char src);
void speed_delay(int speed);
/*横纵向数字打印*/
void out_3(char zimu, int k);
void print_4(char src, int option);
void out_4(char src, char dst, int option);
/*彩打*/
void tower_print();
void tower_layer_print(int n, char src);
void tower_move(int n, char src, char dst);
/*游戏模式*/
int game_input_outcome(char* g_s, char* g_d);
/*参数输入和与option对应的执行内容*/
void input(int* n, char* src, char* tmp, char* dst, int option);
void operation(int n, char src, char tmp, char dst, int option);
/*与main函数对应的执行内容*/
void h1();
void h2();
void h3();
void h4();
void h5();
void h6();
void h7();
void h8();
void h9();


