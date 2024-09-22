/* 2152131 计科 吴洪蕊 */
#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"

#define sh 3
#define sw 6
/*
* BASIC
1、菜单已单独显示 >>> common_menu.cpp
2、查找指定坐标位置及周围相同项的函数 两个search的逻辑不一样无法合并 >>> check_1/2
3、判断整个游戏(彩球)/本关(消灭星星)是否结束的函数 >>> hint
4、内部数组进行位置移动的函数 >>> falling_down
5、命令行下打印数组的函数(包括反显、特殊位置标注等) >>> print_out
6、上下两个状态栏的内容显示函数 >>> 这是什么？
7、显示初始外框架的函数 >>> print_canvas
8、绘制一个色块的函数 >>> print_circle
9、移动一个色块的函数 >>> remove_circle

*ADVANCE
1、用随机值填充矩阵的函数，两个项目要求的随机值方式有所不同，如何提炼共用? >>>  fill / input / initializa_input
2、不能采用 if-else 方式将两个完全不同的功能合并为一个函数,而应该近可能采用提取公共步骤的方法(公共步骤占到 50%+才有合并的必要)
3、各人自行定义，无具体要求，如果未提炼任何公用函数，也需要提交一个能集成在项目中编译成功的文件(仅包含一些头文件、命名空间即可
4、根据提炼结果，可以给予额外加分，具体分值待定说明:上述要求的函数放在common 中，函数声明及公共宏定义放在 include 中
*/

/*------------------------------ 初始化区----------------------------------*/
void input(int* r, int* c, int min, int max);
void initializa_input(int s[11][11], int r, int c, int mode = 1);
void fill(int s[11][11], int r, int c, int option, int mode = 1);


/*------------------------------ 移动区----------------------------------*/
void remove_0(int (*s)[11], int r, int c, int(*h)[11], int mode = 0, char timu = 'm');


/*------------------------------ prompt区----------------------------------*/
void enter_option(int option, int r = -5);


/*------------------------------ 打印区----------------------------------*/
void print_out(int (*s)[11], int r, int c, int option, int (*h)[11], int mode = 1);
void clear_instrcution(int r);


/*------------------------------ 判断区----------------------------------*/
int check_1(int(*s)[11], int r, int c, int(*h)[11], int option);
int hint(int(*s)[11], int r, int c, int(*h)[11], int print_hint_circle);

int check_2(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int store =1);
void search(int (*s)[11], int r, int c, int (*h)[11], int ir, int ic, int target, int(*pool)[12], int option);

int check_3(int (*s)[11], int r, int c, int (*h)[11]);



/*------------------------------ 图形区----------------------------------*/
void set_cmd(int r, int c = -1, int mode = 1);
void print_canvas(int(*s)[11], int r, int c, int grid, int speed, int mode = 1);

void print_circle(int(*s)[11], int r, int c, int grid, int(*h)[11], int speed, int option);
void remove_circle(int(*s)[11], int r, int c, int(*h)[11]);

void print_star(int(*s)[11], int r, int c, int grid, int mode = 0);

/*动态下落*/
void falling_down(int(*s)[11], int r, int c, int(*h)[11], int mode = 1, int grid = -1);
void lan(int(*s)[11], int j, int w, int lie, int hang, int helper_down);
//void drop(int(*s)[11], int ir, int ic, int grid);

/*------------------------------ 悬浮区----------------------------------*/
void location_2(int (*s)[11], int r, int c, int grid, int mode =1);

/*magic_ball*/
void enter_option(int option, int r);
void clear_instrcution(int r);
void fill_new(int(*s)[11], int r, int c, int(*h)[11], int print_new_circle);


/*pop_star*/
void score(int r, int c, int (*h)[11], int* num, int* total, int* rest, int mode = 1);
void in_h(int (*h)[11]);