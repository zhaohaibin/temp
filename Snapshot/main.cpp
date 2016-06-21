#include <iostream>
#include <fstream>
#include <algorithm>
#include "Snapshot.h"
#include <Windows.h>

using namespace std;

Snapshot snapshot;

void loadSnapShot()
{
	snapshot.loadSnapShot();
}

void localSnapshotWriteToDb()
{
	snapshot.snapshotWriteToDb();
}

bool compare(xfile_shared_ptr spXfileL, xfile_shared_ptr spXfileR)
{
	return spXfileL->m_path < spXfileR->m_path;
}
void writeFile(const string& path, vector<xfile_shared_ptr>& dataVec)
{
	ofstream newfileStream;
	newfileStream.open(path, ios::trunc);
	std::sort(dataVec.begin(), dataVec.end(), compare);
	if(newfileStream.is_open())
	{
		for(int i = 0; i < dataVec.size(); ++i)
		{
			newfileStream << dataVec[i]->m_path<< endl;
		}
		newfileStream.close();
	}
}

void filterLocalChange()
{
	std::shared_ptr<change_xfiles> spChangeFiles(new change_xfiles());
	snapshot.filterLocalChange(spChangeFiles);

	cout << "lc: 新增文件 " << spChangeFiles->m_addXfile.size() << "新增文件夹" << spChangeFiles->m_addFolder.size();
	cout << " 更新文件" << spChangeFiles->m_updateXfile.size() << " 删除文件" << spChangeFiles->m_deleteXfile.size();
	cout << " 删除文件夹" << spChangeFiles->m_deleteFolder.size()<< endl;
}

void copyFile(const string& targetPath)
{
	snapshot.copyLocalFile(targetPath);
}

int main()
{
	//loadSnapShot();
	//copyFile("D:/target");

	for(int i = 0; i < 1000; ++i)
	{
		int n;
		cin >>n;
		if(n == 3)
			break;
		cout << "第"<<(i+1) <<"次"<<endl;
		loadSnapShot();
		filterLocalChange();
		if(n == 1)
		{
			localSnapshotWriteToDb();
		}
		Sleep(100);
	}

	snapshot.clear();
	int n;
	cin >>n;
	return 0;
}