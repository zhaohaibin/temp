#include <iostream>
#include <ctime>

#include "Snapshot.h"
#include "directory_tree.h"
#include "server_directory_tree.h"
#include "directory_helper.h"
#include "file_system_helper.h"


Snapshot::Snapshot(void)
	: m_rootPath("D:/Sync")
	, m_spLocalTree(new directory_tree())
	, m_spLastTree(new directory_tree())
	, m_spServerTree(new directory_tree())
{
	m_db.initializeDb("D:/db/snapshot.db");
	m_snapshotRecorder.set_root_path("D:/log/snapshot");
}


Snapshot::~Snapshot(void)
{
}

bool Snapshot::loadSnapShot(string& error)
{
	bool ret = true;
	m_snapshotRecorder.make_record_path();
	ret = _loadLocalSnapshot(error);
	if(ret == false)
		return ret;

	ret = _loadLastSnapshot(error);

	return ret;
}

bool Snapshot::_loadLocalSnapshot(string& error)
{
	m_spLocalTree->clear();

	vector<xfile_shared_ptr> localXfileVec;
	bool ret = enum_directory(m_rootPath, localXfileVec, error);
	if(ret == false)
		return ret;

	m_snapshotRecorder.record_local_snapshot(localXfileVec);
	for(int i = 0; i < localXfileVec.size(); ++i)
	{
		string path = _formatePath(localXfileVec[i]->m_path);
		localXfileVec[i]->m_path = path;
		if( m_spLocalTree->insert(localXfileVec[i]) == false )
		{
			ret = false;
			error = "构建目录树失败没有找到父节点：" + localXfileVec[i]->m_path;
			return ret;
		}
	}
	_set_space_type(m_spLocalTree);
	return ret;
}

bool Snapshot::_loadLastSnapshot(string& error)
{
	bool ret = true;
	m_spLastTree->clear();
	
	vector<boost::any> dataVec;
	shared_ptr<db_table> spSnapshot = m_db.getTable(TABLE_SNAPSHOT);
	if( spSnapshot->read_all(dataVec) != 0 )
	{
		error = "从数据库中读取数据时发生错误";
		ret = false;
	}

	m_snapshotRecorder.record_last_snapshot(dataVec);
	for(int i = 0; i < dataVec.size(); ++i)
	{
		ret = m_spLastTree->insert(boost::any_cast<xfile_shared_ptr>(dataVec[i]));
		if(ret == false)
		{
			error = "构建目录树失败没有找到父节点：" + boost::any_cast<xfile_shared_ptr>(dataVec[i])->m_path;
		}
	}
	return ret;
}

bool Snapshot::_loadServerSnapshot(string& error)
{
	bool ret = true;
	map<uint64_t, xfile_shared_ptr> xfiles;
	m_snapshotRecorder.record_server_snapshot(xfiles);
	shared_ptr<server_directory_tree> spServerTree(new server_directory_tree());
	spServerTree->init(xfiles);
	m_spServerTree = directory_helper::conv_to_directory_tree(spServerTree);
	return ret;
}

std::string Snapshot::_formatePath( const string& path )
{
	string str = path.substr(m_rootPath.length(),path.length()-m_rootPath.length());
	return str;
}

void Snapshot::snapshotWriteToDb( )
{
	shared_ptr<vector<xfile_shared_ptr>> spXfileVec(new vector<xfile_shared_ptr>());
	m_spLocalTree->for_each(std::bind(&Snapshot::_convertToVec, this, std::placeholders::_1, spXfileVec));

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
	m_spLocalTree->for_each(std::bind(&Snapshot::_filterLocalAddAndUpdate,this,std::placeholders::_1, spChangeXfiles));
	m_spLastTree->for_each(std::bind(&Snapshot::_filterLocalDelete,this,std::placeholders::_1, spChangeXfiles));
	m_snapshotRecorder.record_local_changes(spChangeXfiles);
}

void Snapshot::_filterLocalAddAndUpdate(shared_ptr<directory_tree::node> spLocalNode, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree::node> spLastNode = m_spLastTree->find(spLocalNode->file()->m_path);
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
				spChangeXfiles->push_to_update(spLocalXfile);
				//spChangeXfiles->m_updateXfile.push_back(spLocalXfile);
				return;
			}

			if(spLocalXfile->m_size != spLocalXfile->m_size)
			{
				spChangeXfiles->push_to_update(spLocalXfile);
				//spChangeXfiles->m_updateXfile.push_back(spLocalXfile);
				return;
			}
		}else
		{
			spChangeXfiles->push_to_add(spLocalXfile);
			//spChangeXfiles->m_addXfile.push_back(spLocalXfile);
		}
		
	}else
	{
		spChangeXfiles->push_to_add(spLocalNode->file());
		//spChangeXfiles->m_addXfile.push_back(spLocalNode->file());
	}
}

