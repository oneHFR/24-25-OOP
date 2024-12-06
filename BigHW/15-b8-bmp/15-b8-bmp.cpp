/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
//不再允许加入任何头文件，特别是<Windows.h>，查到就是0分甚至是倒扣-20!!!!!
using namespace std;

#include "15-b8-bmp.h"
#define OFFSET 54
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：按需补充
***************************************************************************/
bitmap::bitmap(const char* const filename)
{
    //_filename = new char[strlen(filename) + 1];
    //strcpy(_filename, filename);

    ifstream bmp(filename, ios::in | ios::binary);
    if (!bmp) {
        bmp.close();
        return;
    }

    bmp.seekg(10, ios::beg);
    bmp.read((char*)(&bp), 4);
    bmp.seekg(18, ios::beg);
    bmp.read((char*)(&w), 4);
    bmp.seekg(22, ios::beg);
    bmp.read((char*)(&h), 4);
    bmp.seekg(28, ios::beg);
    bmp.read((char*)(&bit_pp), 2);

    bit_pr = w * bit_pp;
    if (bit_pr % 32)
        bit_pr += 32 - (bit_pr % 32);
    byte_pr = bit_pr / 8;

    pixel_data = new unsigned char[byte_pr * h];
    bmp.seekg(bp, ios::beg);
    bmp.read((char*)pixel_data, byte_pr * h);

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
    //if (_filename) {
    //    delete[] _filename;
    //    _filename = nullptr; // 这个好像没啥用
    //}
    if (pixel_data) {
        delete[] pixel_data;
        pixel_data = nullptr;
    }
}

//按需增加内容

unsigned int bitmap::pixel_value(int row, int col) const
{
    if (!pixel_data || row < 0 || row >= h || col < 0 || col >= w)
        return 0;

    char ind = 0;
    PALETTE _RGB = { 0 };

    unsigned char* row_ptr = pixel_data + byte_pr * (h - 1 - row);
    switch (bit_pp) {
        case 1:
            ind = (row_ptr[col / 8] >> (7 - col % 8)) & 1;
            memcpy(&_RGB, pixel_data + OFFSET + ind * 4, 4);
            break;

        case 4:
            ind = (row_ptr[col / 2] >> ((!(col % 2)) * 4)) & 15;
            memcpy(&_RGB, pixel_data + OFFSET + ind * 4, 4);
            break;

        case 8:
            ind = row_ptr[col];
            memcpy(&_RGB, pixel_data + OFFSET + ((unsigned char)ind) * 4, 4);
            break;

        case 24:
            memcpy(&_RGB, row_ptr + 3 * col, 3);
            break;

        default:
            return 0;
    }

    return (_RGB.R << 16) | (_RGB.G << 8) | _RGB.B;
}

int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    if (!pixel_data || !draw_point || w <= 0 || h <= 0)
        return -1;

    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            int x = col, y = row;
            unsigned int color = pixel_value(row, is_mirror ? (w - 1 - col) : col);

            switch (angle) {
                case 0:
                    x = col;
                    y = row;
                    break;
                case 90:
                    x = row;
                    y = w - 1 - col;
                    break;
                case 180:
                    x = w - 1 - col;
                    y = h - 1 - row;
                    break;
                case 270:
                    x = h - 1 - row;
                    y = col;
                    break;
                default:
                    return -1;
            }

            draw_point(top_left_x + x,
                top_left_y + y,
                (color >> 16) & 0xFF,  // R
                (color >> 8) & 0xFF,   // G
                color & 0xFF           // B
            );
        }
    }

    return 0;
}
