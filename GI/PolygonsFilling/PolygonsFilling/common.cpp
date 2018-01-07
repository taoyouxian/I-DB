#include "common.h" 
#include "storage.h"  


/* ---------------utils-------------*/
namespace stringUtils
{
	void trim(string &s)
	{

		if (!s.empty())
		{
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
		}
	}
	//注意：当字符串为空时，也会返回一个空字符串
	void split(std::string& s, std::string delim, std::vector< std::string >& ret)
	{
		size_t last = 0;
		size_t index = s.find_first_of(delim, last);
		while (index != std::string::npos)
		{
			ret.push_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
		if (index - last>0)
		{
			ret.push_back(s.substr(last, index - last));
		}
	}

	void toUpperStr(string & str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	}
	void toLowerStr(string & str)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	}
	int String2Int(string str)
	{
		stringstream ss;
		ss << str;
		int i;
		ss >> i;
		return i;
	}
}

/*假定sql_str中的字符串空格只有一个,可以自己写工具函数处理成这种*/
void createSql(string sql_str, string& table_name, map<string, pair<int, int> >& paras_map)
{
	typedef int val_type;
	std::map<string, val_type> type_map;
	type_map["INT"] = INT_TYPE;
	type_map["VARCHAR"] = VARCHAR_TYPE;


	stringUtils::trim(sql_str);
	int first_space_index = sql_str.find(" ");
	string opt = sql_str.substr(0, first_space_index);
	stringUtils::toLowerStr(opt);

	// cout << opt;
	if ("create" == opt)
	{
		// create table;
		int left_bracket_index = sql_str.find("(");
		int second_space_index = sql_str.find(" ", first_space_index + 1);
		table_name = sql_str.substr(second_space_index + 1, left_bracket_index - second_space_index - 1);

		//        cout << "table_name: " + table_name << endl;

		string paras_str = sql_str.substr(left_bracket_index + 1, sql_str.size() - left_bracket_index - 2);
		//cout << paras_str  << endl;

		vector<string> paras_array; // elem like "rid INT"
		vector<string> tmp_one_para;    // elem like "rid" "INT"
		stringUtils::split(paras_str, ",", paras_array);
		// 实际处理这个map没必要有，直接处理就可以，不需要存储。
		//map<string, val_type> paras_map;

		for (int i = 0; i < paras_array.size(); i++)
		{
			stringUtils::trim(paras_array[i]);
			//cout << paras_array[i] << endl;
			tmp_one_para.clear();
			stringUtils::split(paras_array[i], " ", tmp_one_para);
			string tmp_para_name = tmp_one_para[0];
			stringUtils::toUpperStr(tmp_para_name);
			// cout << tmp_one_para[0] << " " << tmp_para_name << endl;

			// special tmp_one_para[0] should process case by case
			if (tmp_para_name == "PRIMARY")
			{
				int pk_index = sql_str.find(tmp_one_para[0]);
				int pk_left_bracket_index = sql_str.find("(", pk_index);
				int pk_right_bracket_index = sql_str.find(")", pk_index);
				string real_pk = sql_str.substr(pk_left_bracket_index + 1, pk_right_bracket_index - pk_left_bracket_index - 1);
				//                cout << "PRIMARY KEY: " + real_pk << endl;
			}
			// other case
			else
			{
				// normal
				// paras_map就是参数数组了。
				string para_type = tmp_one_para[1];
				stringUtils::toUpperStr(para_type);
				if (para_type.find("(") != string::npos)
				{
					string var_sz = para_type.substr(para_type.find("(") + 1, para_type.find(")") - para_type.find("(") - 1);

					//cout << "type size: "  << stringUtils::String2Int(var_sz) << endl;
					paras_map[tmp_one_para[0]] = make_pair(type_map[para_type.substr(0, para_type.find("("))], stringUtils::String2Int(var_sz));
					//                    cout<<tmp_one_para[0]<< " "<<para_type.substr(0,para_type.find("("))<<"with size: "  << stringUtils::String2Int(var_sz)<<endl;
					// head->data_dict(dictID).initAttribute(tmp_one_para[0], type_map[para_type.substr(0,para_type.find("("))], stringUtils::String2Int(var_sz));
				}
				else
				{
					paras_map[tmp_one_para[0]] = make_pair(type_map[tmp_one_para[1]], -1);
					//                    cout<<tmp_one_para[0]<< " "<<tmp_one_para[1]<<endl;
					//head->data_dict(dictID).initAttribute(tmp_one_para[0], type_map[tmp_one_para[1]], default_val);
				}

			}

		}

	}
} 

void changeTime(char *target, char *type){
	// 更改文件名
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	/*printf("%d-%02d-%02d %02d:%02d:%02d\n",
	t->tm_year + 1900,
	t->tm_mon + 1,
	t->tm_mday,
	t->tm_hour,
	t->tm_min,
	t->tm_sec);*/
	strcpy(target, PATH);
	strcat(target, "logs\\");
	char tmp[10];
	sprintf(tmp, "%d", t->tm_year + 1900);
	strcat(target, tmp);
	sprintf(tmp, "-%d", t->tm_mon + 1);
	strcat(target, tmp);
	sprintf(tmp, "-%d", t->tm_mday);
	strcat(target, tmp);
	sprintf(tmp, " %d", t->tm_min);
	strcat(target, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(target, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(target, tmp);
	strcat(target, type);
}

