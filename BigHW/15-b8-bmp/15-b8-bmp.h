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


struct PALETTE
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
};


class bitmap {
private:
    //�����������Ҫ�����ݳ�Ա����Ա����������������
private:
    int bp;
    int w;
    int h;
    unsigned short bit_pp;  // bit per pixel
    int bit_pr;  // per_row
    int byte_pr;  // byte
    unsigned char* pixel_data;     // ������������ ��Ȼio̫����
    char* _filename;
    unsigned int pixel_value(int row, int col) const;

public:
    //���в��ֲ���������κ�����
    bitmap(const char *const filename);
    ~bitmap();
    int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};

