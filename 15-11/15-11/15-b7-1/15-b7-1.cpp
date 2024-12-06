/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#if (_MSC_VER)       //VS2022
#include <conio.h>
#endif
#include <cmath>
//根据需要可加入其它头文件
using namespace std;

//此处为示例，允许修改结构体名称，允许修改结构体中的成员内容，要求sizeof必须是64
struct demo {
    unsigned char d[64];
};

struct Parameter {
    const char* name;
    int begin;
    int end;
    long long min;
    long long max;
};

struct Parameter params[] = {
    { "玩家昵称", 1, 16, 0, 0 },                      // 玩家昵称，不受范围约束
    { "生命", 17, 18, 0, 10000 },                   // 生命范围 0-10000
    { "力量", 19, 20, 0, 10000 },                   // 力量范围 0-10000
    { "体质", 21, 22, 0, 8192 },                    // 体质范围 0-8192
    { "灵巧", 23, 24, 0, 1024 },                    // 灵巧范围 0-1024
    { "金钱", 25, 28, 0, 100000000 },               // 金钱范围 0-100000000
    { "名声", 29, 32, 0, 1000000 },                 // 名声范围 0-1000000
    { "魅力", 33, 36, 0, 1000000 },                 // 魅力范围 0-1000000
    { "游戏累计时间", 37, 44, 0, 10000000000000000LL }, // 游戏累计时间范围 0-10000000000000000
    { "移动速度", 45, 45, 0, 100 },                 // 移动速度范围 0-100
    { "攻击速度", 46, 46, 0, 100 },                 // 攻击速度范围 0-100
    { "攻击范围", 47, 47, 0, 100 },                 // 攻击范围范围 0-100

    { "攻击力", 49, 50, 0, 2000 },                  // 攻击力范围 0-2000
    { "防御力", 51, 52, 0, 2000 },                  // 防御力范围 0-2000
    { "敏捷度", 53, 53, 0, 100 },                   // 敏捷度范围 0-100
    { "智力", 54, 54, 0, 100 },                     // 智力范围 0-100
    { "经验", 55, 55, 0, 100 },                     // 经验范围 0-100
    { "等级", 56, 56, 0, 100 },                     // 等级范围 0-100
    { "魔法值", 57, 58, 0, 10000 },                   // 魔法范围 0-10000
    { "消耗魔法值", 59, 59, 0, 100 },                 // 消耗魔法范围 0-100
    { "魔法伤害力", 60, 60, 0, 100 },               // 魔法伤害力范围 0-100
    { "魔法命中率", 61, 61, 0, 100 },                   // 命中率范围 0-100
    { "魔法防御力", 62, 62, 0, 100 },               // 魔法防御力范围 0-100
    { "暴击率", 63, 63, 0, 100 },                   // 暴击率范围 0-100
    { "耐力", 64, 64, 0, 100 },                     // 耐力范围 0-100
};

/* 此处允许新增函数，数量不限
   1、所有新增的函数，均不允许定义新的 fstream / ifstream / ofstream 流对象，并进行打开/读/写/关闭等操作
   2、所有新增的函数，均不允许用C方式进行文件处理
   3、上述两个限制同样适用于main函数
*/
static void usage(const char* procname)
{
    printf("usage : %s --modify | --read\n", procname);
}

long long getParameterValue(const struct demo* data, int index)
{
    long long value = 0;
    for (int j = params[index].end - 1; j >= params[index].begin - 1; --j) {
        value = (value << 8) | data->d[j];
    }
    return value;
}

void setParameterValue(struct demo* data, int index, long long value)
{
    for (int j = params[index].end - 1; j >= params[index].begin - 1; --j) {
        data->d[j] = value & 0xFF;
        value >>= 8;
    }
}

void r_show(const struct demo* data, char* name) {
    char displayName[32];
    long long temp = 0;

    // 将 i 的类型改为 size_t
    for (size_t i = 0; i < sizeof(params) / sizeof(params[0]); ++i) {
        if (i == 0) {
            int j = 0;
            for (; j < 16 && data->d[j] != '\0'; ++j)
                name[j] = data->d[j];
            name[j] = '\0';
            cout << setw(18) << params[i].name << "：" << name << endl;
        }
        else {
            strncpy(displayName, params[i].name, sizeof(displayName) - 1);
            displayName[sizeof(displayName) - 1] = '\0';
            if (strstr(displayName, "值") == NULL)
                strncat(displayName, "值", sizeof(displayName) - strlen(displayName) - 1);
            if (strcmp(displayName, "游戏累计时间值") == 0) {
                strncpy(displayName, "游戏累计时间(us)值", sizeof(displayName) - 1);
                displayName[sizeof(displayName) - 1] = '\0';
            }
            temp = 0;
            if (params[i].begin > 0 && params[i].end >= params[i].begin)
                for (size_t j = params[i].begin, j0 = j; j <= (size_t)params[i].end && j - 1 < sizeof(data->d); ++j)
                    temp += (long long)pow(256, (j - j0)) * data->d[j - 1];
            cout << setw(18) << displayName << "：" << temp << endl;
        }
    }
}

