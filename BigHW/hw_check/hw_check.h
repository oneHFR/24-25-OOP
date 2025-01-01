/* 2152131 ����� �ƿ� */

#pragma once
#include "../include/class_aat.h"
#include "../include/class_cft.h"
#include "../include_mariadb_x86/mysql/mysql.h"
#include <fstream>
#include <string>
#include <vector>


static void usage(const char* const full_procname);
int checkArgs(const int argc, const char* const argv[], args_analyse_tools args[]);



enum OPT_ARGS {
    OPT_ARGS_HELP = 0,
    OPT_ARGS_DEBUG,
    OPT_ARGS_ACTION,
    OPT_ARGS_CNO,
    OPT_ARGS_STU,
    OPT_ARGS_FILE,
    OPT_ARGS_CHAPTER,
    OPT_ARGS_WEEK,
    OPT_ARGS_DISPLAY,
    OPT_ARGS_CFGFILE,
};

struct First_Check_Info {
    //const int token_num = 3;
    //3��Ҫд������û���ڽṹ�������ó���
    string msg_info;
    bool token_ok[3];	// no class name
    string first_line;
};

//second line stu info
struct Second_Check_Stu {
    string stu_no;
    string stu_name;
    string msg;
};

//second line check info
struct Second_Check {
    bool isValid = true;
    bool second_safe = false;
    int cutline = 1024;
    bool has_duplicate = false;
    string stu_no;
    string stu_name;
    string stu_cno;
    vector<Second_Check_Stu> check_list;
};

struct STU_List {
    string stu_no;
    string stu_name;
    string stu_class_fname;
    string stu_class_sname;
    string stu_cno;
};

struct HW_List {
    string hw_cno;
    string hw_chapter;
    string hw_week;
    string hw_filename;
};

class HW_Check {
protected:
    // �����в��� 
    struct Args {
        int action = 0;     // 0:base 1:firstline 2:secondline  
        long long cno = 10108001;        // �κ�
        int stu = -1;        // -1��ʾall
        string file = "all";    // all������ļ���
        int chapter = -1;    // -1��ʾall
        int week = -1;       // -1��ʾall
        string display = "111111"; // ��ʾ����
        string cfgfile = "hw_check.conf"; // �����ļ�
    } Args;

    // ������Ϣ
    struct Config {
        string src_rootdir;  // Դ�ļ���Ŀ¼
        string dbserver;     // ���ݿ������
        string dbuser;       // ���ݿ��û���
        string dbpasswd;     // ���ݿ�����
        string dbname;       // ���ݿ���
    } config;

    const vector<string> extention{
        "h",
        "hpp",
        "c",
        "cpp"
    };

public:
    int get_data(vector<STU_List>& stu_list, vector<HW_List>& hw_list, vector<Second_Check>& second_list);
    int readArgs(args_analyse_tools args[]);  // ������ʵ��
    int readConf(); // ������ʵ��
    int exe();

    // ���ּ��ľ���ʵ��
    int baseCheck(ifstream& infile, string& filename, int& imsg);
    int firstlineCheck(ifstream& infile, const STU_List& stu, int& imsg, First_Check_Info& info);
    int secondlineCheck(ifstream& infile, Second_Check& second_check, int& imsg, First_Check_Info& info);

    int checkInfoCheck1(const string& str, const STU_List& stu, First_Check_Info& info);
    //int checkFirstLineFormat(const string& line, vector<string>& tokens);
    int checkInfoCheck2(vector<Second_Check>& list);

    void processHomework(vector<Second_Check>& list, const STU_List& stu, const HW_List& hw, int msg_cnt_glb[], int msg_cnt_stu[], const char* msg_set[], First_Check_Info& info);
    void printSingleFileInfo(const vector<STU_List>& stu_list, const string& filename);
};