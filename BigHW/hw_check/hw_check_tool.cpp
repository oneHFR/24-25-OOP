/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>


#include "../include/class_aat.h"
#include "../include/class_cft.h"
#include "hw_check.h"

using namespace std;
using std::min;

# define prompt_len 14

/*
                          正确 : 6926
                        未提交 : 2678
             PDF文件格式不正确 : 3
  源文件格式不正确(VS无法识别) : 1
    源文件格式不正确(非GB编码) : 290
        首行多行注释格式不正确 : 7
                首行不是注释行 : 275
                  首行不是三项 : 123
                  首行检查出错 : 1359
*/

const int
    MSG_CORRECT = 0,                    // 正确
    MSG_NOT_SUBMIT = 1,                 // 未提交
    MSG_PDF_ERROR = 2,                  // PDF文件格式错误
    MSG_VS_FORMAT_ERROR = 3,           // 源文件格式不正确(VS无法识别)
    MSG_ENCODE_ERROR = 4,               // 源文件格式不正确(非GB编码)
    MSG_MULTI_COMMENT_FORMAT_ERROR = 5, // 首行多行注释格式不正确
    MSG_FIRST_LINE_NOT_COMMENT = 6,     // 首行不是注释行
    MSG_FIRST_LINE_NOT_THREE = 7,       // 首行不是三项
    MSG_FIRST_LINE_ERROR = 8,           // 首行检查出错

    MSG_SECOND_LINE_ERROR = 9,          // 次行不是注释
    S1 = 10,
    S2 = 11, 
    S3 = 12;

const char* msg_set[] = {
    "正确",  //0
    "未提交",  //1
    "PDF文件格式不正确", //2
    "源文件格式不正确(VS无法识别)", //3
    "源文件格式不正确(非GB编码)", //4
    "首行多行注释格式不正确", //5 首行不是符合要求的/* */格式
    "首行不是注释行",  //6
    "首行不是三项", //7
    "首行检查出错",  //6 首行学号不匹配班级不匹配 [///*1953628 孙万录 计科*/]
                                    //    首行学号不匹配姓名不匹配 [///*孙万录 计科 1953628*/]

    "次行不是注释", //9
    "待查1", //10
    "待查1", //11
    "待查1", //12
};

void HW_Check::printSingleFileInfo(const vector<STU_List>& stu_list, const string& filename) {
//课号: 10108001 学生数量 : 87 源文件名 : 15 - b5.c
    if (this->Args.cno2) {
        cout << "课号 : " << this->Args.cno << "," << this->Args.cno2;
    }
    else {
        cout << "课号 : " << this->Args.cno;
    }
        cout << " 学生数量 : " << (Args.stu == -1 ? stu_list.size() : 1)
        << " 源文件名 : " << filename << endl;
}

void printStudentInfo(size_t index, const STU_List& stu, size_t hw_count)
{
    string name = stu.stu_name;
    size_t pos;
    while ((pos = name.find("·")) != string::npos) {
        name.replace(pos, strlen("·"), ".");
    }
    cout << resetiosflags(ios::right) << setiosflags(ios::left)
        << setw(3) << index << ": 学号-" << stu.stu_no << " 姓名-"
        << name << " 课号-" << stu.stu_cno
        << " 文件数量-" << hw_count << endl;
}

void printStudentStatistics(const int msg_cnt[], size_t hw_count, const char* msg_set[])
{
    int mar = 12;
    int mar2 = 2;

    // 计算最长提示文本的长度
    size_t max_width = 0;
    for (int i = 0; i < prompt_len; i++) {
        if (msg_cnt[i] > 0) {
            size_t current_len = strlen(msg_set[i]);
            if (current_len > max_width) {
                max_width = current_len;
            }
        }
    }

    cout << resetiosflags(ios::left) << setiosflags(ios::right);
    if (msg_cnt[0] == hw_count)
        cout << "全部通过";
    else
        cout << "检查通过";
    cout << msg_cnt[0] << "/" << hw_count
        << "个文件，本次通过" << msg_cnt[0] << "个\n";
    cout << string(max_width + mar, '-') << endl;  // 动态设置分隔线长度
    cout << "学生详细信息\n";
    cout << string(max_width + mar, '-') << endl;

    for (int i = 0; i < prompt_len; i++) {
        if (msg_cnt[i] > 0) {
            cout << setw(max_width + mar2) << msg_set[i] << " : " << msg_cnt[i] << endl;
        }
    }
    cout << string(max_width + mar, '-') << "\n\n";
}

