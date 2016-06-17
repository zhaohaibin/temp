/* -------------------------------------------------------------------------
//	FileName    : DBFS.h
//	Author      : tanzhongbei
//	Create Time : 2012-3-31-14:52:25
//	Description : 	
//
// -----------------------------------------------------------------------*/

#ifndef __DBFS_h__
#define __DBFS_h__

#include "libxlive/CrossPlatformConfig.h"
#include "libxlive/LibXLiveError.h"
#include "SQLiteWarpper.h"
#include <atlpath.h>
#include <atlsync.h>
#include "../../XLiveShell/Editor/xliveshell_def.h"

class CDBbase
{
public:
	CDBbase();
	~CDBbase();

public:
	ErrorNo init(LPCTSTR szPath);
	ErrorNo uninit();

private:
	ErrorNo _initDBEnv(LPCTSTR path);
	ErrorNo _uninitDBEnv();

protected:
	SQLite::Database				m_db;
	SQLite::StatementPtr			m_ExcST;
	//CCriticalSection				m_DBlock;

	ErrorNo beginTransaction();
	ErrorNo endTransaction(int error);
	ErrorNo commitTransaction();
	ErrorNo rollbackTransaction();
	virtual ErrorNo _createTables() = 0;

	std::wstring get_app_version(); 

	
	int get_db_version(int& db_version);

	void delete_table();

	ErrorNo create_db_version_table();
	ErrorNo insert_db_version(int db_version);
	ErrorNo update_db_version(int db_version);
	void initialize_db_version_table();

};


// 配置信息存储KEY-VALUE
class CDBConfig : public CDBbase
{
public:
	CDBConfig();
	~CDBConfig();
public:
	// query
 	ErrorNo setItem(LPCTSTR szSection, LPCTSTR szField, LPCTSTR szValue);
	ErrorNo queryItem(LPCTSTR szSection, LPCTSTR szField, CString& strValue,LPCTSTR szDefault = L"");
    // transction
protected:
	virtual ErrorNo _createTables();
};

// 用户信息配置
class CDBUser: public CDBConfig
{
public:
	CDBUser();
	~CDBUser();
public:
	// query

	ErrorNo getAllTasks(HistoryTaskList& allTasks);
	ErrorNo insertTask(HistoryTask_Ptr task);
	ErrorNo deleteTask(HistoryTask_Ptr task);
	ErrorNo getCountOfFailTask(UInt32& count);

	ErrorNo clearFailTask();
	ErrorNo clearSuccessTask();
	ErrorNo getDBVersion(__int64& dbVersion);


	ErrorNo insert_file_change_message(std::shared_ptr<Json::Value> p_value);
	ErrorNo query_file_change_message(std::shared_ptr<Json::Value> p_value);					//查询表中全部消息
	ErrorNo delete_one_item_from_file_change_message(const std::shared_ptr<Json::Value> p_value);
	ErrorNo clear_file_change_message(std::shared_ptr<Json::Value> p_value);
	

	///////////////////////////////history_task表相关函数/////////////////////////////////////////////////////////////////
	ErrorNo insert_to_history_task(std::shared_ptr<Json::Value> p_value);
	ErrorNo delete_one_item_from_history_task(const std::shared_ptr<Json::Value> p_value);
	ErrorNo update_cur_size_in_history_task(const std::shared_ptr<Json::Value> p_value);
	ErrorNo update_error_code_in_history_task(const std::shared_ptr<Json::Value> p_value);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ErrorNo get_max_id(uint64& max_id, const std::string& sql);
private:
	ErrorNo _addTask(HistoryTask_Ptr task);
	ErrorNo _updateTask(HistoryTask_Ptr task);

protected:
	virtual ErrorNo _createTables();
	
};
// 
#endif	/*__DBFS_h__*/