void printGameData(const struct demo* data, char* name) {
    cout << "--------------------------------------" << endl;
    cout << "  游戏存档文件修改工具" << endl;
    cout << "--------------------------------------" << endl;

    for (size_t i = 0; i < sizeof(params) / sizeof(params[0]); ++i) {
        cout << "  " << char('a' + i) << "." << setw(12) << left << params[i].name;
        if (i == 0) {
            if (name[0] == '\0') {
                int j = 0;
                for (; j < 16 && data->d[j] != '\0'; ++j) {
                    name[j] = data->d[j];
                }
                name[j] = '\0';
            }
            printf("(%s)\n", name);
        }
        else
            cout << "(" << getParameterValue(data, i) << ")" << endl;
    }

    cout << "--------------------------------------" << endl;
    cout << "  0.放弃修改" << endl;
    cout << "  1.存盘退出" << endl;
    cout << "--------------------------------------" << endl;
}

int modifyGameData(struct demo* data, char* name) {
    while (1) {
        printGameData(data, name);

        cout << "请选择[a..y, 0..1] ";
        #if (_MSC_VER)       //VS2022
                char choice = _getch();
        #elif (__linux__)       //Linux
                char choice = getchar();
        #endif

        if (choice == '0') {
            cout << endl;
            return 0;
        }
        else if (choice == '1') {
            cout << endl;
            return 1;
        }
        else if (choice >= 'a' && choice <= 'y') {
            size_t index = choice - 'a'; // 使用 size_t 类型
            if (index < sizeof(params) / sizeof(params[0])) { // 使用 size_t 类型
                if (index == 0) {
                    char currentName[16] = { 0 };
                    strncpy(currentName, name, sizeof(currentName) - 1);
                    printf("\n\n玩家昵称，当前值=%s，请输入 : ", currentName);
                    scanf("%s", name);
                    while (getchar() != '\n');
                }
                else {
                    long long value;
                    cout << "\n\n" << params[index].name
                        << "，当前值=" << getParameterValue(data, index)
                        << "，范围[" << params[index].min << ".." << params[index].max
                        << "]，请输入 : ";
                    cin >> value;
                    if (value >= params[index].min && value <= params[index].max) {
                        setParameterValue(data, index, value);
                    }
                }
            }
        }
    }
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：整个函数，只允许出现一次open、一次read（因为包含错误处理，允许多次close）
***************************************************************************/
int read(struct demo* data)
{
    /* 本函数中只允许定义一个 ifstream流对象，不再允许定义任何形式的fstream/ifstream/ofstream流对象，也不允许使用C方式的文件处理 */
    ifstream gfile;

    /* 文件打开，具体要求为：
       1、要求以读方式打开，打开方式***自行指定
       2、除本次open外，本函数其它地方不允许再出现open  */
    gfile.open("game.dat", ios::in | ios::binary);

    /* 进行后续操作，包括错误处理、读文件、显示各游戏项的值、关闭文件等，允许调用函数
       其中：只允许用一次性读取64字节的方法将game.dat的内容读入***（缓冲区名称、结构体名称自行指定）
                 gfile.read(***, sizeof(demo));
    */
    if (!gfile) {
        cout << "无法打开文件game.dat" << endl;
        return -1;
    }

    gfile.read((char*)data->d, sizeof(data->d));
    gfile.close();
    return 1;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：整个函数，只允许出现一次open、一次read、一次write（因为包含错误处理，允许多次close）
***************************************************************************/
int modify(struct demo* data)
{
    /* 本函数中只允许定义一个 fstream流对象，不再允许定义任何形式的fstream/ifstream/ofstream流对象，也不允许使用C方式的文件处理 */
    fstream gfile;

    /* 文件打开，具体要求为：
       1、要求以读写方式打开，打开方式***自行指定
       2、除本次open外，本函数其它地方不允许再出现open  */
    gfile.open("game.dat", ios::binary | ios::out);

    /* 进行后续操作，包括错误处理、读文件、显示各游戏项的值、关闭文件等，允许调用函数
       其中：只允许用一次性读取64字节的方法将game.dat的内容读入***（缓冲区名称、结构体名称自行指定）
                 gfile.read(***, sizeof(demo));
             只允许用一次性写入64字节的方法将***的内容写入game.dat中（缓冲区名称、结构体名称自行指定）
                 gfile.write(***, sizeof(demo));
    */
    if (!gfile) {
        cout << "无法打开文件game.dat" << endl;
        return -1;
    }

    gfile.write((char*)data->d, sizeof(data->d));
    gfile.close();
    return 1;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数允许带参数，不允许进行文件读写
***************************************************************************/
int main(int argc, char** argv)
{
    struct demo gameData = { 0 };
    char name[16] = { 0 };
    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }
    if (strcmp(argv[1], "--read") == 0) {
        if (read(&gameData))
            r_show(&gameData, name);
        else
            return -1;
    }
    else if (strcmp(argv[1], "--modify") == 0) {
        if (read(&gameData)) {
            if (modifyGameData(&gameData, name)) {
                modify(&gameData);
            }
            else
                return -1;
        }
    }
    return 0;
}