void printGlobalStatistics2(size_t stu_count, size_t hw_count, const int msg_cnt[], const char* msg_set[])
{
        /*
        检查通过76/87个学生，本次通过76个
        ========================
        详细信息
        ========================
                  正确 : 76
                未提交 : 8
          次行不是注释 : 3
        ========================

        检查通过76/87个学生，本次通过87个
        ========================
        详细信息
        ========================
                  正确 : 76
                未提交 : 8
          次行不是注释 : 3
        ========================

        */
    int mar = 12;
    int mar2 = 2;

    size_t max_width = 0;
    for (int i = 0; i < prompt_len; i++) {
        if (msg_cnt[i] > 0) {
            size_t current_len = strlen(msg_set[i]);
            if (current_len > max_width) {
                max_width = current_len;
            }
        }
    }

    cout << resetiosflags(ios::left) << setiosflags(ios::right);
    if (msg_cnt[0] == stu_count)
        cout << "\n全部通过";
    else
        cout << "\n检查通过";
    cout << msg_cnt[0]  << "/"<< stu_count << "个学生，本次通过"
        << msg_cnt[0] << "个" << endl;

    cout << string(max_width + mar, '=') << endl;
    cout << "详细信息\n";
    cout << string(max_width + mar, '=') << endl;

    for (int i = 0; i < prompt_len; i++) {
        if (msg_cnt[i] > 0) {
            cout << setw(max_width + mar2) << msg_set[i] << " : " << msg_cnt[i] << endl;
        }
    }
    cout << string(max_width + mar, '=') << "\n\n";
}

void printGlobalStatistics(size_t stu_count, size_t hw_count, const int msg_cnt[], const char* msg_set[])
{
    int mar = 12;
    int mar2 = 2;
    size_t max_width = 0;
    for (int i = 0; i < prompt_len; i++) {
        if (msg_cnt[i] > 0) {
            size_t current_len = strlen(msg_set[i]);
            if (current_len > max_width) {
                max_width = current_len;
            }
        }
    }

    cout << resetiosflags(ios::left) << setiosflags(ios::right);
    cout << "\n共完成" << stu_count << "个学生的检查，文件总数:"
        << stu_count * hw_count << "，通过总数:" << msg_cnt[0]
        << "，本次通过" << msg_cnt[0] << "个" << endl;

    cout << string(max_width + mar, '=') << endl;
    cout << "整体详细信息\n";
    cout << string(max_width + mar, '=') << endl;

    for (int i = 0; i < prompt_len; i++) {
        if (msg_cnt[i] > 0) {
            cout << setw(max_width + mar2) << msg_set[i] << " : " << msg_cnt[i] << endl;
        }
    }
    cout << string(max_width + mar, '=') << "\n\n";
}

bool isBlank(const string& line) {
    return all_of(line.begin(), line.end(), isspace);
}

void trim(string& s) {
    auto isNotSpace = [](unsigned char ch) { return !isspace(ch); };
    s.erase(s.begin(), find_if(s.begin(), s.end(), isNotSpace));
    s.erase(find_if(s.rbegin(), s.rend(), isNotSpace).base(), s.end());
}

bool checkNextBytes(ifstream& file, int count)
{
    unsigned char ch;
    for (int i = 0; i < count; i++) {
        if (!file.get((char&)ch)) return false;  // 读取失败
        if ((ch & 0xc0) != 0x80) return false;   // 不是10xxxxxx格式
    }
    return true;
}

bool isUft8File(ifstream& file)
{
    file.clear();
    file.seekg(0, ios::beg);

    unsigned char ch;
    bool allASCII = true;

    while (file.get((char&)ch)) {
        if ((ch & 0x80) == 0) continue;  // 1字节

        allASCII = false;
        if ((ch & 0xe0) == 0xc0) {       // 2字节
            if (!checkNextBytes(file, 1)) return false;
        }
        else if ((ch & 0xf0) == 0xe0) {  // 3字节
            if (!checkNextBytes(file, 2)) return false;
        }
        else if ((ch & 0xf8) == 0xf0) {  // 4字节
            if (!checkNextBytes(file, 3)) return false;
        }
        else return false;
    }

    return !allASCII;  // true:是UTF-8且有非ASCII字符
}

bool isComment(const string& str) {
    size_t firstNonSpace = str.find_first_not_of(" \t");
    bool isCorrect = (firstNonSpace != string::npos && (str.substr(firstNonSpace, 2) == "//" || str.substr(firstNonSpace, 2) == "/*"));
    return isCorrect;
}

bool isMacFormat(ifstream& file) {
    file.clear();
    file.seekg(0, ios::beg);

    char c;
    int cr_count = 0, lf_count = 0;

    // 统计文件中CR和LF的数量
    while (file.get(c)) {
        if (c == '\r') cr_count++;
        if (c == '\n') lf_count++;
    }

    // Mac格式: 只有CR(\r)没有LF(\n)
    return (cr_count > 0 && lf_count == 0);
}

int removeCommentFlag(string& str) {
    //单行
    size_t sPos = str.find("//");
    if (sPos != string::npos) {
        str = str.substr(sPos + 2);
        return 0;
    }

    //多行
    size_t startPos = str.find("/*");
    size_t endPos = str.find("*/");
    if (startPos != string::npos && endPos != string::npos
        && startPos < endPos) {
        str = str.substr(startPos + 2, endPos - (startPos + 2));
        return 0;
    }

    return -1;
}

