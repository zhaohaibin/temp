#include "Snapshot.h"
#include "directory_tree.h"
#include "file_system_helper.h"
#include <iostream>

Snapshot::Snapshot(void)
	: m_rootPath("D:\\Sync")
{
	m_db.initializeDb("D:\\db\\snapshot.db");
}


Snapshot::~Snapshot(void)
{
}

void Snapshot::loadSnapShot()
{
	_loadLocalSnapshot();
	_loadLastSnapshot();
}

void Snapshot::_loadLocalSnapshot()
{
	cout << "开始构建本地快照" <<endl;
	m_localTree.clear();
	time_t start = time(0);
	string strError;
	vector<xfile_shared_ptr> localXfileVec;
	bool ret = enum_directory(m_rootPath, localXfileVec, strError);

	cout << "枚举完成用时："<<time(0) - start << endl;
	
	start = time(0);
	for(int i = 0; i < localXfileVec.size(); ++i)
	{
		string path = _formatePath(localXfileVec[i]->m_path);
		localXfileVec[i]->m_path = path;
		m_localTree.insert(localXfileVec[i]);
	}
 	time_t endTime = time(0);
 	cout << "目录树构建完成用时：" << endTime - start << endl; 
}

void Snapshot::_loadLastSnapshot()
{
	cout << "开始构建last 快照" <<endl;
	m_lastTree.clear();
	time_t start = time(0);
	vector<boost::any> dataVec;
	shared_ptr<db_table> spSnapshot = m_db.getTable(TABLE_SNAPSHOT);
	spSnapshot->read_all(dataVec);
	for(int i = 0; i < dataVec.size(); ++i)
	{
		m_lastTree.insert(boost::any_cast<xfile_shared_ptr>(dataVec[i]));
	}
	cout << "last快照构建完成用时："<<time(0) - start <<endl;
}

std::string Snapshot::_formatePath( const string& path )
{
	string str = path.substr(m_rootPath.length(),path.length()-m_rootPath.length());
	return str;
}

void Snapshot::snapshotWriteToDb( )
{
	shared_ptr<vector<xfile_shared_ptr>> spXfileVec(new vector<xfile_shared_ptr>());
	m_localTree.for_each(std::bind(&Snapshot::_convertToVec, this, std::placeholders::_1, spXfileVec));

	shared_ptr<db_table> spTable = m_db.getTable(TABLE_SNAPSHOT);	
	spTable->clear();
	vector<boost::any> dataVec;
	for( int i = 0; i < spXfileVec->size(); ++i)
	{
		dataVec.push_back(spXfileVec->at(i));
		if(dataVec.size() >= 10000)
		{
			if(spTable)
			{
				spTable->insert(dataVec);
				dataVec.clear();
			}
		}
	}
	if(dataVec.size() > 0)
		spTable->insert(dataVec);
}

void Snapshot::filterLocalChange( shared_ptr<change_xfiles> spChangeXfiles )
{
	m_localTree.for_each(std::bind(&Snapshot::_filterLocalAddAndUpdate,this,std::placeholders::_1, spChangeXfiles));
	m_lastTree.for_each(std::bind(&Snapshot::_filterLocalDelete,this,std::placeholders::_1, spChangeXfiles));
}

void Snapshot::_filterLocalAddAndUpdate(shared_ptr<directory_tree_node> spLocalNode, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree_node> spLastNode = m_lastTree.find(spLocalNode->file()->m_path);
	if(spLastNode)
	{
		xfile_shared_ptr spLocalXfile = spLocalNode->file();
		xfile_shared_ptr spLastXfile = spLastNode->file();
		if(spLocalXfile->is_directory() && spLastXfile->is_directory())
			return;

		if(spLocalXfile->is_directory() == spLastXfile->is_directory())
		{
			if(spLocalXfile->m_mtime != spLastXfile->m_mtime)
			{
				spChangeXfiles->m_updateXfile.push_back(spLocalXfile);
				return;
			}

			if(spLocalXfile->m_size != spLocalXfile->m_size)
			{
				spChangeXfiles->m_updateXfile.push_back(spLocalXfile);
				return;
			}
		}else
		{
			spChangeXfiles->m_addXfile.push_back(spLocalXfile);
		}
		
	}else
	{
		spChangeXfiles->m_addXfile.push_back(spLocalNode->file());
	}
}

