/* 2152131 ����� �ƿ� */
#pragma once

//����������κ�ͷ�ļ����ر���<Windows.h>���鵽����0�������ǵ���-20 ������

//���в���������ϣ��˽������⼸��Ԥ������������ã����ܷ�������ҵ�������㣡����
//#pragma pack(show) //�Ծ�����Ϣ����ʽ��ʾ��ǰ�ֽڶ����ֵ
//#pragma pack(push) //����ǰ�ֽڶ���ֵѹ�������ջ��ջ��
//#pragma pack(push, 4) //����ǰ�ֽڶ���ֵѹ�������ջ��ջ����Ȼ���ٽ�4���õ�ǰֵ
//#pragma pack(pop)  //��������ջջ�����ֽڶ���ֵ����������Ϊ��ǰֵ
//#pragma pack() //���������ǻָ�Ĭ��ֵ

//������������Ҫ�Ľṹ�壨�ࣩ����������������

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
    //�����������Ҫ�����ݳ�Ա����Ա����������������
    int _beg_pos;//10-13�ֽ�
    int _width;//18-21�ֽ�
    int _height;//22-25�ֽ�
    unsigned short _bit_per_pixel;//28-29�ֽ�
    int bit_per_row;
    int byte_per_row;
    char* _filename;
    unsigned int get_pixel_value(int row, int col) const;

public:
    //���в��ֲ���������κ�����
    bitmap(const string& filename);
    ~bitmap();
    int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};

