/* 2152131 吴洪蕊 计科 */
#include<iostream>
#include<string.h>
#include<iomanip>

using namespace std;
#define param_max_num 3
#define space "       "

struct Param {
    const char* name;
    const char* description;
    const int sub_num; 
    const int min;
    const int max;
    const int default_;
    int value;
};

int usage(const char* const procname, const Param params[])
{
    //cout << "Usage: " << procname << "[-l 大小][-n 数量][-t] IP地址" << endl;
    cout << "Usage: " << procname << " ";
    for (int i = 0; i < param_max_num; i++){
        cout << "[" << params[i].name;
        if (params[i].description != NULL)
            cout << " " << params[i].description;
        cout << "] ";
    }
    cout << "IP地址" << endl;

    cout << space << "==================================" << endl;
    cout << space << " 参数 附加参数 范围        默认值" << endl;
    cout << space << "==================================" << endl;

    for (int i = 0; i < param_max_num; i++){
        cout << space << " " << params[i].name << "   " << params[i].sub_num
                << space << " [" << setiosflags(ios::left)
                << setw(2) << params[i].min << ".." << setw(5) << params[i].max << "] "
                << resetiosflags(ios::left) << params[i].default_ << endl;
    }

    cout << space << "==================================" << endl;
    return 0;
}
/*

Usage: b4 [-l 大小] [-n 数量] [-t] IP地址
       ==================================
        参数 附加参数 范围        默认值
       ==================================
        -l   1        [32..64000] 64
        -n   1        [1..1024]   4
        -t   0        [0..1]      0
       ==================================

       -n 20 -l 256 -t -n 10 192.168.1.10
       -n 70020 -l 256 -t -l 128 192.168.1.10
*/

bool check_IP(const char* ip) {
    int segments = 0;
    int segmentValue = 0;
    int length = strlen(ip);
    int i = 0;

    while (i < length) {
        segmentValue = 0;

        while (i < length && isdigit(ip[i])) {
            segmentValue = segmentValue * 10 + (ip[i] - '0');
            i++;
        }

        if (segmentValue < 0 || segmentValue > 255)
            return false;
        segments++;

        if (i < length) {
            if (ip[i] != '.')
                return false;
            i++;
        }
    }

    return (segments == 4);
}

int valid(int argc, char* argv[], Param params[])
{
    for (int i = 1; i < argc - 1; i++) {
        if (argv[i][0] != '-') {
            cout << "参数" << argv[i] << "不是以-开头的合法参数" << endl;
            return 0; 
        }

        int param_helper = 0;
        for (int j = 0; j < param_max_num; j++) {
            if (strcmp(argv[i], params[j].name) == 0) {
                param_helper = true;

                if (params[j].sub_num) {
                    if (i < argc - 1) {
                        int value = atoi(argv[++i]);
                        if (i == argc - 1) {
                            cout << "参数" << params[j].name << "没有后续参数" << endl;
                            return 0;
                        }

                        if (value < params[j].min || value > params[j].max) {
                            params[j].value = params[j].default_;
                        }
                        else
                            params[j].value = value;
                    }
                    else {
                        cout << "参数" << params[j].name << "没有后续参数" << endl;
                        params[j].value = params[j].default_;
                        return 0;
                    }
                }
                else
                    params[j].value = 1; //针对t
            }
        }
        if (!param_helper) {
            cout << "参数" << argv[i] << "不存在" << endl;
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[])
{
    Param params[param_max_num] = {
        { "-l", "大小", 1, 32, 64000, 64 },
        { "-n", "数量", 1, 1, 1024, 4},
        { "-t", NULL,  0, 0, 1, 0}
    };

    if (argc < 2){
        usage(argv[0], params);
        return 0;
    }

    if (!check_IP(argv[argc - 1])){
        cout << "IP地址错误" << endl;
        return -1;
    }

    if (valid(argc, argv, params)) {
        cout << "参数检查通过" << endl;
        for (int i = 0; i < param_max_num; i++)
            cout << params[i].name << " 参数：" << params[i].value << endl;
        cout << "IP地址：" << argv[argc - 1] << endl;
    }

    return 0;

}