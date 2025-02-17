/* 2152131 吴洪蕊 计科 */
#pragma once

//不允许加入任何头文件，特别是<Windows.h>，查到就是0分甚至是倒扣-20 ！！！

//自行查阅相关资料，了解下面这几个预编译命令的作用，看能否给你的作业带来方便！！！
//#pragma pack(show) //以警告信息的形式显示当前字节对齐的值
//#pragma pack(push) //将当前字节对齐值压入编译器栈的栈顶
//#pragma pack(push, 4) //将当前字节对齐值压入编译器栈的栈顶，然后再将4设置当前值
//#pragma pack(pop)  //将编译器栈栈顶的字节对齐值弹出并设置为当前值
//#pragma pack() //不带参数是恢复默认值

//允许定义其它需要的结构体（类）、常量、常变量等

#define PALLETE_POS 54


struct palette
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char reserved;
};


class bitmap {
private:
    //自行添加你需要的数据成员及成员函数，不限制数量
    int _beg_pos;//10-13字节
    int _width;//18-21字节
    int _height;//22-25字节
    unsigned short _bit_per_pixel;//28-29字节
    int bit_per_row;
    int byte_per_row;
    char* _filename;
    unsigned int get_pixel_value(int row, int col) const;

public:
    //公有部分不允许添加任何内容
    bitmap(const string& filename);
    ~bitmap();
    int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};

