/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
/* ����Լ���Ҫ��ͷ�ļ���ע������ */
#include "../include/class_cft.h"
using namespace std;

/* ���������Զ��庯����ʵ�֣��Ѹ��������ݲ�ȫ�� */
string config_file_tools::remove_carriage_return(const string& line)
{
	if (!line.empty() && line[line.length() - 1] == '\r')
		return line.substr(0, line.length() - 1);
	return line;
}

void config_file_tools::parse_line(const string& line)
{
	if (line.empty())
		return;

	if (is_group_line(line))
		add_group(line);
	else if (is_item_line(line))
		add_item(line);
}

string config_file_tools::trim(const string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}

pair<string, string> config_file_tools::parse_item(const string& line) {
	size_t pos = line.find('=');
	if (pos != string::npos) {
		// ��ȡ���ƺ�ֵ����ȥ�����˿հ��ַ�
		string name = trim(line.substr(0, pos));
		string value = trim(line.substr(pos + 1));
		return make_pair(name, value); // �������ƺ�ֵ�Ķ�
	}
	return make_pair("", "");  // ���û���ҵ� '='�����ؿն�
}

string config_file_tools::get_group_name(const string& line) {
	// ���������� `[]` �У�ȥ�����˵����Ų�����
	if (line.size() > 2 && line[0] == '[' && line.back() == ']') {
		return line.substr(1, line.size() - 2);  // ����ȥ�����ŵ�����
	}
	return "";  // �����ʽ����ȷ�����ؿ��ַ���
}

string config_file_tools::get_item_name(const string& line)
{
	size_t pos = line.find_first_of("=");
	if (pos != string::npos) {
		return trim(line.substr(0, pos));  // ���ؼ���������
	}
	return "";
}

string config_file_tools::get_item_value(const string& line)
{
	size_t pos = line.find_first_of("=");
	if (pos != string::npos) {
		return trim(line.substr(pos + 1));  // ����ֵ����
	}
	return "";
}

void config_file_tools::add_group(const string& line) {
	string group_name = get_group_name(line);  // ��������ȡ����
	ConfigGroup group;
	group.name = group_name;  // ��������
	config_data.push_back(group);  // ������ӵ� config_data
}

void config_file_tools::add_item(const string& line) {
	// ��������������ƺ�ֵ
	pair<string, string> item = parse_item(line);

	// ����������ӵ���ǰ��� items ��
	if (!config_data.empty()) {
		// ���赱ǰ���� config_data �е����һ����
		ConfigItem config_item;
		config_item.name = item.first;  // ��������
		config_item.value = item.second;  // ������ֵ

		// �����������һ����� items ��
		config_data.back().items.push_back(config_item);
	}
}

bool config_file_tools::is_group_line(const string& line)
{
	return line[0] == '[' && line[line.length() - 1] == ']';
	//return line.size() > 2 && line[0] == '[' && line.back() == ']';
}

bool config_file_tools::is_item_line(const string& line)
{
	if (item_separate_character_type == BREAK_CTYPE::Equal)
		return line.find('=') != string::npos;
	else
		return line.find(' ') != string::npos || line.find('\t') != string::npos;
}

void config_file_tools::read_config_file()
{
	ifstream fp(cfgname, ios::in | ios::binary);
	if (!fp.is_open()) {
		cfgname = "";
		return;
	}

	string line;
	while (getline(fp, line)) {
		line = remove_carriage_return(line);

		if (line.length() > MAX_LINE) {
			cfgname = "";
			fp.close();
			return;
		}
		parse_line(line);
	}

	fp.close();
}

string config_file_tools::to_upper(const string& str) const {
	string upper_str;
	for (char ch : str) {
		upper_str += toupper(ch);
	}
	return upper_str;
}

void config_file_tools::to_lower(string& str) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += ('a' - 'A');  // ��дתСд
		}
	}
}



config_file_tools::config_file_tools(const string& cfgname, const enum BREAK_CTYPE bctype)
{
	this->cfgname = cfgname;
	this->item_separate_character_type = bctype;

	read_config_file();
}

