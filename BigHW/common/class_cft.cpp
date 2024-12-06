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
		// 提取名称和值，并去除两端空白字符
		string name = trim(line.substr(0, pos));
		string value = trim(line.substr(pos + 1));
		return make_pair(name, value); // 返回名称和值的对
	}
	return make_pair("", "");  // 如果没有找到 '='，返回空对
}

string config_file_tools::get_group_name(const string& line) {
	// 假设组名在 `[]` 中，去掉两端的括号并返回
	if (line.size() > 2 && line[0] == '[' && line.back() == ']') {
		return line.substr(1, line.size() - 2);  // 返回去掉括号的组名
	}
	return "";  // 如果格式不正确，返回空字符串
}

string config_file_tools::get_item_name(const string& line)
{
	size_t pos = line.find_first_of("=");
	if (pos != string::npos) {
		return trim(line.substr(0, pos));  // 返回键（项名）
	}
	return "";
}

string config_file_tools::get_item_value(const string& line)
{
	size_t pos = line.find_first_of("=");
	if (pos != string::npos) {
		return trim(line.substr(pos + 1));  // 返回值部分
	}
	return "";
}

void config_file_tools::add_group(const string& line) {
	string group_name = get_group_name(line);  // 从行中提取组名
	ConfigGroup group;
	group.name = group_name;  // 设置组名
	config_data.push_back(group);  // 将组添加到 config_data
}

void config_file_tools::add_item(const string& line) {
	// 解析配置项的名称和值
	pair<string, string> item = parse_item(line);

	// 将配置项添加到当前组的 items 中
	if (!config_data.empty()) {
		// 假设当前组是 config_data 中的最后一个组
		ConfigItem config_item;
		config_item.name = item.first;  // 配置项名
		config_item.value = item.second;  // 配置项值

		// 添加配置项到最后一个组的 items 中
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
			str[i] += ('a' - 'A');  // 大写转小写
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
	// 清空输出容器
	ret.clear();
	int num = 0;

	// 遍历 config_data 中的每个组
	for (const auto& group : config_data) {
		if (!group.name.empty()) {  // 确保组名不为空
			ret.push_back(group.name);  // 添加组名到返回的容器中
			num++;
		}
	}

	// 如果没有找到组，返回 -1，表示失败
	return num > 0 ? num : -1;
}

int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	// 将 const char* 转换为 string 类型后调用 string 重载版本
	if (group_name == nullptr) {
		return -1; // 无效参数
	}
	string group_name_str(group_name);
	return get_all_item(group_name_str, ret, is_case_sensitive);
}

int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	ret.clear(); // 清空结果容器
	bool group_found = false;
	for (const auto& group : config_data) {
		string current_group_name = is_case_sensitive ? group.name : to_upper(group.name);
		string target_group_name = is_case_sensitive ? group_name : to_upper(group_name);

		if (current_group_name == target_group_name) {
			group_found = true;
			for (const auto& item : group.items) {
				ret.push_back(item.name + "=" + item.value); // 拼接为 "name=value" 格式
			}
			break; // 组名唯一，找到后直接结束
		}
	}
	return group_found ? ret.size() : -1; // 返回找到的项目数或 -1 表示未找到
}

int config_file_tools::item_get_raw(const char* const group_name, const char* const item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// 参数有效性检查
	if (group_name == nullptr || item_name == nullptr) {
		return -1;
	}

	string group_name_str = group_name;
	string item_name_str = item_name;

	// 根据大小写敏感性处理输入字符串
	if (!group_is_case_sensitive) {
		group_name_str = to_upper(group_name_str);
		item_name_str = to_upper(item_name_str);
	}

	// 遍历所有配置组
	for (const ConfigGroup& current_group : config_data) {
		string current_group_name = current_group.name;

		// 处理组名大小写
		if (!group_is_case_sensitive) {
			current_group_name = to_upper(current_group_name);
		}

		// 比对组名
		if (group_name_str == current_group_name) {
			// 遍历组内所有配置项
			for (const ConfigItem& item : current_group.items) {
				string current_item_name = item.name;

				// 处理配置项名称大小写
				if (!item_is_case_sensitive) {
					current_item_name = to_upper(current_item_name);
					item_name_str = to_upper(item_name_str);
				}

				// 比对配置项名称
				if (item_name_str == current_item_name) {
					ret = item.value;
					return 1;  // 找到匹配项
				}
			}
			return 0;  // 找到组但未找到配置项
		}
	}

	return -1;  // 未找到组
}

int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_raw(group_name.c_str(), item_name.c_str(), ret, group_is_case_sensitive, item_is_case_sensitive);
}