void Snapshot::_filterLocalDelete(shared_ptr<directory_tree::node> spLastNode, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree::node> spLocalNode = m_spLocalTree->find(spLastNode->file()->m_path);
	if(spLocalNode)
	{
		xfile_shared_ptr spLocalXfile = spLocalNode->file();
		xfile_shared_ptr spLastXfile = spLastNode->file();
		if(spLocalXfile->is_directory() != spLastXfile->is_directory())
			spChangeXfiles->push_to_delete(spLastXfile);
			//spChangeXfiles->m_deleteXfile.push_back(spLastXfile);
	}else
	{
		spChangeXfiles->push_to_delete(spLastNode->file());
		//spChangeXfiles->m_deleteXfile.push_back(spLastNode->file());
	}		
}

void Snapshot::_filterRemoteAddAndUpdateXfile( shared_ptr<directory_tree::node> spNode_L, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree::node> spNode_R = m_spLastTree->find(spNode_L->file()->m_path);
	if(spNode_R)
	{
		xfile_shared_ptr spXfile_L = spNode_L->file();
		xfile_shared_ptr spXfile_R = spNode_R->file();
		if(spXfile_L->is_directory() && spXfile_R->is_directory())
			return;

		if(!spXfile_L->is_directory() && !spXfile_R->is_directory())
		{
			if(spXfile_L->m_sha1 != spXfile_R->m_sha1)
				spChangeXfiles->push_to_update(spXfile_L);
				//spChangeXfiles->m_updateXfile.push_back(spXfile_L);
		}else
		{
			spChangeXfiles->push_to_add(spXfile_L);
			//spChangeXfiles->m_addXfile.push_back(spXfile_L);
		}
	}else
	{
		spChangeXfiles->push_to_add(spNode_L->file());
		//spChangeXfiles->m_addXfile.push_back(spNode_L->file());
	}
}

void Snapshot::_filterRemoteDeleteXfile( shared_ptr<directory_tree::node> spLastNode, shared_ptr<change_xfiles> spChangeXfiles )
{
	shared_ptr<directory_tree::node> spRemoteNode =  m_spServerTree->find(spLastNode->file()->m_path);
	if(spRemoteNode)
	{
		xfile_shared_ptr spLastXfile = spLastNode->file();
		xfile_shared_ptr spRemoteXfile = spRemoteNode->file();
		if(spLastXfile->is_directory() != spRemoteXfile->is_directory())
		{
			spChangeXfiles->push_to_delete(spLastXfile);
			//spChangeXfiles->m_deleteXfile.push_back(spLastXfile);
		}
	}else
	{
		spChangeXfiles->push_to_delete(spLastNode->file());
		//spChangeXfiles->m_deleteXfile.push_back(spLastNode->file());
	}
}

void Snapshot::filterRemoteChange( shared_ptr<change_xfiles>spChangeXfiles )
{
	m_spServerTree->for_each(std::bind(&Snapshot::_filterRemoteAddAndUpdateXfile, this, std::placeholders::_1, spChangeXfiles));
	m_spLastTree->for_each(std::bind(&Snapshot::_filterRemoteDeleteXfile, this, std::placeholders::_1, spChangeXfiles));
	m_snapshotRecorder.record_server_changes(spChangeXfiles);
}

void Snapshot::_copyFile( const string& path, shared_ptr<directory_tree::node> spNode )
{
	try
	{
		xfile_shared_ptr spXfile = spNode->file();
		string targetPath = path;
		targetPath += spNode->parent()->file()->m_path;
		targetPath += "/" + spXfile->m_name;
		if(spXfile->is_directory())
		{
			string error;
			make_dirs(targetPath, error);
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
	m_spLocalTree->for_each(std::bind(&Snapshot::_copyFile, this, path, std::placeholders::_1));
}

void Snapshot::_convertToVec( shared_ptr<directory_tree::node> spNode, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec )
{
	spXfileVec->push_back(spNode->file());
}

void Snapshot::clear()
{
	m_spLocalTree->clear();
	m_spLastTree->clear();
	m_spServerTree->clear();
}

void Snapshot::_set_space_type( shared_ptr<directory_tree> spTree )
{
	directory_tree::node_subitems_type subitems = spTree->root_subitems();
	directory_tree::node_subitems_type::iterator it = subitems.begin();
	for( ; it != subitems.end(); ++it)
	{
		if(it->second->file()->m_name == PUBLIC)
		{
			it->second->file()->m_space_type = SPACE_PUBLIC;
		}else if(it->second->file()->m_name == PRIVATE)
		{
			it->second->file()->m_space_type = SPACE_PRIVATE;
		}
	}
	spTree->for_each(std::bind(&Snapshot::_set_directory_node_space_type, this, std::placeholders::_1));
}

void Snapshot::_set_directory_node_space_type( shared_ptr<directory_tree::node> spNode )
{
	shared_ptr<directory_tree::node> spParent = spNode->parent();
	if(spParent->parent())
	{
		spNode->file()->m_space_type = spParent->file()->m_space_type;
	}
}
