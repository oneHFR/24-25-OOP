/* 2152131 ����� �ƿ� */
/* 2351285 ��� 2353568 �ܺ� 2253767 ����� 2351114 �쿡�� 2252036 �ջ� */
/* ��л����˼ͬѧ�ľ���*/
#include<iostream>
#include<fstream>
#include <iomanip>
#include<cstdlib>
#include<cstring>
#include <sstream>
using namespace std;
 
#define LEN 256
//Usage: d2 --infile hex��ʽ�ļ� --outfile bin��ʽ�ļ�
//d2 --infile a.hex --outfile a.bin

int usage(const char* procname)
{
	cout << "Usage: " << procname << " --infile hex��ʽ�ļ� --outfile bin��ʽ�ļ�" << endl;
	cout << "       " << procname << " --infile a.hex --outfile a.bin" << endl;
	//cout << "       " << procname << " --infile a.docx --outfile a.hex" << endl;

	return 0;
}

void print_out(ifstream& in, ostringstream& output_stream)
{
	int curr_ch = 0, prev_ch = 0, last_ch = 0, last2_ch = 0, hex_value;
	while ((curr_ch = in.get()) != EOF) {
		if (last2_ch == 32 && curr_ch == 32) {
			if (prev_ch <= 'f' && prev_ch >= 'a') {
				if (last_ch <= 'f' && last_ch >= 'a') {
					hex_value = (last_ch - 'a' + 10) * 16 + (prev_ch - 'a' + 10);
					output_stream << char(hex_value);
				}
				else if (last_ch <= '9' && last_ch >= '0') {
					hex_value = (last_ch - '0') * 16 + (prev_ch - 'a' + 10);
					output_stream << char(hex_value);
				}
			}
			else if (prev_ch >= '0' && prev_ch <= '9') {
				if (last_ch <= 'f' && last_ch >= 'a') {
					hex_value = (last_ch - 'a' + 10) * 16 + (prev_ch - '0');
					output_stream << char(hex_value);
				}
				else if (last_ch <= '9' && last_ch >= '0') {
					hex_value = (last_ch - '0') * 16 + (prev_ch - '0');
					output_stream << char(hex_value);
				}
			}
		}
		last2_ch = last_ch;
		last_ch = prev_ch;
		prev_ch = curr_ch;
	}
}

int h2b(ifstream& in, ofstream& out, char** argv, bool wrap)
{
	int index_in = 2;
	int index_out = 4;
	if (wrap) {
		index_in = 4;
		index_out = 2;
	}

	in.open(argv[index_in], ios::in | ios::binary);
	if (!in.is_open()) {
		cout << "�����ļ�" << argv[index_in] << "��ʧ��!" << endl;
		return -1;
	}

	out.open(argv[index_out], ios::out | ios::binary);
	if (!out.is_open()) {
		cout << "����ļ�" << argv[index_out] << "��ʧ��" << endl;
		return -1;
	}
	ostringstream oss;
	print_out(in, oss);
	out << oss.str();
	return 0;
}

int main(int argc, char* argv[])
{
	char file_name[LEN];
	ifstream in(file_name, ios::in | ios::binary);
	ofstream out;
	if ((argc != 5)) {
		usage(argv[0]);
		return 0;
	}

	if (strcmp(argv[1], "--infile") == 0 && strcmp(argv[3], "--outfile") == 0)
		h2b(in, out, argv, 0); // h2b Ц�� ʲô����ǩ֤
	else if (strcmp(argv[3], "--infile") == 0 && strcmp(argv[1], "--outfile") == 0)
		h2b(in, out, argv, 1);
	else
		usage(argv[0]);


	out.close();
	in.close();

	return 0;
}