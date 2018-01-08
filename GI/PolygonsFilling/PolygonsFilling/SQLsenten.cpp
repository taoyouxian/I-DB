#include "common.h"
#include "storage.h"

// {"create","drop","insert","delete","update","select"};

SQLsenten::SQLsenten()
{
	//行数
	FILE *f = fopen("out.sql", "r");
	char strLine[1024];
	fgets(strLine, 1024, f);
	int len = atoi(strLine);
	len--;
	//关键字，参数列表
	for (int i = 0; i < len; ++i)
	{
		fgets(strLine, 1024, f);
		string s(strLine);
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		string::size_type position;
		position = s.find(":");
		if (position == string::npos){
			key.push_back(s);
			continue;
		}
		string k = s.substr(0, position);
		string v = s.substr(position + 1);

		key.push_back(k);

		if (k == "data_type")
		{
			vector<string> tmp = string2vec(v), data_type, data_length;

			for (size_t j = 0; j < tmp.size(); ++j)
			{
				string::size_type index;
				index = tmp[j].find(" ");
				if (index != string::npos){
					string type = tmp[j].substr(0, index);
					string len = tmp[j].substr(index + 1);
					data_type.push_back(type);
					data_length.push_back(len);
				}
				else{
					data_type.push_back(tmp[j]);
					data_length.push_back("10");
				}
			}

			value["data_type"] = data_type;
			value["data_length"] = data_length;
			key.push_back("data_length");

		}
		else{
			if (v != "")
			value[k] = string2vec(v);
			if (value[k].size() == 0)
				value[k].push_back("null");
		}
	}
}

vector<string> SQLsenten::string2vec(string line)//按逗号分割字符串
{
	string s;
	if (line[line.length() - 1] == '\n'){
		string ss(line.begin(), line.end() - 1);
		s = ss;
	}
	else{
		string ss(line.begin(), line.end());
		s = ss;
	}
	vector<string> vec;
	string::size_type pos;
	while (s != "")
	{
		// cout<<s<<' '<<pos+1<<' '<<s.length()<<endl;
		pos = s.find(",");
		if (pos == string::npos){
			vec.push_back(s);
			break;
		}
		string subs = s.substr(0, pos);
		if (subs != "")
			vec.push_back(subs);
		//cout << s.substr(pos+1)<<endl;
		s = s.substr(pos + 1, s.length() - pos - 1);
		//cout<<s<<' '<<pos+1<<' '<<s.length()<<endl;
	}
	return vec;
}

void SQLsenten::print()//打印结果
{
	cout << "this is operator " << key[0];
	for (size_t i = 1; i < key.size(); ++i){
		cout << key[i] << ":";
		vector<string> v = value[key[i]];
		for (size_t i = 0; i < v.size(); ++i)
		{
			cout << v[i] << ' ';
		}
		cout << endl;
	}
}


//SQLsenten readSQL(const char* filename)//传入sql文件的名字
//{
//	FILE *f = fopen(filename,"r");
//	SQLsenten sql_sentence(f);
//
//	return sql_sentence;
//}