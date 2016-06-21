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


struct change_xfiles
{
	vector<xfile_shared_ptr> m_addXfile;
	vector<xfile_shared_ptr> m_updateXfile;
	vector<xfile_shared_ptr> m_deleteXfile;
};

class Snapshot
{
public:
	Snapshot(void);
	~Snapshot(void);

public:
	void loadSnapShot();
	void clear();
	void copyLocalFile(const string& path);

	void filterLocalChange(shared_ptr<change_xfiles> spChangeXfiles);
	void filterRemoteChange(shared_ptr<change_xfiles>spChangeXfiles);

	void snapshotWriteToDb();

private:
	void _loadLocalSnapshot();
	void _loadLastSnapshot();
	void _loadServerSnapshot();

	string _formatePath(const string& path);

	void _filterLocalAddAndUpdate(shared_ptr<directory_tree_node<string>> spLocalNode, shared_ptr<change_xfiles> spChangeXfiles );
	void _filterLocalDelete(shared_ptr<directory_tree_node<string>> spLastNode, shared_ptr<change_xfiles> spChangeXfiles );

	void _filterRemoteAddAndUpdateXfile(shared_ptr<directory_tree_node<string>> spNode_L, shared_ptr<change_xfiles> spChangeXfiles );
	void _filterRemoteDeleteXfile(shared_ptr<directory_tree_node<string>> spLastNode, shared_ptr<change_xfiles> spChangeXfiles );

	void _copyFile(const string& path, shared_ptr<directory_tree_node<string>> spNode);

	void _convertToVec(shared_ptr<directory_tree::node> spNode, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec);
private:
	string	m_rootPath;
	shared_ptr<directory_tree> m_spLocalTree;
	shared_ptr<directory_tree> m_spLastTree;
	shared_ptr<directory_tree> m_spServerTree;
	user_db m_db;
	snapshot_recorder m_snapshotRecorder;
};