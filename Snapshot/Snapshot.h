#pragma once
#include "Xfile.h"
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>
#include "xfile.h"
#include "directory_tree.h"
#include "db/user_db.h"
#include "snapshot_recorder.h"
using namespace kingfile::filesystem;

#define PRIVATE	"个人空间"
#define PUBLIC		"共享空间"
struct change_xfiles
{
	void push_to_add(xfile_shared_ptr spXfile)
	{
		if(spXfile->is_directory())
			m_addFolder.push_back(spXfile);
		else
			m_addXfile.push_back(spXfile);
	}

	void push_to_update(xfile_shared_ptr spXfile)
	{
		m_updateXfile.push_back(spXfile);
	}

	void push_to_delete(xfile_shared_ptr spXfile)
	{
		if(spXfile->is_directory())
			m_deleteFolder.push_back(spXfile);
		else
			m_deleteXfile.push_back(spXfile);
	}
	vector<xfile_shared_ptr> m_addXfile;
	vector<xfile_shared_ptr> m_addFolder;
	vector<xfile_shared_ptr> m_updateXfile;
	vector<xfile_shared_ptr> m_deleteXfile;
	vector<xfile_shared_ptr> m_deleteFolder;
};

class Snapshot
{
public:
	Snapshot(void);
	~Snapshot(void);

public:
	bool loadSnapShot(string& error);
	void clear();
	void copyLocalFile(const string& path);

	void filterLocalChange(shared_ptr<change_xfiles> spChangeXfiles);
	void filterRemoteChange(shared_ptr<change_xfiles>spChangeXfiles);

	void snapshotWriteToDb();

private:
	bool _loadLocalSnapshot(string& error);
	bool _loadLastSnapshot(string& error);
	bool _loadServerSnapshot(string& error);

	string _formatePath(const string& path);

	void _filterLocalAddAndUpdate(shared_ptr<directory_tree_node<string>> spLocalNode, shared_ptr<change_xfiles> spChangeXfiles );
	void _filterLocalDelete(shared_ptr<directory_tree_node<string>> spLastNode, shared_ptr<change_xfiles> spChangeXfiles );

	void _filterRemoteAddAndUpdateXfile(shared_ptr<directory_tree_node<string>> spNode_L, shared_ptr<change_xfiles> spChangeXfiles );
	void _filterRemoteDeleteXfile(shared_ptr<directory_tree_node<string>> spLastNode, shared_ptr<change_xfiles> spChangeXfiles );

	void _copyFile(const string& path, shared_ptr<directory_tree_node<string>> spNode);

	void _convertToVec(shared_ptr<directory_tree::node> spNode, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec);

	void _set_space_type(shared_ptr<directory_tree> spTree);
	void _set_directory_node_space_type(shared_ptr<directory_tree::node> spNode);
private:
	string	m_rootPath;
	shared_ptr<directory_tree> m_spLocalTree;
	shared_ptr<directory_tree> m_spLastTree;
	shared_ptr<directory_tree> m_spServerTree;
	user_db m_db;
	snapshot_recorder m_snapshotRecorder;
};