config_file_tools::config_file_tools(const char* const cfgname, const enum BREAK_CTYPE bctype)
{
	this->item_separate_character_type = bctype;
	if (cfgname == nullptr) {
		this->cfgname = "";
		this->is_read_success = false;
		return;
	}
	*this = config_file_tools(string(cfgname), bctype);
}

config_file_tools::~config_file_tools()
{
	cfgname.clear();
	config_data.clear();
}

bool config_file_tools::is_read_succeeded() const
{
	return is_read_success;
	//return !cfgname.empty() && !config_data.empty();
}

int config_file_tools::get_all_group(vector <string>& ret)
{
	// ����������
	ret.clear();
	int num = 0;

	// ���� config_data �е�ÿ����
	for (const auto& group : config_data) {
		if (!group.name.empty()) {  // ȷ��������Ϊ��
			ret.push_back(group.name);  // ������������ص�������
			num++;
		}
	}

	// ���û���ҵ��飬���� -1����ʾʧ��
	return num > 0 ? num : -1;
}

int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	// �� const char* ת��Ϊ string ���ͺ���� string ���ذ汾
	if (group_name == nullptr) {
		return -1; // ��Ч����
	}
	string group_name_str(group_name);
	return get_all_item(group_name_str, ret, is_case_sensitive);
}

int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	ret.clear(); // ��ս������
	bool group_found = false;
	for (const auto& group : config_data) {
		string current_group_name = is_case_sensitive ? group.name : to_upper(group.name);
		string target_group_name = is_case_sensitive ? group_name : to_upper(group_name);

		if (current_group_name == target_group_name) {
			group_found = true;
			for (const auto& item : group.items) {
				ret.push_back(item.name + "=" + item.value); // ƴ��Ϊ "name=value" ��ʽ
			}
			break; // ����Ψһ���ҵ���ֱ�ӽ���
		}
	}
	return group_found ? ret.size() : -1; // �����ҵ�����Ŀ���� -1 ��ʾδ�ҵ�
}

int config_file_tools::item_get_raw(const char* const group_name, const char* const item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// ������Ч�Լ��
	if (group_name == nullptr || item_name == nullptr) {
		return -1;
	}

	string group_name_str = group_name;
	string item_name_str = item_name;

	// ���ݴ�Сд�����Դ��������ַ���
	if (!group_is_case_sensitive) {
		group_name_str = to_upper(group_name_str);
		item_name_str = to_upper(item_name_str);
	}

	// ��������������
	for (const ConfigGroup& current_group : config_data) {
		string current_group_name = current_group.name;

		// ����������Сд
		if (!group_is_case_sensitive) {
			current_group_name = to_upper(current_group_name);
		}

		// �ȶ�����
		if (group_name_str == current_group_name) {
			// ������������������
			for (const ConfigItem& item : current_group.items) {
				string current_item_name = item.name;

				// �������������ƴ�Сд
				if (!item_is_case_sensitive) {
					current_item_name = to_upper(current_item_name);
					item_name_str = to_upper(item_name_str);
				}

				// �ȶ�����������
				if (item_name_str == current_item_name) {
					ret = item.value;
					return 1;  // �ҵ�ƥ����
				}
			}
			return 0;  // �ҵ��鵫δ�ҵ�������
		}
	}

	return -1;  // δ�ҵ���
}

