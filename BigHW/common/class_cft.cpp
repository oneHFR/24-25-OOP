/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
/* 添加自己需要的头文件，注意限制 */
#include "../include/class_cft.h"
using namespace std;

/* 给出各种自定义函数的实现（已给出的内容不全） */
string config_file_tools::truncate_after_marker(const string& source_text)
{
	if (source_text.length() == 0)
		return "";

	size_t comment_marker1 = source_text.find(";");
	size_t comment_marker2 = source_text.find("#");
	size_t comment_marker3 = source_text.find("//");

	size_t first_comment_pos = string::npos;

	first_comment_pos = min(first_comment_pos, comment_marker1);
	first_comment_pos = min(first_comment_pos, comment_marker2);
	first_comment_pos = min(first_comment_pos, comment_marker3);

	if (first_comment_pos != string::npos)
		return source_text.substr(0, first_comment_pos);

	return source_text;
}

string config_file_tools::trim(const string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}

int config_file_tools::read_config_file()
{
	ifstream in(cfgname);
	if (!in.is_open()) {
		is_read_success = false;
		return -2;
	}

	string buffer;
	ConfigGroup current_group;
	current_group.name = SIMPLE_GNAME;
	config_data.push_back(current_group);

	while (getline(in, buffer)) {
		config_lines.push_back(buffer);

		if (buffer.length() > MAX_LINE) {
			is_read_success = false;
			in.close();
			return -3;
		}

		buffer = truncate_after_marker(buffer);
		buffer = trim(buffer);
		if (buffer.empty()) {
			continue;
		}

		if (buffer.front() == '[' && buffer.back() == ']') {
			ConfigGroup new_group;
			new_group.name = buffer;
			config_data.push_back(new_group);
			continue;
		}

		char separator = (item_separate_character_type == BREAK_CTYPE::Equal) ? '=' : ' ';
		size_t sep_pos = buffer.find(separator);

		if (sep_pos != string::npos) {
			string item_name = trim(buffer.substr(0, sep_pos));
			string item_value = trim(buffer.substr(sep_pos + 1));

			if (!item_name.empty() && item_name[0] != ';' && item_name[0] != '#') {
				ConfigItem new_item;
				new_item.name = item_name;
				new_item.value = item_value;
				new_item.raw = buffer;
				config_data.back().items.push_back(new_item);
			}
		}
		else {
			if (buffer[0] != ';' && buffer[0] != '#') {
				ConfigItem new_item;
				new_item.name = "";
				new_item.value = buffer;
				new_item.raw = buffer;
				config_data.back().items.push_back(new_item);
			}
		}
	}

	in.close();
	is_read_success = true;
	return 0;
}

string config_file_tools::to_upper(const string& str) const {
	string upper_str;
	for (char ch : str) {
		upper_str += toupper(ch);
	}
	return upper_str;
}



/*————————————————————————————————————————————————————*/
/*————————————————————————————————————————————————————*/
/*————————————————————————————————————————————————————*/
/*————————————————————————————————————————————————————*/

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
	if (!is_read_success)
		return false;

	if (config_data.empty() ||
		(config_data.size() == 1 && config_data[0].items.empty())) {
		return false;
	}

	return true;
}

int config_file_tools::get_all_group(vector <string>& ret)
{
	for (const ConfigGroup& a : config_data) {
		if (a.name.empty() && a.items.empty()) {
			continue;
		}
		ret.push_back(a.name);
	}
	return (config_data[0].name.empty() && config_data[0].items.empty()) ? config_data.size() - 1 : config_data.size();
}

int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	if (group_name == nullptr)
		return -1;

	ret.clear();

	string name = group_name;
	if (name != SIMPLE_GNAME && name[0] != '[')
		name = "[" + name + "]";

	if (!is_case_sensitive)
		name = to_upper(name);

	for (const ConfigGroup& g : config_data) {
		string curr_group_name = g.name;
		if (curr_group_name != SIMPLE_GNAME && curr_group_name[0] != '[') {
			curr_group_name = "[" + curr_group_name + "]";
		}

		if (!is_case_sensitive)
			curr_group_name = to_upper(curr_group_name);

		if (name == curr_group_name) {
			for (const ConfigItem& item : g.items)
				ret.push_back(item.raw);
			return g.items.size();
		}
	}

	return 0;
}

int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	return this->get_all_item(group_name.c_str(), ret, is_case_sensitive);
}

