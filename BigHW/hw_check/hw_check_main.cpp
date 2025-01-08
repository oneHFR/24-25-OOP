/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include "../include/class_aat.h"
#include "hw_check.h"

// �Բ�ű�
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
	const string Action_Name[] = { "base","firstline","secondline","" }; //Ҳ���Ը�����Ҫ������ͷ�ļ����Ա㹲�� 

	args_analyse_tools args[] = {
		args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--action",   ST_EXTARGS_TYPE::str_with_set_error, 1, -1, Action_Name),	//����-1�����壬��ʾ��Ĭ�ϣ�����ָ��
		args_analyse_tools("--cno",      ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--stu",      ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--file",     ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99), //����-1��ʾ�������½�ѡ��
		args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20), //����-1��ʾ�������ܴ�ѡ��
		args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
		args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
		args_analyse_tools()  //���һ�������ڽ���
	};


	if (checkArgs(argc, argv, args) < 0)
		return -1;

	HW_Check hc;
	if (hc.readArgs(args) < 0 ||
		hc.readConf() < 0)
		return -1;

	return hc.exe();
}