bool skipLines(ifstream& infile, int count, string& line)
{
    while (count > 0) {
        if (!getline(infile, line)) return false;
        trim(line);
        if (!isBlank(line)) count--;
    }
    return true;
}

/*___________________________________________________________________________________________________*/

static void usage(const char* const full_procname)
{

    const char* procname = strrchr(full_procname, '\\');
    if (procname == NULL)
        procname = full_procname;

    const int wkey = 7 + strlen(procname) + 1;
    const int wopt = 7 + strlen(procname) + 4;
    cout << endl;
    //args_analyse_print(args);

    cout << "Usage: " << procname << " 必选项/可选项" << endl;
    cout << endl;
    cout << setw(wkey) << ' ' << "必选项：指定操作" << endl;
    cout << setw(wopt) << ' ' << "--action opname : 支持的opname如下" << endl;
    cout << setw(wopt) << ' ' << "\t base              : 基础检查(文件是否提交、是否被改动、编码是否正确)" << endl;
    cout << setw(wopt) << ' ' << "\t firstline         : 首行检查（仅源程序文件需要）" << endl;
    cout << setw(wopt) << ' ' << "\t secondline        : 次行检查（仅部分源程序文件需要）" << endl;

    cout << setw(wkey) << ' ' << "必选项：指定课号" << endl;
    cout << setw(wopt) << ' ' << "--cno course_no     : 课号" << endl;

    cout << setw(wkey) << ' ' << "必选项：指定学生" << endl;
    cout << setw(wopt) << ' ' << "--stu no/all        : 指定单个/全部(7位数字为学号,all为全部)" << endl;

    cout << setw(wkey) << ' ' << "必选项：指定文件" << endl;
    cout << setw(wopt) << ' ' << "--file filename/all : 具体文件名/全部(all为全部,其余为具体文件名)" << endl;

    cout << setw(wkey) << ' ' << "可选项：" << endl;
    cout << setw(wopt) << ' ' << "--chapter n         : 在--file的基础上再进行章节的筛选(无/-1则全部章节,可与--week共存)" << endl;
    cout << setw(wopt) << ' ' << "--week n            : 在--file的基础上再进行周次的筛选(无/-1则全部周次,可与--chapter共存)" << endl;
    cout << setw(wopt) << ' ' << "--display xxxxx     : 每位0/1分别表示正常信息/未提交信息/错误信息/汇总信息/严重错误信息" << endl;
    cout << setw(wopt) << ' ' << "--cfgfile filename  : 指定配置文件名(缺省:hw_check.conf)" << endl;
    cout << endl;

    const char* DEMO_CNO = "10108001";
    const char* DEMO_SRC_FNAME = "12-b1.cpp";
    const char* DEMO_STUNO = "2359999";

    cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的基本信息" << endl;
    cout << endl;

    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的首行信息" << endl;
    cout << endl;

    cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : 检查" << DEMO_CNO << "的所有学生的" << DEMO_SRC_FNAME << "作业的次行信息" << endl;
    cout << endl;

    cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息(指定配置文件)" << endl;
    cout << endl;


    cout << endl;
}

int checkArgs(const int argc, const char* const argv[], args_analyse_tools args[])
{
    int cur_argc = 0;
    if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
        return -1;
    }

    if (args[OPT_ARGS_HELP].existed()) {
        args_analyse_print(args);
        usage(argv[0]);
        return -1;
    }

    if (args[OPT_ARGS_DEBUG].existed()) {
        args_analyse_print(args);
        // flag
    }

    if (args[OPT_ARGS_ACTION].existed() + args[OPT_ARGS_CNO].existed()
        + args[OPT_ARGS_STU].existed() + args[OPT_ARGS_FILE].existed() != 4) {
        usage(argv[0]);

        if (!args[OPT_ARGS_ACTION].existed()) {
            cout << "参数[" << args[OPT_ARGS_ACTION].get_name() << "]必须指定." << endl;
            return -1;
        }
        if (!args[OPT_ARGS_CNO].existed()) {
            cout << "参数[" << args[OPT_ARGS_CNO].get_name() << "]必须指定." << endl;
            return -1;
        }
        if (!args[OPT_ARGS_STU].existed()) {
            cout << "参数[" << args[OPT_ARGS_STU].get_name() << "]必须指定." << endl;
            return -1;
        }
        if (!args[OPT_ARGS_FILE].existed()) {
            cout << "参数[" << args[OPT_ARGS_FILE].get_name() << "]必须指定." << endl;
            return -1;
        }
        return -1;
    }

    if (args[OPT_ARGS_ACTION].get_string() == "secondline") {
        if (args[OPT_ARGS_FILE].get_string() == "all") {
            //HW_Check_SecondLine 只能针对单文件
            cout << "HW_Check_SecondLine 只能针对单文件" << endl;
            return -1;
        }
        if (args[OPT_ARGS_STU].get_string() != "all") {
            cout << "HW_Check_SecondLine 只能针对全体学生" << endl;
            return -1;
        }
    }

    return 0;
}