int config_file_tools::item_get_null(const char* const group_name, const char* const item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// 检查输入参数是否合法
	if (!group_name || !item_name) {
		return 0; // 参数不合法，视为不存在
	}

	// 遍历所有组，找到目标组
	for (const auto& group : config_data) {
		// 比较组名
		if ((group_is_case_sensitive && group.name == group_name) ||
			(!group_is_case_sensitive && _stricmp(group.name.c_str(), group_name) == 0)) {
			// 在组中查找项目
			for (const auto& item : group.items) {
				// 比较项目名
				if ((item_is_case_sensitive && item.name == item_name) ||
					(!item_is_case_sensitive && _stricmp(item.name.c_str(), item_name) == 0)) {
					return 1; // 找到匹配项，返回1
				}
			}
		}
	}
	return 0; // 未找到匹配项或组，返回0
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
	int result = this->item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (result != 0) {
		// 如果配置项不存在或读取失败，返回默认值
		value = def_value;
		return result;
	}

	// 解析得到的原始值，并赋值给char类型的value
	trim(raw_value);  // 去除多余的空白字符
	if (raw_value.empty()) {
		value = def_value;
		return 0;
	}

	// 检查字符是否在指定的选择集合中
	if (choice_set && strchr(choice_set, raw_value[0]) == nullptr) {
		// 如果字符不在choice_set中，返回错误
		value = def_value;
		return -1;
	}

	value = raw_value[0];  // 将第一个字符赋给value
	return 0;
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
	int result = item_get_raw(group_name, item_name, raw_value, group_is_case_sensitive, item_is_case_sensitive);

	if (result == -1 || raw_value.empty()) {
		value = def_value;  // 配置项不存在，使用默认值
		return 0;
	}

	// 尝试将字符串转换为整数
	try {
		value = std::stoi(raw_value);
	}
	catch (const std::invalid_argument&) {
		value = def_value;  // 转换失败，使用默认值
		return 0;
	}
	catch (const std::out_of_range&) {
		value = def_value;  // 转换失败，使用默认值
		return 0;
	}

	return 0;  // 成功读取并转换值
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
	// 1. 获取指定组和项的原始内容
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result != 0) {
		value = def_value;  // 如果读取失败，返回默认值
		return result;      // 返回错误码
	}

	// 2. 解析配置项中的值（假设格式为 "项名 = 值"）
	size_t pos = target.find_first_of('=') + 1;  // 查找 '=' 分隔符
	if (pos == string::npos) {  // 如果没有找到 '='
		value = def_value;
		return -1;  // 无效格式
	}

	string value_str = target.substr(pos);  // 提取 '=' 后的值部分
	value_str = trim(value_str);  // 去除前后空白字符

	// 3. 将字符串转换为 double
	try {
		value = stod(value_str);  // 转换为 double
	}
	catch (const invalid_argument&) {
		value = def_value;  // 转换失败，返回默认值
		return -1;  // 返回错误
	}

	// 4. 检查值是否在有效范围内
	if (value < static_cast<double>(min_value) || value > static_cast<double>(max_value)) {
		value = def_value;  // 如果超出范围，返回默认值
		return 0;  // 返回成功，值已被设置为默认值
	}

	return 0;  // 成功读取并赋值
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
	// 1. 获取指定组和项的原始内容
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result == -1) {
		// 读取失败，返回默认值
		strncpy(value, def_value, str_maxlen - 1);
		value[str_maxlen - 1] = '\0';
		return 0;
	}

	// 2. 查找 '=' 分隔符，提取值部分
	int pos = target.find_first_of('=');
	if (pos == string::npos || pos + 1 == target.length()) {  // 如果没有找到 '=' 或 '=' 后没有值
		// 返回默认值
		strncpy(value, def_value, str_maxlen - 1);
		value[str_maxlen - 1] = '\0';
		return 0;
	}

	string value_str = target.substr(pos + 1);  // 提取 '=' 后的值部分
	trim(value_str);  // 去除前后空白字符

	// 3. 将提取的值复制到目标字符串，确保不会超过最大长度
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

	return 0;  // 成功读取并赋值
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
		// 如果没有找到配置项，则返回默认值
		if (def_value != DEFAULT_STRING_VALUE) {
			value = def_value;
			return 1;  // 返回默认值
		}
		else {
			value.clear();  // 如果没有默认值，清空返回值
			return 0;  // 配置项不存在，返回空值
		}
	}

	if (raw_value.empty()) {
		value = def_value;  // 如果值为空，使用默认值
		return 0;  // 返回默认值
	}

	// 正常读取到值，返回该值
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
	// 1. 获取指定组和项的原始内容
	string target;
	int result = item_get_raw(group_name, item_name, target, group_is_case_sensitive, item_is_case_sensitive);
	if (result != 0) {
		if (def_value == DEFAULT_IPADDR_VALUE) {
			value = def_value;  // 直接返回默认值
			return 0;  // 不可信的值
		}
		else {
			value = def_value;  // 使用默认值
			return 1;  // 值可信
		}
	}

	// 2. 解析获取的IP地址
	if (target.empty()) {
		value = def_value;
		return (def_value == DEFAULT_IPADDR_VALUE) ? 0 : 1;  // 返回默认值并标记可信或不可信
	}

	// 3. 处理并解析IP地址
	unsigned int ip_value = 0;
	int parts[4] = { 0 };
	int n = sscanf(target.c_str(), "%d.%d.%d.%d", &parts[0], &parts[1], &parts[2], &parts[3]);

	// 4. 校验IP地址格式
	if (n != 4 || parts[0] < 0 || parts[0] > 255 || parts[1] < 0 || parts[1] > 255 ||
		parts[2] < 0 || parts[2] > 255 || parts[3] < 0 || parts[3] > 255) {
		value = def_value;  // 格式错误，返回默认值
		return (def_value == DEFAULT_IPADDR_VALUE) ? 0 : 1;  // 返回默认值并标记可信或不可信
	}

	// 5. 将IP地址转换为32位整数（主机序）
	ip_value = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];

	value = ip_value;
	return 1;  // 值可信
}

int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}


