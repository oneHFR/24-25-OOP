/* 2152131 ����� �ƿ� */
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
                          ��ȷ : 6926
                        δ�ύ : 2678
             PDF�ļ���ʽ����ȷ : 3
  Դ�ļ���ʽ����ȷ(VS�޷�ʶ��) : 1
    Դ�ļ���ʽ����ȷ(��GB����) : 290
        ���ж���ע�͸�ʽ����ȷ : 7
                ���в���ע���� : 275
                  ���в������� : 123
                  ���м����� : 1359
*/

const int
    MSG_CORRECT = 0,                    // ��ȷ
    MSG_NOT_SUBMIT = 1,                 // δ�ύ
    MSG_PDF_ERROR = 2,                  // PDF�ļ���ʽ����
    MSG_VS_FORMAT_ERROR = 3,           // Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)
    MSG_ENCODE_ERROR = 4,               // Դ�ļ���ʽ����ȷ(��GB����)
    MSG_MULTI_COMMENT_FORMAT_ERROR = 5, // ���ж���ע�͸�ʽ����ȷ
    MSG_FIRST_LINE_NOT_COMMENT = 6,     // ���в���ע����
    MSG_FIRST_LINE_NOT_THREE = 7,       // ���в�������
    MSG_FIRST_LINE_ERROR = 8,           // ���м�����

    MSG_SECOND_LINE_ERROR = 9,          // ���в���ע��
    S1 = 10,
    S2 = 11, 
    S3 = 12;

const char* msg_set[] = {
    "��ȷ",  //0
    "δ�ύ",  //1
    "PDF�ļ���ʽ����ȷ", //2
    "Դ�ļ���ʽ����ȷ(VS�޷�ʶ��)", //3
    "Դ�ļ���ʽ����ȷ(��GB����)", //4
    "���ж���ע�͸�ʽ����ȷ", //5 ���в��Ƿ���Ҫ���/* */��ʽ
    "���в���ע����",  //6
    "���в�������", //7
    "���м�����",  //6 ����ѧ�Ų�ƥ��༶��ƥ�� [///*1953628 ����¼ �ƿ�*/]
                                    //    ����ѧ�Ų�ƥ��������ƥ�� [///*����¼ �ƿ� 1953628*/]

    "���в���ע��", //9
    "����1", //10
    "����1", //11
    "����1", //12
};

void HW_Check::printSingleFileInfo(const vector<STU_List>& stu_list, const string& filename) {
//�κ�: 10108001 ѧ������ : 87 Դ�ļ��� : 15 - b5.c
    if (this->Args.cno2) {
        cout << "�κ� : " << this->Args.cno << "," << this->Args.cno2;
    }
    else {
        cout << "�κ� : " << this->Args.cno;
    }
        cout << " ѧ������ : " << (Args.stu == -1 ? stu_list.size() : 1)
        << " Դ�ļ��� : " << filename << endl;
}

void printStudentInfo(size_t index, const STU_List& stu, size_t hw_count)
{
    string name = stu.stu_name;
    size_t pos;
    while ((pos = name.find("��")) != string::npos) {
        name.replace(pos, strlen("��"), ".");
    }
    cout << resetiosflags(ios::right) << setiosflags(ios::left)
        << setw(3) << index << ": ѧ��-" << stu.stu_no << " ����-"
        << name << " �κ�-" << stu.stu_cno
        << " �ļ�����-" << hw_count << endl;
}