int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_raw(group_name.c_str(), item_name.c_str(), ret, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_null(const char* const group_name, const char* const item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// �����������Ƿ�Ϸ�
	if (!group_name || !item_name) {
		return 0; // �������Ϸ�����Ϊ������
	}

	// ���������飬�ҵ�Ŀ����
	for (const auto& group : config_data) {
		// �Ƚ�����
		if ((group_is_case_sensitive && group.name == group_name) ||
			(!group_is_case_sensitive && _stricmp(group.name.c_str(), group_name) == 0)) {
			// �����в�����Ŀ
			for (const auto& item : group.items) {
				// �Ƚ���Ŀ��
				if ((item_is_case_sensitive && item.name == item_name) ||
					(!item_is_case_sensitive && _stricmp(item.name.c_str(), item_name) == 0)) {
					return 1; // �ҵ�ƥ�������1
				}
			}
		}
	}
	return 0; // δ�ҵ�ƥ������飬����0
}

int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_char(const char* const group_name, const char* const item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_value;
	int result = this->item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (result != 0) {
		// �����������ڻ��ȡʧ�ܣ�����Ĭ��ֵ
		value = def_value;
		return result;
	}

	// �����õ���ԭʼֵ������ֵ��char���͵�value
	trim(raw_value);  // ȥ������Ŀհ��ַ�
	if (raw_value.empty()) {
		value = def_value;
		return 0;
	}

	// ����ַ��Ƿ���ָ����ѡ�񼯺���
	if (choice_set && strchr(choice_set, raw_value[0]) == nullptr) {
		// ����ַ�����choice_set�У����ش���
		value = def_value;
		return -1;
	}

	value = raw_value[0];  // ����һ���ַ�����value
	return 0;
}

int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_char(group_name.c_str(), item_name.c_str(), value, choice_set, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_int(const char* const group_name, const char* const item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_value;
	int result = item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (result == -1 || raw_value.empty()) {
		value = def_value;  // ��������ڣ�ʹ��Ĭ��ֵ
		return 0;
	}

	// ���Խ��ַ���ת��Ϊ����
	try {
		value = std::stoi(raw_value);
	}
	catch (const std::invalid_argument&) {
		value = def_value;  // ת��ʧ�ܣ�ʹ��Ĭ��ֵ
		return 0;
	}
	catch (const std::out_of_range&) {
		value = def_value;  // ת��ʧ�ܣ�ʹ��Ĭ��ֵ
		return 0;
	}

	return 0;  // �ɹ���ȡ��ת��ֵ
}

int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_int(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_double(const char* const group_name, const char* const item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// 1. ��ȡָ��������ԭʼ����
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result != 0) {
		value = def_value;  // �����ȡʧ�ܣ�����Ĭ��ֵ
		return result;      // ���ش�����
	}

	// 2. �����������е�ֵ�������ʽΪ "���� = ֵ"��
	size_t pos = target.find_first_of('=') + 1;  // ���� '=' �ָ���
	if (pos == string::npos) {  // ���û���ҵ� '='
		value = def_value;
		return -1;  // ��Ч��ʽ
	}

	string value_str = target.substr(pos);  // ��ȡ '=' ���ֵ����
	value_str = trim(value_str);  // ȥ��ǰ��հ��ַ�

	// 3. ���ַ���ת��Ϊ double
	try {
		value = stod(value_str);  // ת��Ϊ double
	}
	catch (const invalid_argument&) {
		value = def_value;  // ת��ʧ�ܣ�����Ĭ��ֵ
		return -1;  // ���ش���
	}

	// 4. ���ֵ�Ƿ�����Ч��Χ��
	if (value < static_cast<double>(min_value) || value > static_cast<double>(max_value)) {
		value = def_value;  // ���������Χ������Ĭ��ֵ
		return 0;  // ���سɹ���ֵ�ѱ�����ΪĬ��ֵ
	}

	return 0;  // �ɹ���ȡ����ֵ
}
int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_double(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_cstring(const char* const group_name, const char* const item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// 1. ��ȡָ��������ԭʼ����
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result == -1) {
		// ��ȡʧ�ܣ�����Ĭ��ֵ
		strncpy(value, def_value, str_maxlen - 1);
		value[str_maxlen - 1] = '\0';
		return 0;
	}

	// 2. ���� '=' �ָ�������ȡֵ����
	int pos = target.find_first_of('=');
	if (pos == string::npos || pos + 1 == target.length()) {  // ���û���ҵ� '=' �� '=' ��û��ֵ
		// ����Ĭ��ֵ
		strncpy(value, def_value, str_maxlen - 1);
		value[str_maxlen - 1] = '\0';
		return 0;
	}

	string value_str = target.substr(pos + 1);  // ��ȡ '=' ���ֵ����
	trim(value_str);  // ȥ��ǰ��հ��ַ�

	// 3. ����ȡ��ֵ���Ƶ�Ŀ���ַ�����ȷ�����ᳬ����󳤶�
	if (value_str.empty()) {
		strncpy(value, def_value, str_maxlen - 1);
		value[str_maxlen - 1] = '\0';
	}
	else {
		if (value_str.length() >= static_cast<size_t>(str_maxlen)) {
			strncpy(value, value_str.c_str(), str_maxlen - 1);
			value[str_maxlen - 1] = '\0';
		}
		else {
			strcpy(value, value_str.c_str());
		}
	}

	return 0;  // �ɹ���ȡ����ֵ
}

int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	/* ��������ʵ�� */
	return item_get_cstring(group_name.c_str(), item_name.c_str(), value, str_maxlen, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊ string ��
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   string &value                              ��������string��ʽ���ַ���������1ʱ���ţ�����0�򲻿��ţ�
		   const string &def_value                    ������������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_STRING_VALUE�����������
															��ֵ��   DEFAULT_STRING_VALUE ʱ������0��ֵ�����ţ�
															��ֵ���� DEFAULT_STRING_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����Ϊ�򻯣�δ��\"����ת�崦��������ͨ�ַ�
***************************************************************************/
int config_file_tools::item_get_string(const char* const group_name, const char* const item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_value;
	int result = item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (result == -1) {
		// ���û���ҵ�������򷵻�Ĭ��ֵ
		if (def_value != DEFAULT_STRING_VALUE) {
			value = def_value;
			return 1;  // ����Ĭ��ֵ
		}
		else {
			value.clear();  // ���û��Ĭ��ֵ����շ���ֵ
			return 0;  // ��������ڣ����ؿ�ֵ
		}
	}

	if (raw_value.empty()) {
		value = def_value;  // ���ֵΪ�գ�ʹ��Ĭ��ֵ
		return 0;  // ����Ĭ��ֵ
	}

	// ������ȡ��ֵ�����ظ�ֵ
	value = raw_value;
	return 0;
}

int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡĳ������ݣ���������Ϊ IPv4 ��ַ��32bit���ͣ�������
  ���������const char* const group_name               ������
		   const char* const item_name                ������
		   unsigned int &value                        ��������IP��ַ��32λ���ͷ�ʽ������1ʱ���ţ�����0�򲻿��ţ�
		   const unsigned int &def_value              ������������µ�Ĭ��ֵ���ò�����Ĭ��ֵDEFAULT_IPADDR_VALUE�����������
															��ֵ��   DEFAULT_IPADDR_VALUE ʱ������0��ֵ�����ţ�
															��ֵ���� DEFAULT_IPADDR_VALUE ʱ����value=def_value������1
		   const bool group_is_case_sensitive = false : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
		   const bool item_is_case_sensitive = false  : �����Ƿ��Сд���У�true-��Сд���� / Ĭ��false-��Сд������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int config_file_tools::item_get_ipaddr(const char* const group_name, const char* const item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// 1. ��ȡָ��������ԭʼ����
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result != 0) {
		if (def_value == DEFAULT_IPADDR_VALUE) {
			value = def_value;  // ֱ�ӷ���Ĭ��ֵ
			return 0;  // �����ŵ�ֵ
		}
		else {
			value = def_value;  // ʹ��Ĭ��ֵ
			return 1;  // ֵ����
		}
	}

	// 2. ������ȡ��IP��ַ
	if (target.empty()) {
		value = def_value;
		return (def_value == DEFAULT_IPADDR_VALUE) ? 0 : 1;  // ����Ĭ��ֵ����ǿ��Ż򲻿���
	}

	// 3. ��������IP��ַ
	unsigned int ip_value = 0;
	int parts[4] = { 0 };
	int n = sscanf(target.c_str(), "%d.%d.%d.%d", &parts[0], &parts[1], &parts[2], &parts[3]);

	// 4. У��IP��ַ��ʽ
	if (n != 4 || parts[0] < 0 || parts[0] > 255 || parts[1] < 0 || parts[1] > 255 ||
		parts[2] < 0 || parts[2] > 255 || parts[3] < 0 || parts[3] > 255) {
		value = def_value;  // ��ʽ���󣬷���Ĭ��ֵ
		return (def_value == DEFAULT_IPADDR_VALUE) ? 0 : 1;  // ����Ĭ��ֵ����ǿ��Ż򲻿���
	}

	// 5. ��IP��ַת��Ϊ32λ������������
	ip_value = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];

	value = ip_value;
	return 1;  // ֵ����
}

int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* ��������ʵ�� */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}