int HW_Check::readArgs(args_analyse_tools args[])
{
    const string Action_Name[] = { "base","firstline","secondline","" };
    string args_action = args[OPT_ARGS_ACTION].get_string();
    int index = 0;
    while (Action_Name[index] != "") {
        if (Action_Name[index] == args_action) {
            this->Args.action = index;
            break;
        }
        ++index;
    }

    string cno_str = args[OPT_ARGS_CNO].get_string();
    int helper = 1;
    if (cno_str.length() != 8 && cno_str.length() != 13) {
        helper = -1;
        if (this->Args.action == 2) {
            if (cno_str.length() == 17) {
                helper = 2;
                stringstream ss(cno_str);
                string number1, number2;
                if (getline(ss, number1, ',') && getline(ss, number2)) {
                    this->Args.cno = stoll(number1);
                    this->Args.cno2 = stoll(number2);
                }
                //cout << this->Args.cno << endl;;
                //cout << this->Args.cno2 <<endl;
            }
        }
        if (helper == -1) {
            cout << "课号不是8/13位" << endl;
            return helper;
        }
    }
    if (helper != 2) {
        this->Args.cno = stoll(cno_str);
    }

    string stu_tmp = args[OPT_ARGS_STU].get_string();
    if ("all" == stu_tmp) {
        this->Args.stu = -1; 
    }
    else {
        this->Args.stu = atoi(stu_tmp.c_str()); // 		stu	0x0020d6c3	int
    }

    this->Args.file = args[OPT_ARGS_FILE].get_string();
    this->Args.chapter = args[OPT_ARGS_CHAPTER].get_int();
    this->Args.week = args[OPT_ARGS_WEEK].get_int();
    this->Args.display = args[OPT_ARGS_DISPLAY].get_string();
    if (this->Args.display.length() != 5) {
        cout << "参数[--display]的长度不正确，只能是[5]位的0/1." << endl;
        return -1;
    }
    for (int i = 0; i < 5; ++i) {
        char c = this->Args.display[i];
        if (c != '0' && c != '1') {
            //参数[--display]的长度不正确，只能是[5]位的0 / 1.
            cout << "参数[--display]的长度不正确，只能是[5]位的0/1." << endl;
            return -1;
        }
    }
    this->Args.cfgfile = args[OPT_ARGS_CFGFILE].get_string();
    return 0;
}

int HW_Check::readConf()
{
    config_file_tools cfg(this->Args.cfgfile);
    if (cfg.is_read_succeeded() == 0) {
        cout << "配置文件 [" << this->Args.cfgfile << "] 无法打开." << endl;
        return -1;
    }
    if (1) {
        const char* gname = "[文件目录]";
        const char* iname = "src_rootdir";
        const char* def_iname = "../../24251-source/source/"; //flag
        if (cfg.item_get_string(gname, iname, this->config.src_rootdir, def_iname, false, false) < 0) {
            cout << "取配置文件" << iname << "项的值出错." << endl;
            return -1;
        }
    }
    if (1) {
        const char* gname = "[数据库]";
        const char* iname[] = {
            "db_host","db_username","db_passwd","db_name",
        };
        const char* def_iname[] = {
            "10.80.42.244","hwcheck","hw_CheCk-For23241*oOP","homework"
        };
        if (cfg.item_get_string(gname, iname[0], this->config.dbserver, def_iname[0], false, false) < 0) {
            cout << "取配置文件" << iname[0] << "项的值出错." << endl;
            return -1;
        }
        if (cfg.item_get_string(gname, iname[1], this->config.dbuser, def_iname[1], false, false) < 0) {
            cout << "取配置文件" << iname[1] << "项的值出错." << endl;
            return -1;
        }
        if (cfg.item_get_string(gname, iname[2], this->config.dbpasswd, def_iname[2], false, false) < 0) {
            cout << "取配置文件" << iname[2] << "项的值出错." << endl;
            return -1;
        }
        if (cfg.item_get_string(gname, iname[3], this->config.dbname, def_iname[3], false, false) < 0) {
            cout << "取配置文件" << iname[3] << "项的值出错." << endl;
            return -1;
        }
    }
    return 0;
}

/*___________________________________________________________________________________________________*/