void printStudentStatistics(const int msg_cnt[], size_t hw_count, const char* msg_set[])
{
    int mar = 12;
    int mar2 = 2;

    // �������ʾ�ı��ĳ���
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
        cout << "ȫ��ͨ��";
    else
        cout << "���ͨ��";
    cout << msg_cnt[0] << "/" << hw_count
        << "���ļ�������ͨ��" << msg_cnt[0] << "��\n";
    cout << string(max_width + mar, '-') << endl;  // ��̬���÷ָ��߳���
    cout << "ѧ����ϸ��Ϣ\n";
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
        ���ͨ��76/87��ѧ��������ͨ��76��
        ========================
        ��ϸ��Ϣ
        ========================
                  ��ȷ : 76
                δ�ύ : 8
          ���в���ע�� : 3
        ========================

        ���ͨ��76/87��ѧ��������ͨ��87��
        ========================
        ��ϸ��Ϣ
        ========================
                  ��ȷ : 76
                δ�ύ : 8
          ���в���ע�� : 3
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
        cout << "\nȫ��ͨ��";
    else
        cout << "\n���ͨ��";
    cout << msg_cnt[0]  << "/"<< stu_count << "��ѧ��������ͨ��"
        << msg_cnt[0] << "��" << endl;

    cout << string(max_width + mar, '=') << endl;
    cout << "��ϸ��Ϣ\n";
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
    cout << "\n�����" << stu_count << "��ѧ���ļ�飬�ļ�����:"
        << stu_count * hw_count << "��ͨ������:" << msg_cnt[0]
        << "������ͨ��" << msg_cnt[0] << "��" << endl;

    cout << string(max_width + mar, '=') << endl;
    cout << "������ϸ��Ϣ\n";
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
        if (!file.get((char&)ch)) return false;  // ��ȡʧ��
        if ((ch & 0xc0) != 0x80) return false;   // ����10xxxxxx��ʽ
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
        if ((ch & 0x80) == 0) continue;  // 1�ֽ�

        allASCII = false;
        if ((ch & 0xe0) == 0xc0) {       // 2�ֽ�
            if (!checkNextBytes(file, 1)) return false;
        }
        else if ((ch & 0xf0) == 0xe0) {  // 3�ֽ�
            if (!checkNextBytes(file, 2)) return false;
        }
        else if ((ch & 0xf8) == 0xf0) {  // 4�ֽ�
            if (!checkNextBytes(file, 3)) return false;
        }
        else return false;
    }

    return !allASCII;  // true:��UTF-8���з�ASCII�ַ�
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

    // ͳ���ļ���CR��LF������
    while (file.get(c)) {
        if (c == '\r') cr_count++;
        if (c == '\n') lf_count++;
    }

    // Mac��ʽ: ֻ��CR(\r)û��LF(\n)
    return (cr_count > 0 && lf_count == 0);
}

