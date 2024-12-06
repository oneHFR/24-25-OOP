/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <fstream>
//不再允许加入任何头文件，特别是<Windows.h>，查到就是0分甚至是倒扣-20!!!!!
using namespace std;

#include "15-b8-bmp.h"

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：按需补充
***************************************************************************/
bitmap::bitmap(const string& filename)
{
    _filename = new char[filename.length() + 1];
    strcpy(_filename, filename.c_str());

    ifstream bmp(filename, ios::in | ios::binary);
    if (!bmp) {
        _width = _height = _beg_pos = 0;
        _bit_per_pixel = 0;
        return;
    }

    bmp.seekg(10, ios::beg);
    bmp.read((char*)(&_beg_pos), 4);
    bmp.seekg(18, ios::beg);
    bmp.read((char*)(&_width), 4);
    bmp.seekg(22, ios::beg);
    bmp.read((char*)(&_height), 4);
    bmp.seekg(28, ios::beg);
    bmp.read((char*)(&_bit_per_pixel), 2);

    // 计算每行的位数和字节数
    bit_per_row = _width * _bit_per_pixel;
    if (bit_per_row % 32) {
        bit_per_row += 32 - (bit_per_row % 32);
    }
    byte_per_row = bit_per_row / 8;

    bmp.close();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：按需补充
***************************************************************************/
bitmap::~bitmap()
{
    if (_filename) {
        delete[] _filename;
        _filename = nullptr;
    }
}

//按需增加内容

unsigned int bitmap::get_pixel_value(int row, int col) const
{
    ifstream bmp(_filename, ios::in | ios::binary);
    if (!bmp)
        return 0;

    bmp.seekg(_beg_pos, ios::beg);
    bmp.seekg(byte_per_row * (_height - 1 - row), ios::cur);

    char ind = 0;
    palette _RGB = { 0 };

    switch (_bit_per_pixel) {
        case 1:
            bmp.seekg(col / 8, ios::cur);
            bmp.read(&ind, 1);
            ind >>= (7 - col % 8);
            ind &= 1;
            bmp.seekg(PALLETE_POS, ios::beg);
            bmp.seekg(ind * 4, ios::cur);
            bmp.read((char*)&_RGB, 4);
            break;

        case 4:
            bmp.seekg(col / 2, ios::cur);
            bmp.read(&ind, 1);
            ind >>= ((!(col % 2)) * 4);
            ind &= 15;
            bmp.seekg(PALLETE_POS, ios::beg);
            bmp.seekg(ind * 4, ios::cur);
            bmp.read((char*)&_RGB, 4);
            break;

        case 8:
            bmp.seekg(col, ios::cur);
            bmp.read(&ind, 1);
            bmp.seekg(PALLETE_POS, ios::beg);
            bmp.seekg(((unsigned char)ind) * 4, ios::cur);
            bmp.read((char*)&_RGB, 4);
            break;

        case 24:
            bmp.seekg(3 * col, ios::cur);
            bmp.read((char*)&_RGB, 3);
            break;

        default:
            bmp.close();
            return 0;
    }

    bmp.close();
    return (_RGB.R << 16) | (_RGB.G << 8) | _RGB.B;
}

int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    if (!_filename || !draw_point || _width <= 0 || _height <= 0)
        return -1;

    // 处理不同的旋转角度
    for (int row = 0; row < _height; row++) {
        for (int col = 0; col < _width; col++) {
            int x = col, y = row;
            unsigned int color = get_pixel_value(row, is_mirror ? (_width - 1 - col) : col);

            // 根据旋转角度计算实际坐标
            switch (angle) {
                case 0:
                    x = col;
                    y = row;
                    break;
                case 90:
                    x = row;
                    y = _width - 1 - col;
                    break;
                case 180:
                    x = _width - 1 - col;
                    y = _height - 1 - row;
                    break;
                case 270:
                    x = _height - 1 - row;
                    y = col;
                    break;
                default:
                    return -1;
            }

            // 调用画点函数
            draw_point(top_left_x + x,
                top_left_y + y,
                (color >> 16) & 0xFF,  // Red
                (color >> 8) & 0xFF,   // Green
                color & 0xFF           // Blue
            );
        }
    }

    return 0;
}
