/* 2152131 吴洪蕊 计科 */

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
    //3不要写死，但没法在结构体内引用常量
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
    // 命令行参数 
    struct Args {
        int action = 0;     // 0:base 1:firstline 2:secondline  
        long long cno = 10108001;        // 课号
        int stu = -1;        // -1表示all
        string file = "all";    // all或具体文件名
        int chapter = -1;    // -1表示all
        int week = -1;       // -1表示all
        string display = "111111"; // 显示控制
        string cfgfile = "hw_check.conf"; // 配置文件
    } Args;

    // 配置信息
    struct Config {
        string src_rootdir;  // 源文件根目录
        string dbserver;     // 数据库服务器
        string dbuser;       // 数据库用户名
        string dbpasswd;     // 数据库密码
        string dbname;       // 数据库名
    } config;

    const vector<string> extention{
        "h",
        "hpp",
        "c",
        "cpp"
    };

public:
    int get_data(vector<STU_List>& stu_list, vector<HW_List>& hw_list, vector<Second_Check>& second_list);
    int readArgs(args_analyse_tools args[]);  // 参数已实现
    int readConf(); // 配置已实现
    int exe();

    // 三种检查的具体实现
    int baseCheck(ifstream& infile, string& filename, int& imsg);
    int firstlineCheck(ifstream& infile, const STU_List& stu, int& imsg, First_Check_Info& info);
    int secondlineCheck(ifstream& infile, Second_Check& second_check, int& imsg, First_Check_Info& info);

    int checkInfoCheck1(const string& str, const STU_List& stu, First_Check_Info& info);
    //int checkFirstLineFormat(const string& line, vector<string>& tokens);
    int checkInfoCheck2(vector<Second_Check>& list);

    void processHomework(vector<Second_Check>& list, const STU_List& stu, const HW_List& hw, int msg_cnt_glb[], int msg_cnt_stu[], const char* msg_set[], First_Check_Info& info);
    void printSingleFileInfo(const vector<STU_List>& stu_list, const string& filename);
};