#pragma once
#include <boost/any.hpp>
#include <string>
#include <vector>
#include <map>

#include "xfile.h"
#include "time_helper.h"

using namespace std;
using namespace kingfile::filesystem;

struct change_xfiles;
class snapshot_recorder
{
public:
	snapshot_recorder(void);
	snapshot_recorder(const string& rootPaht);
	~snapshot_recorder(void);

	void make_record_path();
	void set_root_path(const string& rootPaht);

	void record_local_snapshot(vector<xfile_shared_ptr>& xfiles);
	void record_last_snapshot(vector<boost::any>& xfiles);
	void record_server_snapshot(map<uint64_t, xfile_shared_ptr>& xfiles);

	void record_local_changes(shared_ptr<change_xfiles> spChanges);
	void record_server_changes(shared_ptr<change_xfiles> spChanges);
private:
	void _write(const std::string& path, vector<xfile_shared_ptr>& xfiles);
	void _write(ofstream& stream, xfile_shared_ptr spXfile);
	void _initTimeSplit();
private:
	string m_rootPath;
	string m_recordPath;
	time_helper::time_split	m_timeSplit; 
	time_helper::time_split	m_shortTimeSplit;
};