int HW_Check::checkInfoCheck1(const string& str, const STU_List& stu, First_Check_Info& info)
{

    if (!isComment(str)) {
        return MSG_FIRST_LINE_NOT_COMMENT;
    }
    // 第二步：提取注释内容（去掉/*和*/）
    size_t firstNonSpace = str.find_first_not_of(" \t");
    bool isCorrect = (firstNonSpace != string::npos && str.substr(firstNonSpace, 2) == "//");
    if (!isCorrect) {
        size_t start = str.find("/*");
        size_t end = str.find("*/");
        if (end == -1) {
            info.msg_info = "首行不是符合要求的/* */格式";
            return MSG_MULTI_COMMENT_FORMAT_ERROR;
        }

        string after_comment = str.substr(end + 2);

        // 查找第一个非空白字符的位置
        if (after_comment.find_first_not_of(" \t\n\r\f\v") != string::npos) {
            info.msg_info = "首行不是符合要求的/* */格式";
            return MSG_MULTI_COMMENT_FORMAT_ERROR;
        }
    }



    string content = str;
    if (removeCommentFlag(content) < 0) {
        cerr << "移除注释标记错误" << endl;
        exit(-1);
    }

    // 第三步：用空格分割内容，计算项数
    vector<string> tokens;
    istringstream iss(content);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    // 检查是否正好是三项
    if (tokens.size() != 3) {
        return MSG_FIRST_LINE_NOT_THREE;
    }

    // 第四步：检查三项是否匹配（不考虑顺序）
    bool found_no = false, found_name = false, found_class = false;

    for (const string& token : tokens) {
        if (token == stu.stu_no) {
            found_no = true;
            info.token_ok[0] = true;
        }
        else if (token == stu.stu_name) {
            found_name = true;
            info.token_ok[1] = true;
        }
        else if (token == stu.stu_class_sname || token == stu.stu_class_fname ||
            stu.stu_class_sname.find(token) != string::npos) {
            found_class = true;
            info.token_ok[2] = true;
        }
    }

    // 生成错误信息
    if (!found_no || !found_name || !found_class) {
        info.msg_info = "首行";
        if (!found_no) info.msg_info += "学号不匹配";
        if (!found_name) info.msg_info += "姓名不匹配";
        if (!found_class) info.msg_info += "班级不匹配";
        if (!info.msg_info.empty() && info.msg_info.back() == ' ') {
            info.msg_info.pop_back();
        }
        return MSG_FIRST_LINE_ERROR;
    }

    return MSG_CORRECT;
}

int HW_Check::checkInfoCheck2(vector<Second_Check>& list)
{
    cout << "交叉检查结果：" << endl;
    cout << resetiosflags(ios::right) << setiosflags(ios::left);

    for (auto& student : list) {
        if (!student.isValid) {
            continue;
        }

        // 检查互查关系
        for (size_t i = 0; i < student.check_list.size(); ++i) {
            if (int(i) > student.cutline)
                break;
            auto& partner = student.check_list[i];
            bool partner_found = false;

            // 查找并验证对方学生信息
            for (const auto& other : list) {

                if (other.stu_no == partner.stu_no) {
                    partner_found = true;

                    // 检查姓名一致性
                    if (partner.stu_name != other.stu_name) {
                        partner.msg = "对方姓名不正确  ";
                    }

                    // 检查对方是否也选择了我
                    bool mutually_selected = false;
                    for (size_t j = 0; j < other.check_list.size(); ++j) {
                        if (int(j) > other.cutline)
                            break;
                        if (other.check_list[j].stu_no == student.stu_no) {
                            mutually_selected = true;
                            if (other.check_list[j].stu_name != student.stu_name) {
                                partner.msg = "没写对你名字  ";
                                if (!other.isValid) {
                                    partner.msg = "抛弃了你  ";
                                }
                            }
                            break;
                        }
                    }

                    if (!mutually_selected) {
                        if(partner.msg == "" || partner.msg == "没写对你名字  ")
                            partner.msg = "抛弃了你  ";
                    }
                    break;
                }
            }
            if (!partner_found) {
                partner.msg += "对方学号不存在  ";
            }
        }
    }




    // 打印
    for (size_t i = 0; i < list.size(); i++) {
        if (i + 1 == 120)
            int a = 1;
        auto& student = list[i];
        //bool has_duplicate = false;
        int more_Safe = 0;

        for (size_t i = 0; i < student.check_list.size(); ++i) {
            for (size_t j = i + 1; j < student.check_list.size(); ++j) {
                if (student.check_list[i].stu_name == student.check_list[j].stu_no || student.check_list[i].stu_no == student.check_list[j].stu_no) {
                    if (student.check_list.size() > 2) {
                        more_Safe++;
                        if (more_Safe >= 2) {
                            student.has_duplicate = true;
                        }
                    }
                    else
                        student.has_duplicate = true;
                    break;
                }
            }
            if (student.has_duplicate) break;
        }

        string name = student.stu_name;
        size_t pos;
        if (this->Args.cno != 5000244001602) {
            while ((pos = name.find("·")) != string::npos)
                name.replace(pos, strlen("·"), ".");
        }
        cout << setw(3) << i + 1 << ": " << student.stu_cno << '-' << student.stu_no << '-' << name << endl;
        if (student.isValid && student.second_safe && student.has_duplicate) {
            size_t half_size = (1 + student.check_list.size()) / 2; // 获取列表的一半大小
            int half_size_final = std::min(int(half_size), student.cutline);
            if (half_size_final >= student.cutline) {
                half_size = 0;
            }
            for (size_t j = half_size; j < half_size + half_size_final; ++j) {
                if (j >= student.check_list.size())
                    break;
                const auto& partner = student.check_list[j];
                if (i + 1 == 120)
                    int a = 1;
                if (partner.stu_no != student.stu_no && partner.stu_name != student.stu_no) {
                    string temp_stu_no = partner.stu_no;
                    removeCommentFlag(temp_stu_no);
                    cout << '\t' << temp_stu_no << ' ' << partner.stu_name
                        << '\t' << partner.msg << endl;
                }
            }
        }

        else if (student.isValid && student.second_safe && !student.has_duplicate){
            size_t half_size = student.check_list.size(); // 获取列表的一半大小
            half_size = size_t(std::min(int(half_size), student.cutline));
            for (size_t j = 0; j < half_size; ++j) {
                const auto& partner = student.check_list[j];
                if (partner.stu_no != student.stu_no) {
                    string temp_stu_no = partner.stu_no;
                    removeCommentFlag(temp_stu_no);
                    cout << '\t' << temp_stu_no << ' ' << partner.stu_name
                        << '\t' << partner.msg << endl;
                }
            }
        }
    }
    cout << endl;
    return 0;
}