int removeCommentFlag(string& str) {
    //����
    size_t sPos = str.find("//");
    if (sPos != string::npos) {
        str = str.substr(sPos + 2);
        return 0;
    }

    //����
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

    cout << "Usage: " << procname << " ��ѡ��/��ѡ��" << endl;
    cout << endl;
    cout << setw(wkey) << ' ' << "��ѡ�ָ������" << endl;
    cout << setw(wopt) << ' ' << "--action opname : ֧�ֵ�opname����" << endl;
    cout << setw(wopt) << ' ' << "\t base              : �������(�ļ��Ƿ��ύ���Ƿ񱻸Ķ��������Ƿ���ȷ)" << endl;
    cout << setw(wopt) << ' ' << "\t firstline         : ���м�飨��Դ�����ļ���Ҫ��" << endl;
    cout << setw(wopt) << ' ' << "\t secondline        : ���м�飨������Դ�����ļ���Ҫ��" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�ָ���κ�" << endl;
    cout << setw(wopt) << ' ' << "--cno course_no     : �κ�" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�ָ��ѧ��" << endl;
    cout << setw(wopt) << ' ' << "--stu no/all        : ָ������/ȫ��(7λ����Ϊѧ��,allΪȫ��)" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�ָ���ļ�" << endl;
    cout << setw(wopt) << ' ' << "--file filename/all : �����ļ���/ȫ��(allΪȫ��,����Ϊ�����ļ���)" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�" << endl;
    cout << setw(wopt) << ' ' << "--chapter n         : ��--file�Ļ������ٽ����½ڵ�ɸѡ(��/-1��ȫ���½�,����--week����)" << endl;
    cout << setw(wopt) << ' ' << "--week n            : ��--file�Ļ������ٽ����ܴε�ɸѡ(��/-1��ȫ���ܴ�,����--chapter����)" << endl;
    cout << setw(wopt) << ' ' << "--display xxxxx     : ÿλ0/1�ֱ��ʾ������Ϣ/δ�ύ��Ϣ/������Ϣ/������Ϣ/���ش�����Ϣ" << endl;
    cout << setw(wopt) << ' ' << "--cfgfile filename  : ָ�������ļ���(ȱʡ:hw_check.conf)" << endl;
    cout << endl;

    const char* DEMO_CNO = "10108001";
    const char* DEMO_SRC_FNAME = "12-b1.cpp";
    const char* DEMO_STUNO = "2359999";

    cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : ���" << DEMO_CNO << "����ѧ����������ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : ���" << DEMO_CNO << "����ѧ���ĵ�4����ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : ���" << DEMO_CNO << "����ѧ���ĵ�6����ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����������ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����" << DEMO_SRC_FNAME << "��ҵ�Ļ�����Ϣ" << endl;
    cout << endl;

    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : ���" << DEMO_CNO << "����ѧ����������ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : ���" << DEMO_CNO << "����ѧ���ĵ�4����ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : ���" << DEMO_CNO << "����ѧ���ĵ�6����ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����������ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����" << DEMO_SRC_FNAME << "��ҵ��������Ϣ" << endl;
    cout << endl;

    cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : ���" << DEMO_CNO << "������ѧ����" << DEMO_SRC_FNAME << "��ҵ�Ĵ�����Ϣ" << endl;
    cout << endl;

    cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : ���" << DEMO_CNO << "����ѧ����������ҵ�Ļ�����Ϣ(ָ�������ļ�)" << endl;
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
            cout << "����[" << args[OPT_ARGS_ACTION].get_name() << "]����ָ��." << endl;
            return -1;
        }
        if (!args[OPT_ARGS_CNO].existed()) {
            cout << "����[" << args[OPT_ARGS_CNO].get_name() << "]����ָ��." << endl;
            return -1;
        }
        if (!args[OPT_ARGS_STU].existed()) {
            cout << "����[" << args[OPT_ARGS_STU].get_name() << "]����ָ��." << endl;
            return -1;
        }
        if (!args[OPT_ARGS_FILE].existed()) {
            cout << "����[" << args[OPT_ARGS_FILE].get_name() << "]����ָ��." << endl;
            return -1;
        }
        return -1;
    }

    if (args[OPT_ARGS_ACTION].get_string() == "secondline") {
        if (args[OPT_ARGS_FILE].get_string() == "all") {
            //HW_Check_SecondLine ֻ����Ե��ļ�
            cout << "HW_Check_SecondLine ֻ����Ե��ļ�" << endl;
            return -1;
        }
        if (args[OPT_ARGS_STU].get_string() != "all") {
            cout << "HW_Check_SecondLine ֻ�����ȫ��ѧ��" << endl;
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
            cout << "�κŲ���8/13λ" << endl;
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
        cout << "����[--display]�ĳ��Ȳ���ȷ��ֻ����[5]λ��0/1." << endl;
        return -1;
    }
    for (int i = 0; i < 5; ++i) {
        char c = this->Args.display[i];
        if (c != '0' && c != '1') {
            //����[--display]�ĳ��Ȳ���ȷ��ֻ����[5]λ��0 / 1.
            cout << "����[--display]�ĳ��Ȳ���ȷ��ֻ����[5]λ��0/1." << endl;
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
        cout << "�����ļ� [" << this->Args.cfgfile << "] �޷���." << endl;
        return -1;
    }
    if (1) {
        const char* gname = "[�ļ�Ŀ¼]";
        const char* iname = "src_rootdir";
        const char* def_iname = "../../24251-source/source/"; //flag
        if (cfg.item_get_string(gname, iname, this->config.src_rootdir, def_iname, false, false) < 0) {
            cout << "ȡ�����ļ�" << iname << "���ֵ����." << endl;
            return -1;
        }
    }
    if (1) {
        const char* gname = "[���ݿ�]";
        const char* iname[] = {
            "db_host","db_username","db_passwd","db_name",
        };
        const char* def_iname[] = {
            "10.80.42.244","hwcheck","hw_CheCk-For23241*oOP","homework"
        };
        if (cfg.item_get_string(gname, iname[0], this->config.dbserver, def_iname[0], false, false) < 0) {
            cout << "ȡ�����ļ�" << iname[0] << "���ֵ����." << endl;
            return -1;
        }
        if (cfg.item_get_string(gname, iname[1], this->config.dbuser, def_iname[1], false, false) < 0) {
            cout << "ȡ�����ļ�" << iname[1] << "���ֵ����." << endl;
            return -1;
        }
        if (cfg.item_get_string(gname, iname[2], this->config.dbpasswd, def_iname[2], false, false) < 0) {
            cout << "ȡ�����ļ�" << iname[2] << "���ֵ����." << endl;
            return -1;
        }
        if (cfg.item_get_string(gname, iname[3], this->config.dbname, def_iname[3], false, false) < 0) {
            cout << "ȡ�����ļ�" << iname[3] << "���ֵ����." << endl;
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
    // �ڶ�������ȡע�����ݣ�ȥ��/*��*/��
    size_t firstNonSpace = str.find_first_not_of(" \t");
    bool isCorrect = (firstNonSpace != string::npos && str.substr(firstNonSpace, 2) == "//");
    if (!isCorrect) {
        size_t start = str.find("/*");
        size_t end = str.find("*/");
        if (end == -1) {
            info.msg_info = "���в��Ƿ���Ҫ���/* */��ʽ";
            return MSG_MULTI_COMMENT_FORMAT_ERROR;
        }

        string after_comment = str.substr(end + 2);

        // ���ҵ�һ���ǿհ��ַ���λ��
        if (after_comment.find_first_not_of(" \t\n\r\f\v") != string::npos) {
            info.msg_info = "���в��Ƿ���Ҫ���/* */��ʽ";
            return MSG_MULTI_COMMENT_FORMAT_ERROR;
        }
    }



    string content = str;
    if (removeCommentFlag(content) < 0) {
        cerr << "�Ƴ�ע�ͱ�Ǵ���" << endl;
        exit(-1);
    }

    // ���������ÿո�ָ����ݣ���������
    vector<string> tokens;
    istringstream iss(content);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    // ����Ƿ�����������
    if (tokens.size() != 3) {
        return MSG_FIRST_LINE_NOT_THREE;
    }

    // ���Ĳ�����������Ƿ�ƥ�䣨������˳��
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

    // ���ɴ�����Ϣ
    if (!found_no || !found_name || !found_class) {
        info.msg_info = "����";
        if (!found_no) info.msg_info += "ѧ�Ų�ƥ��";
        if (!found_name) info.msg_info += "������ƥ��";
        if (!found_class) info.msg_info += "�༶��ƥ��";
        if (!info.msg_info.empty() && info.msg_info.back() == ' ') {
            info.msg_info.pop_back();
        }
        return MSG_FIRST_LINE_ERROR;
    }

    return MSG_CORRECT;
}

int HW_Check::checkInfoCheck2(vector<Second_Check>& list)
{
    cout << "����������" << endl;
    cout << resetiosflags(ios::right) << setiosflags(ios::left);

    for (auto& student : list) {
        if (!student.isValid) {
            continue;
        }

        // ��黥���ϵ
        for (size_t i = 0; i < student.check_list.size(); ++i) {
            if (int(i) > student.cutline)
                break;
            auto& partner = student.check_list[i];
            bool partner_found = false;

            // ���Ҳ���֤�Է�ѧ����Ϣ
            for (const auto& other : list) {

                if (other.stu_no == partner.stu_no) {
                    partner_found = true;

                    // �������һ����
                    if (partner.stu_name != other.stu_name) {
                        partner.msg = "�Է���������ȷ  ";
                    }

                    // ���Է��Ƿ�Ҳѡ������
                    bool mutually_selected = false;
                    for (size_t j = 0; j < other.check_list.size(); ++j) {
                        if (int(j) > other.cutline)
                            break;
                        if (other.check_list[j].stu_no == student.stu_no) {
                            mutually_selected = true;
                            if (other.check_list[j].stu_name != student.stu_name) {
                                partner.msg = "ûд��������  ";
                                if (!other.isValid) {
                                    partner.msg = "��������  ";
                                }
                            }
                            break;
                        }
                    }

                    if (!mutually_selected) {
                        if(partner.msg == "" || partner.msg == "ûд��������  ")
                            partner.msg = "��������  ";
                    }
                    break;
                }
            }
            if (!partner_found) {
                partner.msg += "�Է�ѧ�Ų�����  ";
            }
        }
    }




    // ��ӡ
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
            while ((pos = name.find("��")) != string::npos)
                name.replace(pos, strlen("��"), ".");
        }
        cout << setw(3) << i + 1 << ": " << student.stu_cno << '-' << student.stu_no << '-' << name << endl;
        if (student.isValid && student.second_safe && student.has_duplicate) {
            size_t half_size = (1 + student.check_list.size()) / 2; // ��ȡ�б��һ���С
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
            size_t half_size = student.check_list.size(); // ��ȡ�б��һ���С
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

    // Դ�����ļ����
    if (find(extention.begin(), extention.end(), extension) != extention.end())
    {
        if (isUft8File(infile)) {
            //second_check.isValid = false;
            imsg = MSG_ENCODE_ERROR;
            return -1;
        }
        return 0;
    }

    // PDF�ļ���� 
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
        cerr << "�Ƴ�ע�ͱ�Ǵ���" << endl;
        exit(-1);
    }

    istringstream stream(secondLine);
    string studentNo, studentName;
    int count = 0;

    //while (stream >> (count % 2 ? studentName : studentNo)) {
    //    count++;
    //    if (count % 2 == 1) {
    //        // ���ѧ�ų���
    //        if (studentNo.length() != STU_NO_LENGTH) {
    //            //imsg = MSG_SECOND_LINE_ERROR;
    //            //second_check.isValid = false;
    //            info.msg_info = "��" + to_string((count + 1) / 2) + "λͬѧ��ѧ��[" + studentNo + "]����" + to_string(STU_NO_LENGTH) + "λ���������ݺ���";

    //            second_check.cutline = ((count + 1) / 2) - 1;
    //            second_check.second_safe = true;
    //            return -1;
    //        }
    //        // ����Ƿ�д���Լ�
    //        if (studentNo == second_check.stu_no) {
    //            //imsg = MSG_SECOND_LINE_ERROR;
    //            //second_check.isValid = false;
    //            info.msg_info = "��[" + to_string((count + 1) / 2) + "]��д���Լ����������ݺ���";
    //            second_check.second_safe = true;
    //            return -1;
    //        }
    //    }
    //    else {
    //        second_check.check_list.push_back({ studentNo, studentName, "" });
    //    }
    //info.msg_info = "��[" + to_string((count + 1) / 2) + "]��ѧ����Ϣ���������������ݺ���";

    while (stream >> (count % 2 ? studentName : studentNo)) {

        count++;

        // ����Ƿ�ֻ����һ������
        if (count % 2 == 1 && stream.eof()) {
            info.msg_info = "��[" + to_string((count + 1) / 2 -1) + "]��ѧ���������Ϣ��ȫ(ֻ����һ��)���������ݺ���";
            second_check.cutline = ((count + 1) / 2) - 1;
            second_check.second_safe = true;
            return -1;
        }

        if (count % 2 == 1) {
            // �ȼ��ѧ�����Ƿ�����Ƿ��ַ����������ַ���
            if (!all_of(studentNo.begin(), studentNo.end(), [](unsigned char c) {
                return isdigit(c);
                })) {
                //info.msg_info = "��" + to_string((count + 1) / 2) + "λͬѧ��ѧ��[" + studentNo + "]���з������ַ����������ݺ���";
                info.msg_info = "��" + to_string((count + 1) / 2) + "λͬѧ��ѧ��[" + studentNo + "]����" + to_string(STU_NO_LENGTH) + "λ���������ݺ���";

                second_check.cutline = ((count + 1) / 2) - 1;
                second_check.second_safe = true;
                return -1;
            }
            // �ټ��ѧ�ų���
            if (studentNo.length() != STU_NO_LENGTH) {
                info.msg_info = "��" + to_string((count + 1) / 2) + "λͬѧ��ѧ��[" + studentNo + "]����" + to_string(STU_NO_LENGTH) + "λ���������ݺ���";
                second_check.cutline = ((count + 1) / 2) - 1;
                second_check.second_safe = true;
                return -1;
            }
            // ����Ƿ�д���Լ�
            if (studentNo == second_check.stu_no) {
                info.msg_info = "��[" + to_string((count + 1) / 2) + "]��д���Լ����������ݺ���";
                second_check.second_safe = true;
                return -1;
            }
        }
        
        else {
            // ���ѧ�������Ƿ�Ϊ��
            if (studentName.empty()) {
                info.msg_info = "��[" + to_string(count / 2 - 1) + "]��ѧ������Ϊ�գ��������ݺ���";
                second_check.cutline = (count / 2) - 1;
                second_check.second_safe = true;
                return -1;
            }
            second_check.check_list.push_back({ studentNo, studentName, "" });
        }
    }

    // ������һ�ζ�ȡ�Ƿ��������Ƿ������ż���
    if (count % 2 == 1) {
        info.msg_info = "��[" + to_string((count + 1) / 2) + "]��ѧ���������Ϣ��ȫ(ֻ����һ��)���������ݺ���";
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

    // �����µ� Second_Check ����
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
        // �������
        if (baseCheck(infile, cur_filename, imsg) < 0) {
            //second_check.isValid = false;
            //it->isValid = false;  // ���� isValid Ϊ false
            second_check_list[check_index].isValid = false;  // ʹ������ֱ�ӷ���
            msg = msg_set[imsg];
            msg_cnt_glb[imsg]++;
            msg_cnt_stu[imsg]++;
        }
        else if (this->Args.action == 1) {  // ���м��
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
        else if (this->Args.action == 2) {  // �ڶ��м��
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

    // ����display���ô�ӡ��Ϣ
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
        // ���������еĵ��
        while ((pos = name.find("��")) != string::npos) {
            name.replace(pos, strlen("��"), ".");
        }
        if (this->Args.cno != 5000244001602) {
            mar = 16;
            int width = 1;
        }
        // ����ѧ��/������ϵ��ܳ��Ȳ����ù̶����
        string student_info = stu.stu_no + "/" + name;
        cout << resetiosflags(ios::right) << setiosflags(ios::left)
            << setw(3) << index4s << ": "
            << setw(mar) << student_info  // ʹ�ù̶����16����
            << " : " << msg << endl;     // ����ð��ǰ��ļ��
        index4s++;
    }
    

} 

/*___________________________________________________________________________________________________*/


int HW_Check::exe()
{
    // �洢���ݵ�����
    vector<STU_List> stu_list;
    vector<HW_List> hw_list;
    vector<Second_Check> second_check_list;
    if (get_data(stu_list, hw_list, second_check_list) < 0)
        return -1;

    // ��ʽ�������س���
    const char* INDENT = "  ";
    const int FILENAME_WIDTH = 28, STU_INFO_WIDTH = 8;

    // ͳ������
    int msg_cnt_glb[prompt_len] = { 0 };

    if (this->Args.action == 2 || this->Args.file != "all") {
        // ������ҵ�б��ҵ�Ŀ���ļ�
        for (const auto& hw : hw_list) {
            printSingleFileInfo(stu_list, hw.hw_filename);
            break;  // �ҵ�Ŀ���ļ����˳�ѭ��

        }
    }

    // ����ÿ��ѧ��
    for (size_t stu_i = 0; stu_i < stu_list.size(); stu_i++) {
        const auto& stu = stu_list[stu_i];
        int msg_cnt_stu[prompt_len] = { 0 };
        First_Check_Info info;

        // ��ӡѧ��������Ϣ
        if (this->Args.action != 2 && this->Args.file == "all")
            printStudentInfo(stu_i + 1, stu, hw_list.size());


        // ����ÿ����ҵ
        for (const auto& hw : hw_list) {
            if (hw.hw_filename == "3-b1-1.c")
                int a = 0; // flag
            //if(a)
            //cout << second_check_list.size() << endl;
            processHomework(second_check_list, stu, hw, msg_cnt_glb, msg_cnt_stu, msg_set, info);
        }

        // ��ӡѧ����ϸͳ��
        if (this->Args.action != 2 && this->Args.display[3] != '0' && this->Args.file == "all") {
            printStudentStatistics(msg_cnt_stu, hw_list.size(), msg_set);
        }
    }

    // ��ӡȫ��ͳ��
    if (this->Args.action != 2 && this->Args.file == "all")
        printGlobalStatistics(stu_list.size(), hw_list.size(), msg_cnt_glb, msg_set);
    else
        printGlobalStatistics2(stu_list.size(), hw_list.size(), msg_cnt_glb, msg_set);

    // �ڶ��м��
    if (this->Args.action == 2) {
        checkInfoCheck2(second_check_list);
    }

    return 0;
}


