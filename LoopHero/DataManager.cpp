#include "DataManager.h"

void DataManager::LoadIniFile(string filePath)
{
	if (mDatas.find(filePath) != mDatas.end()) return;
	mDatas.insert(make_pair(filePath, map<string, map<string, string>>()));

	ifstream ifs;
	ifs.open(filePath.c_str(), ios_base::in);
	if (ifs.is_open())
	{
		string group, key, data;
		while (!ifs.eof())
		{
			char c = ifs.peek();
			switch (c)
			{
			case '[':
				getline(ifs, group);
				group = group.substr(1, group.length() - 2);
				if (mDatas[filePath].find(group) == mDatas[filePath].end()) mDatas[filePath].insert(make_pair(group, map<string, string>()));
				break;
			case '#':
			case '\n':
				getline(ifs, data);
				break;
			default:
				getline(ifs, key, '=');
				getline(ifs, data);
				if (!data.empty()) data = data.substr(1, data.length() - 2);
				if (mDatas[filePath][group].find(key) == mDatas[filePath][group].end()) mDatas[filePath][group].insert(make_pair(key, data));
				break;
			}
		}

		ifs.close();
	}
}
