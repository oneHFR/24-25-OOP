/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include<cmath>
#include <string>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS  //��ʹ��lib����Ч

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */
/***************************************************************************
	�������ƣ�
	��    �ܣ����һ���ַ����Ƿ��ǺϷ���IPv4��ַ
	���������
	�� �� ֵ��
	˵    ����
***************************************************************************/
static int check_ip(string ip) 
{
	int pos[4];
	int n = 0;
	for (int i = 0; i < int(ip.length()); i++) {
		if (ip[i] == '.') {
			if (i == 0 || i == ip.length() - 1)
				return -1;
			pos[n++] = i;
		}
	}
	if (n != 3)
		return -1;
	if (pos[0] + 1 == pos[1] || pos[1] + 1 == pos[2])
		return -1;
	string parts[] = {
		ip.substr(0, pos[0]),
		ip.substr(pos[0] + 1, pos[1] - pos[0] - 1),
		ip.substr(pos[1] + 1, pos[2] - pos[1] - 1),
		ip.substr(pos[2] + 1)
	};

	for (const auto& part : parts) {
		if (part.length() > 3 || part.empty()) {
			return -1;
		}
		for (char c : part) {
			if (c < '0' || c > '9') {
				return -1;
			}
		}
		int num = atoi(part.c_str());
		if (num < 0 || num > 255) {
			return -1;
		}
	}
	return 1;
}

