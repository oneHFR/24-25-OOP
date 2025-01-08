/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include "../include/class_aat.h"
#include "hw_check.h"

// 自测脚本
/*
@REM Base
hw_check --action base --cno 10108001 --stu all --file all > m.txt
hw_ddemo --action base --cno 10108001 --stu all --file all > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >c.txt

hw_check --action base --cno 10108001 --stu all --file all --week 6 --chapter 13> m.txt
hw_ddemo --action base --cno 10108001 --stu all --file all --week 6 --chapter 13 > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action base --cno 10108002 --stu all --file all --week 6 --chapter 13 > m.txt
hw_ddemo --action base --cno 10108002 --stu all --file all --week 6 --chapter 13 > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action base --cno 5000244001602 --stu all --file all --week 6 --chapter 4 > m.txt
hw_ddemo --action base --cno 5000244001602 --stu all --file all --week 6 --chapter 4 > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action base --cno 5000244001602 --stu all --file all > m.txt
hw_ddemo --action base --cno 5000244001602 --stu all --file all > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action base --cno 10108001 --stu all --file 15-b5.c > m.txt
hw_ddemo --action base --cno 10108001 --stu all --file 15-b5.c > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action base --cno 10108001 --stu 2152131 --file 15-b5.c > m.txt
hw_ddemo --action base --cno 10108001 --stu 2152131 --file 15-b5.c > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

@REM Firstline
hw_check --action firstline --cno 10108001 --stu all --file all > m.txt
hw_ddemo --action firstline --cno 10108001 --stu all --file all > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action firstline --cno 10108001 --stu all --file all --week 6 --chapter 13> m.txt
hw_ddemo --action firstline --cno 10108001 --stu all --file all --week 6 --chapter 13 > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action firstline --cno 10108002 --stu all --file all --week 6 --chapter 13 > m.txt
hw_ddemo --action firstline --cno 10108002 --stu all --file all --week 6 --chapter 13 > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action firstline --cno 5000244001602 --stu all --file all --week 6 --chapter 4 > m.txt
hw_ddemo --action firstline --cno 5000244001602 --stu all --file all --week 6 --chapter 4 > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action firstline --cno 5000244001602 --stu all --file all > m.txt
hw_ddemo --action firstline --cno 5000244001602 --stu all --file all > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action firstline --cno 10108001 --stu all --file 15-b5.c > m.txt
hw_ddemo --action firstline --cno 10108001 --stu all --file 15-b5.c > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action firstline --cno 10108001 --stu 2152131 --file 15-b5.c > m.txt
hw_ddemo --action firstline --cno 10108001 --stu 2152131 --file 15-b5.c > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

@REM Secondline
hw_ddemo --action secondline --cno 10108001,10108002 --stu all --file 15-b5.c > d.txt
hw_check --action secondline --cno 10108001,10108002 --stu all --file 15-b5.c > m.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action secondline --cno 10108002 --stu all --file 15-b2.cpp > m.txt
hw_ddemo --action secondline --cno 10108002 --stu all --file 15-b2.cpp > d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

hw_check --action secondline --cno 5000244001602 --stu all --file 5-b14.c --cfgfile hw_check.conf> m.txt
hw_ddemo --action secondline --cno 5000244001602 --stu all --file 5-b14.c --cfgfile hw_check.conf> d.txt
txt_compare --file1 d.txt --file2 m.txt --trim right --display normal >>c.txt

*/

int main(int argc, char** argv)
{
	const string Action_Name[] = { "base","firstline","secondline","" }; //也可以根据需要，放入头文件中以便共享 

	args_analyse_tools args[] = {
		args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--action",   ST_EXTARGS_TYPE::str_with_set_error, 1, -1, Action_Name),	//参数-1无意义，表示无默认，必须指定
		args_analyse_tools("--cno",      ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--stu",      ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--file",     ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99), //参数-1表示不进行章节选择
		args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20), //参数-1表示不进行周次选择
		args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
		args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
		args_analyse_tools()  //最后一个，用于结束
	};


	if (checkArgs(argc, argv, args) < 0)
		return -1;

	HW_Check hc;
	if (hc.readArgs(args) < 0 ||
		hc.readConf() < 0)
		return -1;

	return hc.exe();
}