void Snapshot::_filterLocalDelete(shared_ptr<directory_tree_node> spLastNode, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree_node> spLocalNode = m_localTree.find(spLastNode->file()->m_path);
	if(spLocalNode)
	{
		xfile_shared_ptr spLocalXfile = spLocalNode->file();
		xfile_shared_ptr spLastXfile = spLastNode->file();
		if(spLocalXfile->is_directory() != spLastXfile->is_directory())
			spChangeXfiles->m_deleteXfile.push_back(spLastXfile);
	}else
	{
		spChangeXfiles->m_deleteXfile.push_back(spLastNode->file());
	}		
}

void Snapshot::_filterRemoteAddAndUpdateXfile( shared_ptr<directory_tree_node> spNode_L, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree_node> spNode_R = m_lastTree.find(spNode_L->file()->m_path);
	if(spNode_R)
	{
		xfile_shared_ptr spXfile_L = spNode_L->file();
		xfile_shared_ptr spXfile_R = spNode_R->file();
		if(spXfile_L->is_directory() && spXfile_R->is_directory())
			return;

		if(!spXfile_L->is_directory() && !spXfile_R->is_directory())
		{
			if(spXfile_L->m_sha1 != spXfile_R->m_sha1)
				spChangeXfiles->m_updateXfile.push_back(spXfile_L);
		}else
		{
			spChangeXfiles->m_addXfile.push_back(spXfile_L);
		}
	}else
	{
		spChangeXfiles->m_addXfile.push_back(spNode_L->file());
	}
}

void Snapshot::_filterRemoteDeleteXfile( shared_ptr<directory_tree_node> spLastNode, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree_node> spRemoteNode =  m_remoteTree.find(spLastNode->file()->m_path);
	if(spRemoteNode)
	{
		xfile_shared_ptr spLastXfile = spLastNode->file();
		xfile_shared_ptr spRemoteXfile = spRemoteNode->file();
		if(spLastXfile->is_directory() != spRemoteXfile->is_directory())
		{
			spChangeXfiles->m_deleteXfile.push_back(spLastXfile);
		}
	}else
	{
		spChangeXfiles->m_deleteXfile.push_back(spLastNode->file());
	}
}

void Snapshot::filterRemoteChange( shared_ptr<change_xfiles>spChangeXfiles )
{
	m_remoteTree.for_each(std::bind(&Snapshot::_filterRemoteAddAndUpdateXfile, this, std::placeholders::_1, spChangeXfiles));
	m_localTree.for_each(std::bind(&Snapshot::_filterRemoteDeleteXfile, this, std::placeholders::_1, spChangeXfiles));
}

void Snapshot::_copyFile( const string& path, shared_ptr<directory_tree_node> spNode )
{
	try
	{
		xfile_shared_ptr spXfile = spNode->file();
		string targetPath = path;
		targetPath += spNode->parent()->file()->m_path;
		targetPath += "\\" + spXfile->m_name;
		if(spXfile->is_directory())
		{
			boost::filesystem::create_directory(path);
		}else
		{
			string fromPaht = m_rootPath + spXfile->m_path;
			boost::filesystem::copy_file(fromPaht, targetPath);
		}
	}catch(boost::filesystem::filesystem_error error)
	{

	}
}

void Snapshot::copyLocalFile( const string& path )
{
	m_localTree.for_each(std::bind(&Snapshot::_copyFile, this, path, std::placeholders::_1));
}

void Snapshot::_convertToVec( shared_ptr<directory_tree_node> spNode, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec )
{
	spXfileVec->push_back(spNode->file());
}

void Snapshot::clear()
{
	m_lastTree.clear();
	m_localTree.clear();
}