int config_file_tools::item_get_raw(const char* const group_name, const char* const item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	if (group_name == nullptr || item_name == nullptr)
		return -1;

	string group_name_str = group_name;
	string item_name_str = item_name;

	if (!group_is_case_sensitive) {
		group_name_str = to_upper(group_name_str);
		item_name_str = to_upper(item_name_str);
	}

	for (const ConfigGroup& current_group : config_data) {
		string current_group_name = current_group.name;

		if (!group_is_case_sensitive) {
			current_group_name = to_upper(current_group_name);
		}

		if (group_name_str == current_group_name) {
			for (const ConfigItem& item : current_group.items) {
				string current_item_name = item.name;

				if (!item_is_case_sensitive) {
					current_item_name = to_upper(current_item_name);
					item_name_str = to_upper(item_name_str);
				}

				if (item_name_str == current_item_name) {
					ret = item.value;
					return 1;
				}
			}
			return 0;
		}
	}

	return -1;
}

int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_raw(group_name.c_str(), item_name.c_str(), ret, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_null(const char* const group_name, const char* const item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	if (group_name == nullptr || item_name == nullptr)
		return -1;

	string group_name_str = group_name;
	string item_name_str = item_name;

	if (!group_is_case_sensitive) {
		group_name_str = to_upper(group_name_str);
		item_name_str = to_upper(item_name_str);
	}

	for (const ConfigGroup& curr_group : config_data) {
		string group_name_in_vector = curr_group.name;

		if (!group_is_case_sensitive) {
			group_name_in_vector = to_upper(group_name_in_vector);
		}

		if (group_name_str == group_name_in_vector) {
			for (const ConfigItem& item : curr_group.items) {
				string item_name_in_elem = truncate_after_marker(item.name);

				if (item_name_in_elem.empty())
					continue;

				if (!item_is_case_sensitive)
					item_name_in_elem = to_upper(item_name_in_elem);

				if (item_name_str == item_name_in_elem) {
					return 1;
				}
			}
			return 0;
		}
	}

	return 0;
}

int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_char(const char* const group_name, const char* const item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_value;
	int ret = item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (ret <= 0 || raw_value.empty()) {
		if (def_value != DEFAULT_CHAR_VALUE) {
			value = def_value;
			return 1;
		}
		return 0;
	}

	raw_value = trim(raw_value);
	if (raw_value.empty()) {
		if (def_value != DEFAULT_CHAR_VALUE) {
			value = def_value;
			return 1;
		}
		return 0;
	}

	char target_char = raw_value[0];
	if (choice_set != nullptr && *choice_set != '\0') {
		bool found = false;
		for (const char* p = choice_set; *p != '\0'; p++) {
			if (*p == target_char) {
				found = true;
				break;
			}
		}

		if (!found) {
			if (def_value != DEFAULT_CHAR_VALUE) {
				value = def_value;
				return 1;
			}
			return 0;
		}
	}
	value = target_char;
	return 1;
}

int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_char(group_name.c_str(), item_name.c_str(), value, choice_set, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_int(const char* const group_name, const char* const item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_value;
	int ret = item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (ret <= 0 || raw_value.empty()) {
		if (def_value != DEFAULT_INT_VALUE) {
			value = def_value;
			return 1;
		}
		return 0;
	}

	istringstream iss(raw_value);
	int temp_value;

	if (iss >> temp_value) {
		if (temp_value >= min_value && temp_value <= max_value) {
			value = temp_value;
			return 1;
		}

		if (def_value != DEFAULT_INT_VALUE) {
			value = def_value;
			return 1;
		}
		return 0;
	}

	if (def_value != DEFAULT_INT_VALUE) {
		value = def_value;
		return 1;
	}
	return 0;
}

int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_int(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_double(const char* const group_name, const char* const item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	if (group_name == nullptr || item_name == nullptr)
		return -1;

	string group_name_str = group_name;
	string item_name_str = item_name;

	if (!group_is_case_sensitive) {
		group_name_str = to_upper(group_name_str);
		item_name_str = to_upper(item_name_str);
	}

	for (const ConfigGroup& current_group : config_data) {
		string current_group_name = current_group.name;

		if (!group_is_case_sensitive)
			current_group_name = to_upper(current_group_name);


		if (group_name_str == current_group_name) {
			for (const ConfigItem& item : current_group.items) {
				string current_item_name = item.name;

				if (!item_is_case_sensitive) {
					current_item_name = to_upper(current_item_name);
					item_name_str = to_upper(item_name_str);
				}

				if (item_name_str == current_item_name) {
					if (!item.value.empty()) {
						istringstream iss(item.value);
						double read_value;

						if (iss >> read_value) {
							if (read_value >= min_value && read_value <= max_value) {
								value = read_value;
								return 1;
							}

							if (def_value != DEFAULT_DOUBLE_VALUE) {
								value = def_value;
								return 1;
							}

							return 0;
						}

						if (def_value != DEFAULT_DOUBLE_VALUE) {
							value = def_value;
							return 1;
						}
						return 0;
					}
					break;
				}
			}
			break;
		}
	}

	if (def_value != DEFAULT_DOUBLE_VALUE) {
		value = def_value;
		return 1;
	}

	return 0;
}

int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_double(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_cstring(const char* const group_name, const char* const item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	if (str_maxlen <= 1) {
		value[0] = '\0';
		return 0;
	}

	int max_len = min(str_maxlen, MAX_STRLEN);

	string group_name_str = group_name;
	string item_name_str = item_name;

	if (!group_is_case_sensitive)
		group_name_str = to_upper(group_name_str);

	if (!item_is_case_sensitive)
		item_name_str = to_upper(item_name_str);

	for (const ConfigGroup& g : config_data) {
		string group_name_in_vector = g.name;

		if (!group_is_case_sensitive) {
			group_name_in_vector = to_upper(group_name_in_vector);
		}

		if (group_name_str == group_name_in_vector) {
			for (const ConfigItem& elem : g.items) {
				string item_name_in_elem = elem.name;

				if (!item_is_case_sensitive)
					item_name_in_elem = to_upper(item_name_in_elem);

				if (item_name_str == item_name_in_elem) {
					if (!elem.value.empty()) {
						string string_value = elem.value;

						size_t pos = string_value.find(' ');
						if (pos != string::npos) {
							string_value = string_value.substr(0, pos);
						}

						strncpy(value, string_value.c_str(), max_len - 1);
						value[max_len - 1] = '\0';
						return 1;
					}
					else {
						break;
					}
				}
			}
			break;
		}
	}

	if (def_value != DEFAULT_CSTRING_VALUE) {
		strncpy(value, def_value, max_len - 1);
		value[max_len - 1] = '\0';
		return 1;
	}

	return 0;
}


int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	/* 本函数已实现 */
	return item_get_cstring(group_name.c_str(), item_name.c_str(), value, str_maxlen, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为 string 型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   string &value                              ：读到的string方式的字符串（返回1时可信，返回0则不可信）
		   const string &def_value                    ：读不到情况下的默认值，该参数有默认值DEFAULT_STRING_VALUE，分两种情况
															当值是   DEFAULT_STRING_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_STRING_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：为简化，未对\"等做转义处理，均按普通字符
***************************************************************************/
int config_file_tools::item_get_string(const char* const group_name, const char* const item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_value;
	int result = item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (result == -1) {
		if (def_value != DEFAULT_STRING_VALUE) {
			value = def_value;
			return 1;
		}
		else {
			value.clear();
			return 0;
		}
	}

	if (raw_value.empty()) {
		value = def_value;
		return 0;
	}

	value = raw_value;
	return 0;
}

int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为 IPv4 地址的32bit整型（主机序）
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   unsigned int &value                        ：读到的IP地址，32位整型方式（返回1时可信，返回0则不可信）
		   const unsigned int &def_value              ：读不到情况下的默认值，该参数有默认值DEFAULT_IPADDR_VALUE，分两种情况
															当值是   DEFAULT_IPADDR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_IPADDR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_ipaddr(const char* const group_name, const char* const item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result != 0) {
		if (def_value == DEFAULT_IPADDR_VALUE) {
			value = def_value;
			return 0;
		}
		else {
			value = def_value;
			return 1;
		}
	}

	if (target.empty()) {
		value = def_value;
		return (def_value == DEFAULT_IPADDR_VALUE) ? 0 : 1;
	}

	unsigned int ip_value = 0;
	int parts[4] = { 0 };
	int n = sscanf(target.c_str(), "%d.%d.%d.%d", &parts[0], &parts[1], &parts[2], &parts[3]);

	if (n != 4 || parts[0] < 0 || parts[0] > 255 || parts[1] < 0 || parts[1] > 255 ||
		parts[2] < 0 || parts[2] > 255 || parts[3] < 0 || parts[3] > 255) {
		value = def_value; 
		return (def_value == DEFAULT_IPADDR_VALUE) ? 0 : 1; 
	}

	ip_value = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];

	value = ip_value;
	return 1;
}

int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}