/***************************************************************************
	�������ƣ�
	��    �ܣ���IPv4��ַ�ַ���ת��Ϊ32λ�޷�������
	���������
	�� �� ֵ��
	˵    ����
***************************************************************************/
static unsigned int ip2int(string ip, bool& check) 
{
	unsigned int addr = 0;
	int now = 0;
	if (check_ip(ip) == -1) {
		check = false;
		return 0;
	}
	check = true;
	for (int i = 0; i < (int)ip.length(); i++) {
		if (ip[i] == '.') {
			addr <<= 8;
			addr += now;
			now = 0;
		}
		else {
			now *= 10;
			now += ip[i] - '0';
		}
		if (now > 255) {
			check = false;
			return 0;
		}
	}
	addr <<= 8;
	addr += now;

	return addr;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���һ���޷�������ת��ΪIP��ַ���ַ�����ʾ
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static string int2ip(unsigned int n) 
{
	string s;
	unsigned int id[] = { n & 0xff000000, n & 0xff0000, n & 0xff00, n & 0xff };
	for (int i = 0; i < 4; i++) {
		id[i] >>= ((3 - i) * 8);
		int x = id[i];
		string t;

		if (x < 100) {
			if (x < 10)
				t += x % 10 + '0';
			else {
				t += (x / 10) % 10 + '0';
				t += x % 10 + '0';
			}
		}
		else {
			t += (x / 100) % 10 + '0';
			t += (x / 10) % 10 + '0';
			t += x % 10 + '0';
		}
		s += t;
		if (i != 3)
			s += ".";
	}
	return s;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ַ�����ת��Ϊ����
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static int a2int(const char* const input, bool& isValid) 
{
	int result = 0;
	int sign = 1;

	if (input[0] == '-')
		sign = -1;

	for (int i = 0; i < (int)strlen(input); i++) {
		if (input[i] >= '0' && input[i] <= '9')
			isValid = true;
		else {
			if (i == 0 && input[i] == '-')
				isValid = true;
			else {
				isValid = false;
				return 0;
			}
		}
	}
	if (sign == 1)
		result = atoi(input);
	else if (sign == -1)
		result = atoi(input + 1);

	return result * sign;
}

/***************************************************************************
  �������ƣ�����
  ��    �ܣ�Ԫ��֮����б�� '/' �ָ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static void show(const int show[], int endValue = INVALID_INT_VALUE_OF_SET) {
	for (int i = 0; show[i] != endValue; i++) {
		if (i != 0) {
			cout << '/';
		}
		cout << show[i];
	}
}
static void show(const string show[], const string& endValue = "") {
	for (int i = 0; show[i] != endValue; i++) {
		if (i != 0) {
			cout << '/';
		}
		cout << show[i];
	}
}

/***************************************************************************
  �������ƣ�����
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static bool is_value_in_set(int value, const int* set) {
	for (int i = 0; set[i] != INVALID_INT_VALUE_OF_SET; i++) {
		if (set[i] == value) return true;
	}
	return false;
}
static bool is_value_in_set(const string& value, const string* set) {
	for (int i = 0; !set[i].empty(); i++) {
		if (set[i] == value) return true;
	}
	return false;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	init();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	extargs_bool_default = def;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	extargs_int_default = def;
	extargs_int_value = def;

	extargs_int_min = _min;
	extargs_int_max = _max;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	int s_i = 0;
	while (1) {
		if (set[s_i] == INVALID_INT_VALUE_OF_SET)
			break;
		s_i++;
	}

	extargs_int_set = new(nothrow) int[s_i + 1];
	if (extargs_int_set == NULL) {
		cout << "int��set�ڴ�����ʧ��\n";
		return;
	}

	for (int i = 0; i < s_i; i++)
		extargs_int_set[i] = set[i];
	extargs_int_set[s_i] = INVALID_INT_VALUE_OF_SET;

	int dp = def_of_set_pos;
	if (dp >= s_i)
		dp = 0;

	extargs_int_default = extargs_int_set[dp];
	extargs_int_value = extargs_int_default;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	bool check = 0;
	if (type == ST_EXTARGS_TYPE::str) {
		extargs_string_default = def;
		extargs_string_value = extargs_string_default;
		extargs_delen = int(extargs_string_default.length());
	}
	else if (type == ST_EXTARGS_TYPE::ipaddr_with_default) {
		extargs_ipaddr_default = ip2int(def, check);
		extargs_ipaddr_value = extargs_ipaddr_default;
		extargs_delen = int(def.length());
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	int setLen = 0;
	while (1) {
		if (set[setLen] == "")
			break;
		setLen++;
	}

	extargs_string_set = new(nothrow) string[setLen + 1];
	if (extargs_string_set == NULL) {
		cout << "str����set�ڴ�����ʧ��\n";
		return;
	}
	for (int i = 0; i < setLen; i++)
		extargs_string_set[i] = set[i];
	extargs_string_set[setLen] = "";

	int dp = def_of_set_pos;
	if (dp >= setLen || dp < 0)
		dp = 0;
	extargs_string_default = extargs_string_set[dp];
	extargs_string_value = extargs_string_default;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����double_with_default��double_with_error
  ***************************************************************************/ // flag
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const double	def, const double _min, const double _max)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	extargs_double_default = def;
	extargs_double_value = def;  

	extargs_double_min = _min;
	extargs_double_max = _max;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����double_with_set_default��double_with_set_error
 ***************************************************************************/ // flag
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const double* const set)
{
	init();
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;

	int s_i = 0;
	while (1) {
		if (fabs(set[s_i] - INVALID_DOUBLE_VALUE_OF_SET) < DOUBLE_DELTA)
			break;
		s_i++;
	}
	extargs_double_set = new(nothrow) double[s_i + 1];
	if (extargs_double_set == NULL) {
		cout << "double����set�ڴ�����ʧ��\n";
		return;
	}
	for (int i = 0; i < s_i; i++)
		extargs_double_set[i] = set[i];
	extargs_double_set[s_i] = INVALID_DOUBLE_VALUE_OF_SET;
	int dp = def_of_set_pos;
	if (dp >= s_i)
		dp = 0; 

	extargs_double_default = extargs_double_set[dp];
	extargs_double_value = extargs_double_default;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
	if (extargs_type == ST_EXTARGS_TYPE::int_with_set_default || extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
		delete[]extargs_int_set;
	else if (extargs_type == ST_EXTARGS_TYPE::str_with_set_default || extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
		delete[]extargs_string_set;
}

/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */
void args_analyse_tools::init()
{
	args_name.clear();
	args_existed = 0;
	extargs_type = ST_EXTARGS_TYPE::null;
	extargs_num = 0;

	extargs_bool_default = false;
	extargs_int_default = 0;
	extargs_int_min = 0;
	extargs_int_max = 0;
	extargs_double_min = 0.0;
	extargs_double_max = 0.0;

	extargs_int_set = nullptr;
	extargs_double_default = 0.0;
	extargs_double_set = nullptr;
	extargs_string_default.clear();
	extargs_string_set = nullptr;
	extargs_ipaddr_default = 0;

	extargs_int_value = 0;
	extargs_double_value = 0.0;
	extargs_string_value.clear();
	extargs_ipaddr_value = 0;
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
			��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const double args_analyse_tools::get_double() const
{
	return this->extargs_double_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f000001 תΪ "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	return int2ip(this->extargs_ipaddr_value);
	//return ""; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
			0  ���޺�������
			1  ���к�������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/

int args_analyse_process(const int argc, const char* const* const argv, args_analyse_tools* const args, const int follow_up_args)
{
	int length = 0;
	while (args[length].get_name() != "")
		length++;
	int i;
	for (i = 1; i < argc; i++) {
		int j;
		for (j = 0; j < length; j++) {
			if (args[j].args_name == argv[i]) {
				if (args[j].args_existed) {
					cout << "����[" << args[j].args_name << "]�ظ�.\n";
					return -1;
				}
				args[j].args_existed = 1;
				if (args[j].extargs_num) {
					i++;
					int num;
					bool check = 1;
					int value = 0;
					unsigned int ipv = 0;
					string stringv = "";
					switch (args[j].extargs_type) {
						case ST_EXTARGS_TYPE::int_with_default:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:int, ��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max
									<< "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
								return -1;
							}
							value = a2int(argv[i], check);
							if (!check || value<args[j].extargs_int_min || value > args[j].extargs_int_max)
								value = args[j].extargs_int_default;
							args[j].extargs_int_value = value;
							break;
						case ST_EXTARGS_TYPE::int_with_error:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:int, ��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max
									<< "])" << endl;
								return -1;
							}
							value = a2int(argv[i], check);
							if (!check || value<args[j].extargs_int_min || value > args[j].extargs_int_max)
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:int, ��Χ[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])\n";
								return -1;
							}
							args[j].extargs_int_value = value;
							break;
						case ST_EXTARGS_TYPE::int_with_set_default:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:int, ��ȡֵ[";
								show(args[j].extargs_int_set);
								cout << "] ȱʡ:" << args[j].extargs_int_default << ")" << endl;
								return -1;
							}
							value = a2int(argv[i], check);
							if (!check) {
								args[j].extargs_int_value = args[j].extargs_int_default;
							}
							else {

								for (num = 0; args[j].extargs_int_set[num] != INVALID_INT_VALUE_OF_SET; num++)
									if (value == args[j].extargs_int_set[num])
										break;
								if (args[j].extargs_int_set[num] == INVALID_INT_VALUE_OF_SET)
									args[j].extargs_int_value = args[j].extargs_int_default;
								else
									args[j].extargs_int_value = value;
								break;
							}
						case ST_EXTARGS_TYPE::int_with_set_error:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:int, ��ȡֵ[";
								show(args[j].extargs_int_set);
								cout << "])" << endl;
								return -1;
							}
							value = a2int(argv[i], check);
							for (num = 0; args[j].extargs_int_set[num] != INVALID_INT_VALUE_OF_SET; num++)
								if (value == args[j].extargs_int_set[num])
									break;
							if (args[j].extargs_int_set[num] == INVALID_INT_VALUE_OF_SET)
								check = 0;
							if (!check)
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:int, ��ȡֵ[";
								show(args[j].extargs_int_set);
								cout << "])\n";
								return -1;
							}
							args[j].extargs_int_value = value;
							break;
						case ST_EXTARGS_TYPE::ipaddr_with_default:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:IP��ַ";
								cout << " ȱʡ:" << int2ip(args[j].extargs_ipaddr_default) << ")" << endl;
								return -1;
							}
							ipv = ip2int(argv[i], check);
							if (check == 0)
								ipv = args[j].extargs_ipaddr_default;
							args[j].extargs_ipaddr_value = ipv;
							break;
						case ST_EXTARGS_TYPE::ipaddr_with_error:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:IP��ַ)" << endl;
								return -1;
							}
							ipv = ip2int(argv[i], check);
							if (check == 0)
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:IP��ַ)\n";
								return -1;
							}
							args[j].extargs_ipaddr_value = ipv;
							break;
						case ST_EXTARGS_TYPE::str:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:string";
								if (args[j].extargs_string_default == "")
									cout << ")" << endl;
								else
									cout << " ȱʡ:" << args[j].extargs_string_default << ")" << endl;
								return -1;
							}
							stringv = argv[i];
							args[j].extargs_string_value = stringv;
							break;
						case ST_EXTARGS_TYPE::str_with_set_default:
							if (i >= argc || argv[i][0] == '-' && argv[i][1] == '-')
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ�������. " << "(����:string, ��ȡֵ[";
								show(args[j].extargs_string_set);
								cout << "] ȱʡ:" << args[j].extargs_string_default << ")" << endl;
								return -1;
							}
							stringv = argv[i];
							for (num = 0; args[j].extargs_string_set[num] != ""; num++)
								if (stringv == args[j].extargs_string_set[num])
									break;
							if (args[j].extargs_string_set[num] == "")
								stringv = args[j].extargs_string_default;
							args[j].extargs_string_value = stringv;
							break;
						case ST_EXTARGS_TYPE::str_with_set_error:
							if (!check)
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ���ֵ(" << argv[i] << ")�Ƿ�. (����:int, ��ȡֵ[";
								show(args[j].extargs_string_set);
								cout << "])" << endl;
								return -1;
							}
							stringv = argv[i];
							for (num = 0; args[j].extargs_string_set[num] != ""; num++)
								if (stringv == args[j].extargs_string_set[num])
									break;
							if (args[j].extargs_string_set[num] == "")
							{
								cout << "����[" << args[j].args_name << "]�ĸ��Ӳ���ֵ(" << stringv << ")�Ƿ�. (����:string, ��ȡֵ[";
								show(args[j].extargs_string_set);
								cout << "])" << endl;
								return -1;
							}
							args[j].extargs_string_value = stringv;
							break;
						default:
							break;
					}
				}
				break;
			}
		}
		if (j == length) {
			if (argv[i][0] == '-' && argv[i][1] == '-')
			{
				cout << "����[" << argv[i] << "]�Ƿ�." << endl;
				return -1;
			}
			else if (follow_up_args)
			{
				return i;
			}
			else
			{
				cout << "����[" << argv[i] << "]��ʽ�Ƿ�" << endl;
				return -1;
			}
		}
	}
	return i;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	const char* TypeName[] = {
		"null",
		"Bool",
		"IntWithDefault",
		"IntWithError",
		"IntSETWithDefault",
		"IntSETWithError",
		"DoubleWithDefault",
		"DoubleWithError",
		"DoubleSETWithDefault",
		"DoubleSETWithError",
		"String",
		"StringSETWithDefault",
		"StringSETWithError",
		"IPAddrWithDefault",
		"IPAddrWithError"
	};
	int namelen = 4, typelen = 4, defaultlen = 7, existlen = 6, valuelen = 5, rangelen = 0, buflen = 7;
	int argslen = 0;
	while (args[argslen].get_name() != "")
		argslen++;


	for (int i = 0; i < argslen; i++) {
		namelen = max(namelen, int(args[i].args_name.length()));
		typelen = max(typelen, int(strlen(TypeName[(int)args[i].extargs_type])));
		defaultlen = max(defaultlen, args[i].extargs_delen);
		if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_default ||
			args[i].extargs_type == ST_EXTARGS_TYPE::double_with_error ||
			args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_default ||
			args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
			defaultlen = max(defaultlen, 10);
		}
	}

	for (int i = 0; i < argslen; i++){
		switch (args[i].extargs_type){
			case ST_EXTARGS_TYPE::double_with_default:
			case ST_EXTARGS_TYPE::double_with_error:
			case ST_EXTARGS_TYPE::double_with_set_default:
			case ST_EXTARGS_TYPE::double_with_set_error:
				if (args[i].args_existed != 0) {
					char buf[64];
					sprintf(buf, "%.6f", args[i].extargs_double_value);
					valuelen = max(valuelen, int(strlen(buf)));
				}
				break;

			case ST_EXTARGS_TYPE::int_with_default:
			case ST_EXTARGS_TYPE::int_with_error:
			case ST_EXTARGS_TYPE::int_with_set_default:
			case ST_EXTARGS_TYPE::int_with_set_error:
				if (args[i].args_existed != 0) {
					int number = args[i].extargs_int_value;
					int digitCount = 0;
					if (number < 0)
						number = -number;
					if (number == 0)
						digitCount = 1;
					else {
						while (number != 0) {
							number /= 10;
							++digitCount;
						}
					}
					valuelen = max(valuelen, digitCount);
				}
				break;
			case ST_EXTARGS_TYPE::ipaddr_with_default:
			case ST_EXTARGS_TYPE::ipaddr_with_error:
				if (args[i].args_existed != 0)
					valuelen = max(valuelen, int(int2ip(args[i].extargs_ipaddr_value).length()));
				break;
			case ST_EXTARGS_TYPE::str:
			case ST_EXTARGS_TYPE::str_with_set_default:
			case ST_EXTARGS_TYPE::str_with_set_error:
				if (args[i].args_existed != 0)
					valuelen = max(valuelen, int(args[i].extargs_string_value.length()));
				break;
			default:
				break;
		}
	}

	for (int i = 0; i < argslen; i++) {
		int rangeLength = 0;
		switch (args[i].extargs_type) {
			case ST_EXTARGS_TYPE::double_with_default:
			case ST_EXTARGS_TYPE::double_with_error:
				rangeLength = to_string(args[i].extargs_double_min).length() +
					to_string(args[i].extargs_double_max).length() + 5; // "[min..max]"
				break;

			case ST_EXTARGS_TYPE::int_with_default:
			case ST_EXTARGS_TYPE::int_with_error:
				rangeLength = to_string(args[i].extargs_int_min).length() +
					to_string(args[i].extargs_int_max).length() + 5; // "[min..max]"
				break;

			case ST_EXTARGS_TYPE::double_with_set_default:
			case ST_EXTARGS_TYPE::double_with_set_error:
				for (int j = 0; fabs(args[i].extargs_double_set[j] - INVALID_DOUBLE_VALUE_OF_SET) >= DOUBLE_DELTA; j++) {
					rangeLength += to_string(args[i].extargs_double_set[j]).length() + 1; // Add 1 for '/'
				}
				rangeLength--; // Remove extra '/' at the end
				break;

			case ST_EXTARGS_TYPE::int_with_set_default:
			case ST_EXTARGS_TYPE::int_with_set_error:
				for (int j = 0; args[i].extargs_int_set[j] != INVALID_INT_VALUE_OF_SET; j++) {
					rangeLength += to_string(args[i].extargs_int_set[j]).length() + 1; // Add 1 for '/'
				}
				rangeLength--; // Remove extra '/' at the end
				break;

			case ST_EXTARGS_TYPE::str_with_set_default:
			case ST_EXTARGS_TYPE::str_with_set_error:
				for (int j = 0; !args[i].extargs_string_set[j].empty(); j++) {
					rangeLength += args[i].extargs_string_set[j].length() + 1; // Add 1 for '/'
				}
				rangeLength--; // Remove extra '/' at the end
				break;

			default:
				rangeLength = 1; // For types with no range/set
				break;
		}
		rangelen = max(rangelen, rangeLength);
	}

	//cout << "namelen: " << namelen << endl;
	//cout << "typelen: " << typelen << endl;
	//cout << "defaultlen: " << defaultlen << endl;
	//cout << "existlen: " << existlen << endl;
	//cout << "valuelen: " << valuelen << endl;
	//cout << "rangelen: " << rangelen << endl;
	//cout << endl; // �������
	if (rangelen == 12)
		buflen--;

	cout << setw(namelen + typelen + defaultlen + existlen + valuelen + rangelen + buflen)
		<< setfill('=') << "=" << setfill(' ') << endl;
	cout << " ";
	cout << setiosflags(ios::left) << setw(namelen + 1) << "name"
		<< setw(typelen + 1) << "type"
		<< setw(defaultlen + 1) << "default"
		<< setw(existlen + 1) << "exists"
		<< setw(valuelen + 1) << "value"
		<< "range/set" << endl;
	cout << setw(namelen + typelen + defaultlen + existlen + valuelen + rangelen + buflen)
		<< setfill('=') << "=" << setfill(' ') << endl;

	for (int i = 0; i < argslen; i++){
		cout << " ";
		cout << setw(namelen) << args[i].args_name << ' ';
		cout << setw(typelen) << TypeName[(int)args[i].extargs_type] << " ";
		cout << setw(defaultlen);
		switch (args[i].extargs_type){
			case ST_EXTARGS_TYPE::double_with_default:
				cout << fixed << setprecision(6) << args[i].extargs_double_default << ' '; // 1
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_double_value;
				cout << " [" << args[i].extargs_double_min << ".." << args[i].extargs_double_max << "]";
				break;

			case ST_EXTARGS_TYPE::double_with_error:
				cout << "/" << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_double_value;
				cout << " [" << args[i].extargs_double_min << ".." << args[i].extargs_double_max << "]";
				break;

			case ST_EXTARGS_TYPE::double_with_set_default:
				cout << fixed << setprecision(6) << args[i].extargs_double_default << ' '; // 1
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_double_value;
				cout << ' ';
				{
					int j = 0;
					while (fabs(args[i].extargs_double_set[j] - INVALID_DOUBLE_VALUE_OF_SET) >= DOUBLE_DELTA) {
						if (j > 0)
							cout << '/';
						//cout << args[i].extargs_double_set[j] << endl;
						cout << fixed << setprecision(6) << args[i].extargs_double_set[j];
						j++;
					}
				}
				break;

			case ST_EXTARGS_TYPE::double_with_set_error:
				cout << "/" << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_double_value;
				cout << ' ';
				{
					int j = 0;
					while (fabs(args[i].extargs_double_set[j] - INVALID_DOUBLE_VALUE_OF_SET) >= DOUBLE_DELTA) {
						if (j > 0)
							cout << '/';
						double tmp = args[i].extargs_double_set[j];
						cout << fixed << setprecision(6) << args[i].extargs_double_set[j];
						j++;
					}
				}
				break;

			case ST_EXTARGS_TYPE::boolean:
				if (args[i].extargs_bool_default == 1)
					cout << "true";
				else
					cout << "false";
				cout << ' ' << setw(existlen) << args[i].args_existed << ' ';

				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << "true";
				cout << ' ' << "/";
				break;
			case ST_EXTARGS_TYPE::int_with_default:
				cout << args[i].extargs_int_default << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_int_value;
				cout << ' ' << '[' << args[i].extargs_int_min << ".." << args[i].extargs_int_max << "]";
				break;
			case ST_EXTARGS_TYPE::int_with_error:
				cout << '/' << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_int_value;
				cout << ' ' << '[' << args[i].extargs_int_min << ".." << args[i].extargs_int_max << "]";
				break;
			case ST_EXTARGS_TYPE::int_with_set_default:
				cout << args[i].extargs_int_default << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_int_value;
				cout << ' ';
				show(args[i].extargs_int_set);
				break;
			case ST_EXTARGS_TYPE::int_with_set_error:
				cout << "/" << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_int_value;
				cout << ' ';
				show(args[i].extargs_int_set);
				break;
			case ST_EXTARGS_TYPE::ipaddr_with_default:
				cout << int2ip(args[i].extargs_ipaddr_default) << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].get_str_ipaddr();
				cout << ' ' << '/';
				break;
			case ST_EXTARGS_TYPE::ipaddr_with_error:
				cout << '/' << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].get_str_ipaddr();
				cout << ' ' << '/';
				break;
			case ST_EXTARGS_TYPE::str:
				if (args[i].extargs_string_default != "")
					cout << args[i].extargs_string_default << ' ';
				else
					cout << '/' << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_string_value;
				cout << ' ' << '/';
				break;
			case ST_EXTARGS_TYPE::str_with_set_default:
				cout << args[i].extargs_string_default << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_string_value;
				cout << ' ';
				show(args[i].extargs_string_set);
				break;
			case ST_EXTARGS_TYPE::str_with_set_error:
				cout << '/' << ' ';
				cout << setw(existlen) << args[i].args_existed << ' ';
				if (args[i].args_existed == 0)
					cout << setw(valuelen) << '/';
				else
					cout << setw(valuelen) << args[i].extargs_string_value;
				cout << ' ';
				show(args[i].extargs_string_set);
				break;
			default:
				break;
		}
		cout << endl;
	}
	cout << setw(namelen + typelen + defaultlen + existlen + valuelen + rangelen + buflen)
		<< setfill('=') << "=" << setfill(' ') << endl <<endl;
	return 0;
}

#endif // !ENABLE_LIB_COMMON_TOOLS
