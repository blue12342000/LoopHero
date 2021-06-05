#include "DataManager.h"

void DataManager::LoadIniFile(string filePath, string tag)
{
	if (mDatas.find(tag) != mDatas.end()) return;
	mDatas.insert(make_pair(tag, map<string, map<string, string>>()));

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
				if (mDatas[tag].find(group) == mDatas[tag].end()) mDatas[tag].insert(make_pair(group, map<string, string>()));
				break;
			case '#':
			case '\n':
				getline(ifs, data);
				break;
			default:
				getline(ifs, key, '=');
				getline(ifs, data);
				if (!data.empty()) data = data.substr(1, data.length() - 2);
				if (mDatas[tag][group].find(key) == mDatas[tag][group].end()) mDatas[tag][group].insert(make_pair(key, data));
				break;
			}
		}

		ifs.close();
	}
}

void DataManager::SaveIniFile(string filePath, string tag)
{
	ofstream ofs;
	ofs.open(filePath, ios_base::out);
	ofs.clear();

	const auto& iniData = mDatas[tag];

	for (const auto& section : iniData)
	{
		ofs << "[" << section.first << "]" << endl;
		for (const auto& pair : section.second)
		{
			ofs << pair.first << "=\"" << pair.second << "\"" << endl;
		}
		ofs << endl;
	}
	ofs.close();
}
