/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <iomanip>
#include "hw_check.h"
#include <string>
#include <sstream>
#include "../include_mariadb_x86/mysql/mysql.h"

using namespace std;


int HW_Check::get_data(vector<STU_List>& stu_list, vector<HW_List>& hw_list, vector<Second_Check>& second_list)
{
    MYSQL* mysql;
    MYSQL_RES* result1 = NULL;
    MYSQL_RES* result2 = NULL;
    MYSQL_ROW row;

    if ((mysql = mysql_init(NULL)) == NULL) {
        cout << "mysql_init failed" << endl;
        return -1;
    }

    if (mysql_real_connect(mysql, this->config.dbserver.c_str(), this->config.dbuser.c_str(),
        this->config.dbpasswd.c_str(), this->config.dbname.c_str(), 0, NULL, 0) == NULL) {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    mysql_set_character_set(mysql, "gbk");

    // 查询学生信息
    string cmd1 = "select * from view_hwcheck_stulist where stu_cno = \"" + to_string(this->Args.cno) + "\"";
    if (this->Args.cno2) {
        cmd1 = "select * from view_hwcheck_stulist where stu_cno = " +
            to_string(this->Args.cno) + " or stu_cno = " +
            to_string(this->Args.cno2);
    }
    if (this->Args.stu != -1) {
        cmd1 += " AND stu_no = \"" + to_string(this->Args.stu) + "\"";
    }

    if (mysql_query(mysql, cmd1.c_str())) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    if ((result1 = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    // 获取学生信息表的字段索引
    MYSQL_FIELD* fields1 = mysql_fetch_fields(result1);
    int num_fields1 = mysql_num_fields(result1);

    int stu_no_idx = -1;
    int stu_name_idx = -1;
    int class_fname_idx = -1;
    int class_sname_idx = -1;
    int stu_cno_idx = -1;

    for (int i = 0; i < num_fields1; i++) {
        string field_name = fields1[i].name;
        if (field_name == "stu_no")
            stu_no_idx = i;
        else if (field_name == "stu_name")
            stu_name_idx = i;
        else if (field_name == "stu_class_fname")
            class_fname_idx = i;
        else if (field_name == "stu_class_sname")
            class_sname_idx = i;
        else if (field_name == "stu_cno")
            stu_cno_idx = i;
    }

    if (stu_no_idx == -1 || stu_name_idx == -1 || class_fname_idx == -1 ||
        class_sname_idx == -1 || stu_cno_idx == -1) {
        cout << "Missing required fields in student table" << endl;
        return -1;
    }

    while ((row = mysql_fetch_row(result1)) != NULL) {
        STU_List tmp_stu;
        tmp_stu.stu_no = row[stu_no_idx];
        tmp_stu.stu_name = row[stu_name_idx];
        tmp_stu.stu_class_fname = row[class_fname_idx];
        tmp_stu.stu_class_sname = row[class_sname_idx];
        tmp_stu.stu_cno = row[stu_cno_idx];
        stu_list.push_back(tmp_stu);

        if (2 == this->Args.action) {
            Second_Check tmp_second;
            tmp_second.stu_no = row[stu_no_idx];
            tmp_second.stu_name = row[stu_name_idx];
            tmp_second.stu_cno = row[stu_cno_idx];
            tmp_second.isValid = true;
            second_list.push_back(tmp_second);
        }
    }

    mysql_free_result(result1);

    // 查询作业信息
    string cmd2 = "select * from view_hwcheck_hwlist where hw_cno in ( \"" + to_string(this->Args.cno) + "\")";
    if (this->Args.cno2) {
        cmd2 = "select * from view_hwcheck_hwlist where (hw_cno = " +
            to_string(this->Args.cno) + " or hw_cno = " +
            to_string(this->Args.cno2) + ")";
    }

    if (this->Args.chapter != -1) {
        cmd2 += " AND hw_chapter = \"" + to_string(this->Args.chapter) + "\"";
    }
    if (this->Args.week != -1) {
        cmd2 += " AND hw_week = \"" + to_string(this->Args.week) + "\"";
    }
    if (this->Args.file != "all") {
        cmd2 += " AND hw_filename = \"" + this->Args.file + "\"";
    }
    //cout << cmd2.c_str();
    if (mysql_query(mysql, cmd2.c_str())) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    if ((result2 = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    // 获取作业信息表的字段索引
    MYSQL_FIELD* fields2 = mysql_fetch_fields(result2);
    int num_fields2 = mysql_num_fields(result2);

    int hw_cno_idx = -1;
    int hw_chapter_idx = -1;
    int hw_week_idx = -1;
    int hw_filename_idx = -1;

    for (int i = 0; i < num_fields2; i++) {
        string field_name = fields2[i].name;
        if (field_name == "hw_cno")
            hw_cno_idx = i;
        else if (field_name == "hw_chapter")
            hw_chapter_idx = i;
        else if (field_name == "hw_week")
            hw_week_idx = i;
        else if (field_name == "hw_filename")
            hw_filename_idx = i;
    }

    if (hw_cno_idx == -1 || hw_chapter_idx == -1 ||
        hw_week_idx == -1 || hw_filename_idx == -1) {
        cout << "Missing required fields in homework table" << endl;
        return -1;
    }

    while ((row = mysql_fetch_row(result2)) != NULL) {
        HW_List tmp;
        tmp.hw_cno = row[hw_cno_idx];
        tmp.hw_chapter = row[hw_chapter_idx];
        tmp.hw_week = row[hw_week_idx];
        tmp.hw_filename = row[hw_filename_idx];

        if (this->Args.action == 1) {
            size_t lastDotIndex = tmp.hw_filename.find_last_of(".");
            string extention = tmp.hw_filename.substr(lastDotIndex + 1);
            auto it = find(this->extention.begin(), this->extention.end(), extention);
            if (it == this->extention.end()) {
                continue;
            }
        }
        hw_list.push_back(tmp);
    }

    mysql_free_result(result2);
    mysql_close(mysql);

    // 如果是第二种操作模式，读取每个学生的文件的第二行来填充 check_list
    if (2 == this->Args.action && !hw_list.empty()) {
        const auto& hw = hw_list[0];

        for (auto& student : second_list) {
            string filename = this->config.src_rootdir + to_string(this->Args.cno) + "-" + student.stu_no + "/" + hw.hw_filename;
            ifstream infile(filename);
            if (!infile) {
                continue;
            }

            string line;
            getline(infile, line);
            if (getline(infile, line)) {
                if (line.find("/*") != string::npos) {
                    size_t pos = line.find("*/");
                    if (pos != string::npos) {
                        line = line.substr(2, pos - 2);
                    }
                }

                istringstream iss(line);
                string studentNo, studentName;

                while (iss >> studentNo >> studentName) {
                    Second_Check_Stu partner;
                    partner.stu_no = studentNo;
                    partner.stu_name = studentName;
                    partner.msg = "";
                    student.check_list.push_back(partner);
                }
            }
            infile.close();
        }
    }

    return 0;
}


