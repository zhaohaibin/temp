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

	writeFile("add.txt", spChangeFiles->m_addXfile);
	writeFile("update.txt", spChangeFiles->m_updateXfile);
	writeFile("delete.txt", spChangeFiles->m_deleteXfile);

	cout << "本地新增文件"<<spChangeFiles->m_addXfile.size() << "  本地更新文件"<<spChangeFiles->m_updateXfile.size();
	cout << "本地删除文件"<<spChangeFiles->m_deleteXfile.size() << endl;
}

void copyFile(const string& targetPath)
{
	snapshot.copyLocalFile(targetPath);
}

int main()
{
	int write_db;
	cin >>write_db;

	loadSnapShot();
	//copyFile("D:/target");
	if(write_db == 1)
	{
		localSnapshotWriteToDb();
		
	}

	for(int i = 0; i < 1000; ++i)
	{
		cout << "第"<<(i+1) <<"次"<<endl;
		filterLocalChange();
// 		int reFilter = 0;
// 		cin >> reFilter;
// 		if(reFilter == 0)
// 		{
// 			break;
// 		}
		loadSnapShot();

		Sleep(100);
		//if(reFilter == 2)
		//	localSnapshotWriteToDb();
	}
	

// 	cout << "检查目录树是否正确"<<endl;
// 	snapshot.checkDirTree();

//	cout << "测试查找功能"  <<endl;
//	snapshot.checkFind();
//	vector<xfile_shared_ptr> xfileVec;
	snapshot.clear();
	int n;
	cin >>n;
	return 0;
}