int HW_Check::baseCheck(ifstream& infile, string& filename, int& imsg)
{

    infile.clear();
    infile.seekg(0, ios::beg);

    string extension = filename.substr(filename.find_last_of(".") + 1);

    if (find(extention.begin(), extention.end(), extension) != extention.end()) {
        if (isMacFormat(infile)) {
            imsg = MSG_VS_FORMAT_ERROR;
            return -1;
        }
    }

    // 源代码文件检查
    if (find(extention.begin(), extention.end(), extension) != extention.end())
    {
        if (isUft8File(infile)) {
            //second_check.isValid = false;
            imsg = MSG_ENCODE_ERROR;
            return -1;
        }
        return 0;
    }

    // PDF文件检查 
    if (extension == "pdf") {
        infile.clear();
        infile.seekg(0, ios::beg);

        const char PDF_HEADER[] = "%PDF-1.x";
        char buf[9] = { 0 };
        infile.read(buf, 8);

        if (strncmp(buf, PDF_HEADER, 7) != 0 || !isdigit(buf[7])) {
            imsg = MSG_PDF_ERROR;
            return -1;
        }
    }

    return 0;
}

int HW_Check::firstlineCheck(ifstream& infile, const STU_List& stu, int& imsg, First_Check_Info& info)
{
    infile.clear();
    infile.seekg(0, ios::beg);

    string firstLine;
    skipLines(infile, 1, firstLine);

    if (!firstLine.empty() && firstLine.back() == '\r') {
        firstLine.pop_back();
    }

    info.first_line = firstLine;

    if (!isComment(firstLine)) {
        imsg = MSG_FIRST_LINE_NOT_COMMENT;
        return -1;
    }

    if ((imsg = checkInfoCheck1(firstLine, stu, info)) != 0) {
        return -1;
    }

    return 0;
}

