#include <fstream>
#include <ctime>

#include "snapshot_recorder.h"
#include "JsonCpp/json.h"
#include "Snapshot.h"
#include "file_system_helper.h"



snapshot_recorder::snapshot_recorder(void)
{
	_initTimeSplit();
}

snapshot_recorder::snapshot_recorder( const string& rootPaht )
	: m_rootPath(rootPaht)
{
	_initTimeSplit();
}


snapshot_recorder::~snapshot_recorder(void)
{
}

void snapshot_recorder::make_record_path()
{
	m_recordPath = m_rootPath + "/" + time_helper::format_long_time(time(0),m_timeSplit);
	string error;
	kingfile::filesystem::make_dirs(m_recordPath, error);
}

void snapshot_recorder::set_root_path( const string& rootPaht )
{
	m_rootPath = rootPaht + "/" + time_helper::format_short_time(time(0), m_shortTimeSplit);
}

void snapshot_recorder::record_local_snapshot( vector<xfile_shared_ptr>& xfiles )
{
	string path = m_recordPath + "/localsnapshot.txt";
	_write(path, xfiles);
}

void snapshot_recorder::record_last_snapshot( vector<boost::any>& xfiles )
{
	string path = m_recordPath + "/lastsnapshot.txt";
	ofstream stream;
	stream.open(path, ios::trunc);
	if(stream.is_open())
	{
		for(int i = 0; i < xfiles.size(); ++i)
		{
			_write(stream, boost::any_cast<xfile_shared_ptr>(xfiles[i]));
		}
	}
}

void snapshot_recorder::record_server_snapshot( map<uint64_t, xfile_shared_ptr>& xfiles )
{
	string path = m_recordPath + "/serversnapshot.txt";
	ofstream stream;
	stream.open(path, ios::trunc);
	if(stream.is_open())
	{
		map<uint64_t, xfile_shared_ptr>::iterator it = xfiles.begin();
		for(; it != xfiles.end(); ++it)
		{
			_write(stream, it->second);
		}
	}
}

void snapshot_recorder::record_local_changes( shared_ptr<change_xfiles> spChanges )
{
	string local_add_path = m_recordPath + "/change_local_add.txt";
	string local_update_path = m_recordPath + "/change_local_update.txt";
	string local_delete_path = m_recordPath + "/change_local_delete.txt";

	_write(local_add_path, spChanges->m_addXfile);
	_write(local_update_path, spChanges->m_updateXfile);
	_write(local_delete_path, spChanges->m_deleteXfile);
}

void snapshot_recorder::record_server_changes( shared_ptr<change_xfiles> spChanges )
{
	string server_add_path = m_recordPath + "/change_server_add.txt";
	string server_update_path = m_recordPath + "/change_server_update.txt";
	string server_delete_path = m_recordPath + "/change_server_delete.txt";

	_write(server_add_path, spChanges->m_addXfile);
	_write(server_update_path, spChanges->m_updateXfile);
	_write(server_delete_path, spChanges->m_deleteXfile);
}

void snapshot_recorder::_write( const std::string& path, vector<xfile_shared_ptr>& xfiles )
{
	
	ofstream stream;
	stream.open(path, ios::trunc);
	if(stream.is_open())
	{
		for(int i = 0; i < xfiles.size(); ++i )
		{
			_write(stream, xfiles[i]);
		}
		stream.close();
	}
}

void snapshot_recorder::_write( ofstream& stream, xfile_shared_ptr spXfile )
{
	Json::Value jobj;
	spXfile->to_json(jobj);
	string str = Json::FastWriter().write(jobj);
	stream << str;
}

void snapshot_recorder::_initTimeSplit()
{
	m_timeSplit.split_year = "-";
	m_timeSplit.split_month = "-";
	m_timeSplit.split_day = " ";
	m_timeSplit.split_hour = "_";
	m_timeSplit.split_minute = "_";
	m_timeSplit.split_second = "";

	m_shortTimeSplit = m_timeSplit;
	m_shortTimeSplit.split_day = "";
}