int HW_Check::secondlineCheck(ifstream& infile, string& filename, Second_Check& second_check, int& imsg, First_Check_Info& info) {
    const int STU_NO_LENGTH = 7;
    infile.clear();
    infile.seekg(0, ios::beg);
    string secondLine;
    skipLines(infile, 2, secondLine);

    string extension = filename.substr(filename.find_last_of(".") + 1);
    if (find(extention.begin(), extention.end(), extension) != extention.end())
    {
        if (isUft8File(infile)) {
            second_check.isValid = false;
            imsg = MSG_ENCODE_ERROR;
            return -1;
        }
    }

    if (!isComment(secondLine)) {
        second_check.isValid = false;
        imsg = MSG_SECOND_LINE_ERROR;
        return -1;
    }

    if (removeCommentFlag(secondLine) < 0) {
        cerr << "移除注释标记错误" << endl;
        exit(-1);
    }

    istringstream stream(secondLine);
    string studentNo, studentName;
    int count = 0;

    //while (stream >> (count % 2 ? studentName : studentNo)) {
    //    count++;
    //    if (count % 2 == 1) {
    //        // 检查学号长度
    //        if (studentNo.length() != STU_NO_LENGTH) {
    //            //imsg = MSG_SECOND_LINE_ERROR;
    //            //second_check.isValid = false;
    //            info.msg_info = "第" + to_string((count + 1) / 2) + "位同学的学号[" + studentNo + "]不是" + to_string(STU_NO_LENGTH) + "位，后续内容忽略";

    //            second_check.cutline = ((count + 1) / 2) - 1;
    //            second_check.second_safe = true;
    //            return -1;
    //        }
    //        // 检查是否写了自己
    //        if (studentNo == second_check.stu_no) {
    //            //imsg = MSG_SECOND_LINE_ERROR;
    //            //second_check.isValid = false;
    //            info.msg_info = "第[" + to_string((count + 1) / 2) + "]项写了自己，后续内容忽略";
    //            second_check.second_safe = true;
    //            return -1;
    //        }
    //    }
    //    else {
    //        second_check.check_list.push_back({ studentNo, studentName, "" });
    //    }
    //info.msg_info = "第[" + to_string((count + 1) / 2) + "]个学生信息不完整，后续内容忽略";

    while (stream >> (count % 2 ? studentName : studentNo)) {

        count++;

        // 检查是否只读到一项（奇数项）
        if (count % 2 == 1 && stream.eof()) {
            info.msg_info = "第[" + to_string((count + 1) / 2 -1) + "]个学生后面的信息不全(只读到一项)，后续内容忽略";
            second_check.cutline = ((count + 1) / 2) - 1;
            second_check.second_safe = true;
            return -1;
        }

        if (count % 2 == 1) {
            // 先检查学号中是否包含非法字符（非数字字符）
            if (!all_of(studentNo.begin(), studentNo.end(), [](unsigned char c) {
                return isdigit(c);
                })) {
                //info.msg_info = "第" + to_string((count + 1) / 2) + "位同学的学号[" + studentNo + "]含有非数字字符，后续内容忽略";
                info.msg_info = "第" + to_string((count + 1) / 2) + "位同学的学号[" + studentNo + "]不是" + to_string(STU_NO_LENGTH) + "位，后续内容忽略";

                second_check.cutline = ((count + 1) / 2) - 1;
                second_check.second_safe = true;
                return -1;
            }
            // 再检查学号长度
            if (studentNo.length() != STU_NO_LENGTH) {
                info.msg_info = "第" + to_string((count + 1) / 2) + "位同学的学号[" + studentNo + "]不是" + to_string(STU_NO_LENGTH) + "位，后续内容忽略";
                second_check.cutline = ((count + 1) / 2) - 1;
                second_check.second_safe = true;
                return -1;
            }
            // 检查是否写了自己
            if (studentNo == second_check.stu_no) {
                info.msg_info = "第[" + to_string((count + 1) / 2) + "]项写了自己，后续内容忽略";
                second_check.second_safe = true;
                return -1;
            }
        }
        
        else {
            // 检查学生姓名是否为空
            if (studentName.empty()) {
                info.msg_info = "第[" + to_string(count / 2 - 1) + "]个学生姓名为空，后续内容忽略";
                second_check.cutline = (count / 2) - 1;
                second_check.second_safe = true;
                return -1;
            }
            second_check.check_list.push_back({ studentNo, studentName, "" });
        }
    }

    // 检查最后一次读取是否完整（是否读到了偶数项）
    if (count % 2 == 1) {
        info.msg_info = "第[" + to_string((count + 1) / 2) + "]个学生后面的信息不全(只读到一项)，后续内容忽略";
        second_check.cutline = ((count + 1) / 2) - 1;
        second_check.second_safe = true;
        return -1;
    }



    second_check.second_safe = true;
    return 0;
}

void HW_Check::processHomework(vector<Second_Check>& second_check_list, const STU_List& stu, const HW_List& hw, int msg_cnt_glb[], int msg_cnt_stu[], const char* msg_set[], First_Check_Info& info)
{
    if (hw.hw_cno != stu.stu_cno)
        return ;
    static int index4s = 1;

    //auto it = find_if(second_check_list.begin(), second_check_list.end(),
    //    [&stu](const Second_Check& check) {
    //        return check.stu_no == stu.stu_no;
    //    });

    // 创建新的 Second_Check 对象
    Second_Check new_check;
    new_check.stu_no = stu.stu_no;
    new_check.stu_name = stu.stu_name;
    new_check.stu_cno = stu.stu_cno;

    bool found = false;
    size_t check_index = 0;

    for (size_t i = 0; i < second_check_list.size(); i++) {
        if (second_check_list[i].stu_no == stu.stu_no) {
            found = true;
            check_index = i;
            break;
        }
    }

    if (!found) {
        second_check_list.push_back(new_check);
        check_index = second_check_list.size() - 1;
    }


    string cur_filename = this->config.src_rootdir + hw.hw_cno + "-" + stu.stu_no + "\\" + hw.hw_filename;
    ifstream infile(cur_filename, ios::in | ios::binary);

    int imsg = 0;
    string msg = msg_set[imsg];

    if (!infile.is_open()) {
        imsg = 1;
        msg = msg_set[imsg];
        msg_cnt_glb[imsg]++;
        msg_cnt_stu[imsg]++;
    }
    else {
        // 基础检查
        if (baseCheck(infile, cur_filename, imsg) < 0) {
            //second_check.isValid = false;
            //it->isValid = false;  // 设置 isValid 为 false
            second_check_list[check_index].isValid = false;  // 使用索引直接访问
            msg = msg_set[imsg];
            msg_cnt_glb[imsg]++;
            msg_cnt_stu[imsg]++;
        }
        else if (this->Args.action == 1) {  // 首行检查
            if (firstlineCheck(infile, stu, imsg, info) < 0) {
                if (imsg == 5 || imsg == 8) {
                    msg = info.msg_info + " [" + info.first_line + "]";
                }
                else if (imsg == 8 || imsg == 6 || imsg == 7) {
                    msg = string(msg_set[imsg]) + " [" + info.first_line + "]";
                }
                else {
                    msg = msg_set[imsg];
                }
                msg_cnt_glb[imsg]++;
                msg_cnt_stu[imsg]++;
            }
        }
        else if (this->Args.action == 2) {  // 第二行检查
            for (auto& it : second_check_list) {
                if (it.stu_no == stu.stu_no) {
                    if (secondlineCheck(infile, cur_filename, it, imsg, info) < 0) {
                        msg = info.msg_info;
                        if (msg == "")
                            msg = string(msg_set[imsg]);

                    }
                    break;
                }
            }
            msg_cnt_glb[imsg]++;
            msg_cnt_stu[imsg]++;
        }
        infile.close();
    }

    if (imsg == 0 && this->Args.action != 2) {
        msg_cnt_glb[imsg]++;
        msg_cnt_stu[imsg]++;
    }

    // 根据display配置打印信息
    if (this->Args.action != 2 && this->Args.file == "all") {
        if (!((imsg == 0 && this->Args.display[0] == '0') ||
            (imsg == 1 && this->Args.display[1] == '0') ||
            (imsg >= 2 && this->Args.display[2] == '0'))) {
            cout << setiosflags(ios::left);
            int width = 28;
            //if ()
            if (this->Args.cno == 5000244001602)
                width++;
            if (this->Args.week != -1 || this->Args.chapter != -1) {
                if (this->Args.cno == 5000244001602) {
                    width = 11;
                }
                else {
                    width = 10;
                }
            }

            cout << "  " << setw(width) << hw.hw_filename << ": " << msg << endl;
        }
    }
    else {
        int mar = 24;
        string name = stu.stu_name;
        size_t pos;
        // 处理姓名中的点号
        while ((pos = name.find("·")) != string::npos) {
            name.replace(pos, strlen("·"), ".");
        }
        if (this->Args.cno != 5000244001602) {
            mar = 16;
            int width = 1;
        }
        // 计算学号/姓名组合的总长度并设置固定宽度
        string student_info = stu.stu_no + "/" + name;
        cout << resetiosflags(ios::right) << setiosflags(ios::left)
            << setw(3) << index4s << ": "
            << setw(mar) << student_info  // 使用固定宽度16对齐
            << " : " << msg << endl;     // 保持冒号前后的间距
        index4s++;
    }
    

} 

/*___________________________________________________________________________________________________*/


int HW_Check::exe()
{
    // 存储数据的向量
    vector<STU_List> stu_list;
    vector<HW_List> hw_list;
    vector<Second_Check> second_check_list;
    if (get_data(stu_list, hw_list, second_check_list) < 0)
        return -1;

    // 格式化输出相关常量
    const char* INDENT = "  ";
    const int FILENAME_WIDTH = 28, STU_INFO_WIDTH = 8;

    // 统计数组
    int msg_cnt_glb[prompt_len] = { 0 };

    if (this->Args.action == 2 || this->Args.file != "all") {
        // 遍历作业列表找到目标文件
        for (const auto& hw : hw_list) {
            printSingleFileInfo(stu_list, hw.hw_filename);
            break;  // 找到目标文件后退出循环

        }
    }

    // 遍历每个学生
    for (size_t stu_i = 0; stu_i < stu_list.size(); stu_i++) {
        const auto& stu = stu_list[stu_i];
        int msg_cnt_stu[prompt_len] = { 0 };
        First_Check_Info info;

        // 打印学生基本信息
        if (this->Args.action != 2 && this->Args.file == "all")
            printStudentInfo(stu_i + 1, stu, hw_list.size());


        // 遍历每个作业
        for (const auto& hw : hw_list) {
            if (hw.hw_filename == "3-b1-1.c")
                int a = 0; // flag
            //if(a)
            //cout << second_check_list.size() << endl;
            processHomework(second_check_list, stu, hw, msg_cnt_glb, msg_cnt_stu, msg_set, info);
        }

        // 打印学生详细统计
        if (this->Args.action != 2 && this->Args.display[3] != '0' && this->Args.file == "all") {
            printStudentStatistics(msg_cnt_stu, hw_list.size(), msg_set);
        }
    }

    // 打印全局统计
    if (this->Args.action != 2 && this->Args.file == "all")
        printGlobalStatistics(stu_list.size(), hw_list.size(), msg_cnt_glb, msg_set);
    else
        printGlobalStatistics2(stu_list.size(), hw_list.size(), msg_cnt_glb, msg_set);

    // 第二行检查
    if (this->Args.action == 2) {
        checkInfoCheck2(second_check_list);
    }

    return 0;
}


