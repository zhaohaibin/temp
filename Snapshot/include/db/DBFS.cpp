/* -------------------------------------------------------------------------
//	FileName    : DBFS.cpp
//	Author      : tanzhongbei
//	Create Time : 2012-3-31-14:58:04
//	Description :	
//
// -----------------------------------------------------------------------*/

#include "StdAfx.h"
#include "DBFS.h"
#include "filehlper.h"
#include "base.h"
#include "StringConv\StringConv.h"
//#include "..\kuaipancontroller\Misc\StringHelper.h"
#include "StringHelper\StringHelper.h"
#include "..\..\XLiveShell\Editor\Core\TaskInfo.h"
#include "..\..\base\base\memory\scoped_ptr.h"
#include "sql_define.h"
#include "..\..\LibXLive\Misc\StringHelper.h"
#include "base\strings\utf_string_conversions.h"
#include "table_data.h"

#define _def_SQL_query_value					"SELECT value FROM config WHERE section == @section and field == @field"

#define _def_SQL_update_config					"insert or replace into config (section, field, value) \
												values(@section,@field,@value)"

#define _def_SQL_openmode						(SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_WAL)

#define _def_SQL_get_dbversion					"select dbversion from db_info"
#define _def_SQL_insert_dbversion				"insert into db_info(dbversion) values(@dbversion)"
#define _def_SQL_update_dbversion				"update db_info set dbversion=@dbversion"

#define _def_SQL_get_allHistoryTask             "select * from history_task order by task_ctime desc"


#define _def_SQL_update_historyTask              "UPDATE  history_task SET task_ctime = @task_ctime, file_id = @file_id, parent_id=@parent_id, file_path = @file_path,\
                                                  sha1 = @sha1,  is_folder = @is_folder,  \
                                                  file_size = @file_size, cur_size = @cur_size, \
                                                  error_code = @error_code, task_type = @task_type \
                                                  WHERE id == @id"

// 3种作用 : 1-隐藏; 2-持久; 3-初始化为0
static const Int32 s_dbVersion = 4;

CDBbase::CDBbase()
{

}
CDBbase::~CDBbase()
{

}

ErrorNo CDBbase::init(LPCTSTR szPath)
{
	ErrorNo ret = XLIVE_OK;
	{
		ret = _initDBEnv(szPath);
		ERROR_CHECK_BOOL(XLIVE_OK == ret);
	}
Exit0:
	return ret;
}

ErrorNo CDBbase::uninit()
{
	ErrorNo ret = XLIVE_OK;
	{
		ret = _uninitDBEnv();
		ERROR_CHECK_BOOL(XLIVE_OK == ret);
	}
Exit0:
	return XLIVE_OK;
}

ErrorNo CDBbase::beginTransaction()
{
	UInt32 ret = SQLITE_OK;
	////m_DBlock.Enter();
	{
		ret = m_db.exec("BEGIN TRANSACTION;");
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	////m_DBlock.Leave();
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;
	return (ErrorNo)ret;    
}

ErrorNo CDBbase::endTransaction(int error)
{
	if(error == SQLITE_OK || error == SQLITE_DONE || error == SQLITE_ROW)
		return commitTransaction();
	else
		return rollbackTransaction();	  
}

ErrorNo CDBbase::commitTransaction()
{
	UInt32 ret = SQLITE_OK;
	////m_DBlock.Enter();
	{
		ret = m_db.exec("COMMIT TRANSACTION;");
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	//m_DBlock.Leave();
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;
	return (ErrorNo)ret;  
}

ErrorNo CDBbase::rollbackTransaction()
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		ret = m_db.exec("ROLLBACK TRANSACTION;");
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	//m_DBlock.Leave();
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;
	return (ErrorNo)ret;    
}

//////////////////////////////////////////////////////////////////////////
// private
ErrorNo CDBbase::_initDBEnv(LPCTSTR szPath)
{
	UInt32 ret = SQLITE_OK;
	{
		USES_CONVERSION;
		ret = m_db.open(W2A_CP(szPath, CP_UTF8), _def_SQL_openmode);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	
		int db_version;
		if( get_db_version(db_version) == XLIVE_OK )
		{
			if(db_version != s_dbVersion)
			{
				update_db_version(s_dbVersion);
				delete_table();
			}
		}else
		{
			initialize_db_version_table();
			delete_table();
		}
		ret = _createTables();
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBbase::_uninitDBEnv()
{
	UInt32 ret = SQLITE_OK;
	{
		m_ExcST->reset();
		ret = m_db.close();
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

std::wstring CDBbase::get_app_version()
{
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO * pFileInfo;

	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);

		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		int ret = GetLastError();
		WORD m_nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource 
		m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS); 
		m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS); 

		std::wstring strVersion ;
		wchar_t version_array[32] = {0};

		wsprintf(version_array,L"%d.%d.%d.%d", m_nProdVersion[0],m_nProdVersion[1],m_nProdVersion[2],m_nProdVersion[3]);

		strVersion.append(version_array);

		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
	return L"";
}

int CDBbase::get_db_version( int& db_version )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		USES_CONVERSION;
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_get_version);
		SQLite::Result result = m_ExcST->query();
		SQLite::ResultPtr presult(result);

		ret = presult->fetchRow(db_version);
		ERROR_CHECK_BOOL(SQLITE_ROW == ret);
	}
Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE == ret || SQLITE_ROW == ret)
		ret = XLIVE_OK;
	else
		ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}

void CDBbase::delete_table()
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		ret = m_db.exec(sql_drop_history_task_table);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
}

ErrorNo CDBbase::create_db_version_table()
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		ret = m_db.exec(sql_create_db_version);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_OK == ret)
		ret = XLIVE_OK;
	else
		ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}

ErrorNo CDBbase::insert_db_version( int db_version )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_insert_db_version);
		m_ExcST->bind(1, db_version);
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}
Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;
	return (ErrorNo)ret;
}

void CDBbase::initialize_db_version_table()
{
	if( create_db_version_table() == XLIVE_OK)
	{
		insert_db_version(s_dbVersion);
	}
}

ErrorNo CDBbase::update_db_version( int db_version )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_update_db_version);
		m_ExcST->bind(1, db_version);
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}
Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;
	return (ErrorNo)ret;
}


//////////////////////////////////////////////////////////////////////////
//

CDBConfig::CDBConfig()
{
}

CDBConfig::~CDBConfig()
{

}

ErrorNo CDBConfig::setItem(LPCTSTR szSection, LPCTSTR szField, LPCTSTR szValue)
{
 	UInt32 ret = SQLITE_OK;
 	//m_DBlock.Enter();
 	{
		USES_CONVERSION;
 		m_ExcST->reset();
 		m_ExcST = m_db.prepare(_def_SQL_update_config);
 		m_ExcST->bind(1, W2A_CP(szSection, CP_UTF8));
		m_ExcST->bind(2, W2A_CP(szField, CP_UTF8));
		m_ExcST->bind(3, W2A_CP(szValue, CP_UTF8));
 		ret = m_ExcST->exec();
 		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
 	}
 Exit0:
 	m_ExcST->release();
 	//m_DBlock.Leave();
 	if (SQLITE_DONE != ret)
 		ret |= XLIVE_SQLITE_MASK;
 	else
 		ret = XLIVE_OK;
 	return (ErrorNo)ret;
}

ErrorNo CDBConfig::queryItem(LPCTSTR szSection, LPCTSTR szField, CString& strValue, LPCTSTR szDefault)
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		USES_CONVERSION;
		m_ExcST->reset();
		m_ExcST = m_db.prepare(_def_SQL_query_value);
		m_ExcST->bind(1, W2A_CP(szSection, CP_UTF8));
		m_ExcST->bind(2, W2A_CP(szField, CP_UTF8));
 		SQLite::Result result = m_ExcST->query();
 		SQLite::ResultPtr presult(result);
 		std::string stdValue;
 		ret = presult->fetchRow(stdValue);
		if (SQLITE_ROW != ret)
			strValue = szDefault;
		else
			strValue = A2W_CP(stdValue.c_str(), CP_UTF8);
	}
 	m_ExcST->release();
 	//m_DBlock.Leave();
 	if (SQLITE_DONE == ret || SQLITE_ROW == ret)
 		ret = XLIVE_OK;
     else
         ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}



ErrorNo CDBConfig::_createTables()
{
	UInt32 ret = SQLITE_OK;
	{
		const char* sql_CreateconfigTable = 
			"CREATE TABLE IF NOT EXISTS config("
			"section		TEXT	,"
			"field			TEXT	,"
			"value			TEXT	,"
			"UNIQUE (section, field))";
		ret = m_db.exec(sql_CreateconfigTable);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);

		ret = m_db.exec(sql_create_db_version);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}

Exit0:
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}



/////////CDBUser/////

CDBUser::CDBUser()
{

}
CDBUser::~CDBUser()
{

}

ErrorNo CDBUser::getAllTasks(HistoryTaskList& allTasks)
{
	//return XLIVE_OK;
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		USES_CONVERSION;
		m_ExcST->reset();
		m_ExcST = m_db.prepare(_def_SQL_get_allHistoryTask);
		SQLite::Result result = m_ExcST->query();
		SQLite::ResultPtr presult(result);
	
		
		UInt64 fileId, parentId;
		std::string  filePath, sha1;
		Int32 bFolder, taskType, errorCode, taskCreateTime, id;
		Int64 fileSize, curSize;
		ret = presult->fetchRow(id, taskCreateTime, fileId, parentId, filePath, sha1, bFolder, fileSize, curSize, errorCode, taskType);

		while(SQLITE_ROW == ret)
		{
			HistoryTask_Ptr ptr = HistoryTask_Ptr(new HistoryTaskInfo);
			ptr->m_taskId = id;
			//ptr->m_taskInfo.m_taskId = id;
			ptr->m_taskInfo.m_fileId        = fileId;
			ptr->m_taskInfo.m_parentId      = parentId;
			ptr->m_taskInfo.m_filePath   	= filePath;
			ptr->m_taskInfo.m_sha1      	= sha1;
			ptr->m_taskInfo.m_fileSize   	= fileSize;
			ptr->m_taskInfo.m_taskType      = (TaskType::type)taskType;
			ptr->m_taskInfo.m_isFolder      = bFolder;
			
			ptr->m_curSize			= curSize;
			ptr->m_errorCode		= errorCode;
			ptr->m_taskCreateTime   = taskCreateTime;
			
			allTasks.push_back(ptr);
			ret = presult->fetchRow(id, taskCreateTime, fileId, parentId, filePath, sha1, bFolder, fileSize, curSize, errorCode, taskType);
		} 
	}
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE == ret || SQLITE_ROW == ret)
		ret = XLIVE_OK;
	else
		ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::insertTask(HistoryTask_Ptr task)
{
	return ErrorNo(0);
	UInt32 ret = SQLITE_OK;
	{
		if (task->is_newTask())
		{
			ret = _addTask(task);
		}
		else
		{
			ret = _updateTask(task);
		}
	}
	return (ErrorNo)ret;
}


ErrorNo CDBUser::_addTask(HistoryTask_Ptr task)
{
 	UInt32 ret = SQLITE_OK;
// 	//m_DBlock.Enter();
// 	{
// 		//	groupid,fileid,filename,operator,time,useremail,username,path
// 		m_ExcST->reset();
// 		m_ExcST = m_db.prepare(_def_SQL_insert_historyTask);
// 
// 		m_ExcST->bind(1, (Int64)task->m_taskCreateTime);
// 		m_ExcST->bind(2, task->m_taskInfo.m_fileId, SQLITE_TRANSIENT);
// 		m_ExcST->bind(3, task->m_taskInfo.m_parentId, SQLITE_TRANSIENT);
// 		m_ExcST->bind(4, task->m_taskInfo.m_filePath, SQLITE_TRANSIENT);
// 		m_ExcST->bind(5, task->m_taskInfo.m_sha1, SQLITE_TRANSIENT);
// 		m_ExcST->bind(6, task->m_taskInfo.m_isFolder);
// 		m_ExcST->bind(7, (Int64)task->m_taskInfo.m_fileSize);
// 		m_ExcST->bind(8, (Int64)task->m_curSize);
// 		m_ExcST->bind(9, (Int32)task->m_errorCode);
// 		m_ExcST->bind(10, task->m_taskInfo.m_taskType);
// 
// 		ret = m_ExcST->exec();
// 		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
// 
// 	}
// 
// Exit0:
// 	m_ExcST->release();
// 	//m_DBlock.Leave();
// 	if (SQLITE_DONE != ret)
// 		ret |= XLIVE_SQLITE_MASK;
// 	else
// 		ret = XLIVE_OK;
// 
 	return (ErrorNo)ret;
}

ErrorNo CDBUser::_updateTask(HistoryTask_Ptr task)
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
// 	{
// 		//	groupid,fileid,filename,operator,time,useremail,username,path
// 		m_ExcST->reset();
// 		m_ExcST = m_db.prepare(_def_SQL_update_historyTask);
// 
// 		m_ExcST->bind(1, (Int64)task->m_taskCreateTime);
// 		m_ExcST->bind(2, task->m_taskInfo.m_fileId, SQLITE_TRANSIENT);
// 		m_ExcST->bind(3, task->m_taskInfo.m_parentId, SQLITE_TRANSIENT);
// 		m_ExcST->bind(4, task->m_taskInfo.m_filePath, SQLITE_TRANSIENT);
// 		m_ExcST->bind(5, task->m_taskInfo.m_sha1, SQLITE_TRANSIENT);
// 		m_ExcST->bind(6, task->m_taskInfo.m_isFolder);
// 		m_ExcST->bind(7, (Int64)task->m_taskInfo.m_fileSize);
// 		m_ExcST->bind(8, (Int64)task->m_curSize);
// 		m_ExcST->bind(9, (Int32)task->m_errorCode);
// 		m_ExcST->bind(10, task->m_taskInfo.m_taskType);
// 		m_ExcST->bind(11, (Int64)task->m_taskId);
// 
// 		ret = m_ExcST->exec();
// 		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
// 	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::getCountOfFailTask(UInt32& count)
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		//	groupid,fileid,filename,operator,time,useremail,username,path
		m_ExcST->reset();
		m_ExcST = m_db.prepare("select count(*) from history_task where cur_size < file_size");

		SQLite::Result result = m_ExcST->query();
		SQLite::ResultPtr presult(result);

		ret = presult->fetchRow(count);
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::deleteTask(HistoryTask_Ptr task)
{
	UInt32 ret = SQLITE_OK;
// 	//m_DBlock.Enter();
// 	{
// 		//	groupid,fileid,filename,operator,time,useremail,username,path
// 		m_ExcST->reset();
// 		m_ExcST = m_db.prepare("delete from history_task where task_ctime=@task_ctime and file_id=@file_id");
// 		m_ExcST->bind(1, (Int64)task->m_taskCreateTime);
// 		m_ExcST->bind(2, task->m_taskInfo.m_fileId, SQLITE_TRANSIENT);
// 		
// 		ret = m_ExcST->exec();
// 		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
// 	}
// 
// Exit0:
// 	m_ExcST->release();
// 	//m_DBlock.Leave();
// 	if (SQLITE_DONE != ret)
// 		ret |= XLIVE_SQLITE_MASK;
// 	else
// 		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::clearFailTask()
{
	UInt32 ret = SQLITE_OK;
	//ret = XLIVE_OK;
	//return ErrorNo(ret);
	//m_DBlock.Enter();
	{
		//	groupid,fileid,filename,operator,time,useremail,username,path
		m_ExcST->reset();
		m_ExcST = m_db.prepare("delete from history_task where error_code > 0");

		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::clearSuccessTask()
{
/*	return XLIVE_OK;*/
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		//	groupid,fileid,filename,operator,time,useremail,username,path
		m_ExcST->reset();
		m_ExcST = m_db.prepare("delete from history_task where error_code=0");

		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::getDBVersion(__int64& dbVersion)
{
	dbVersion = 0;
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		USES_CONVERSION;
		m_ExcST->reset();
		m_ExcST = m_db.prepare(_def_SQL_get_dbversion);
		SQLite::Result result = m_ExcST->query();
		SQLite::ResultPtr presult(result);

		ret = presult->fetchRow(dbVersion);
	}
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE == ret || SQLITE_ROW == ret || SQLITE_OK == ret)
		ret = XLIVE_OK;
	else
		ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::_createTables()
{
	UInt32 ret = CDBConfig::_createTables();
	ERROR_CHECK_BOOL(SQLITE_OK == ret);
	{
		const char* sql_CreateHistoryTask =
			"CREATE TABLE IF NOT EXISTS history_task("
			"id             INTEGER         PRIMARY KEY AUTOINCREMENT,"
			"task_ctime     INTEGER						  ,"
			"file_id      INTEGER,"
			"parent_id    INTEGER,"
			"file_path    VACHAR(260)       COLLATE NOCASE,"
			"sha1         VACHAR(42)        COLLATE NOCASE,"
			"is_folder      INTEGER                       ,"
			"file_size      INTEGER                       ,"
			"cur_size       INTEGER                       ,"
			"error_code     INTEGER                       ,"
			"task_type      INTEGER                       )";
	
		ret = m_db.exec(sql_CreateHistoryTask);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);

		const char* sql_create_cache_block_list = "CREATE TABLE IF NOT EXISTS block_list ("
			"id INTEGER ,"
			"name VACHAR (260) COLLATE NOCASE,"
			"path VACHAR (260) COLLATE NOCASE)";
		ret = m_db.exec(sql_create_cache_block_list);

		const char* sql_create_file_change_message = "CREATE TABLE IF NOT EXISTS file_change_message("
			"ID	INTEGER	PRIMARY KEY AUTOINCREMENT,"
			"P_XID	INTEGER,"
			"XID	INTEGER,"
			"SPACE_TYPE	INTEGER,"
			"NAME		VACHAR(260)       COLLATE NOCASE,"
			"OP_TIME	INTEGER,"
			"FILE_TYPE	INTEGER,"
			"OP_TYPE		INTEGER,"
			"CONTENT	VACHAR(260),"
			"STATE		INTEGER)";
		ret = m_db.exec(sql_create_file_change_message);
		ERROR_CHECK_BOOL(SQLITE_OK == ret);
	}
Exit0:
	if (SQLITE_OK != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;
	return (ErrorNo)ret;
}

ErrorNo CDBUser::insert_file_change_message(std::shared_ptr<Json::Value> p_value )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_insert_to_file_change_message);
		m_ExcST->bind(1, (Int64)(*p_value)["P_XID"].asInt64());
		m_ExcST->bind(2, (Int64)(*p_value)["XID"].asInt64());
		m_ExcST->bind(3, (*p_value)["SPACE_TYPE"].asInt());
		m_ExcST->bind(4, (*p_value)["NAME"].asString(), SQLITE_TRANSIENT);
		m_ExcST->bind(5, (Int64)(*p_value)["OP_TIME"].asInt64());
		m_ExcST->bind(6, (*p_value)["FILE_TYPE"].asInt());
		m_ExcST->bind(7,  (*p_value)["OP_TYPE"].asInt());
		m_ExcST->bind(8, (*p_value)["CONTENT"].asString(), SQLITE_TRANSIENT);
		m_ExcST->bind(9, (*p_value)["STATE"].asInt());
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
		if(SQLITE_DONE == ret)
		{
			wstring wstr = base::ASCIIToWide(StringHelper::i64ToString((*p_value)["XID"].asInt64()));
			wstr += L"\n";
			OutputDebugString(wstr.c_str());
		}
	}
Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	uint64 id;
	if(ret == XLIVE_OK)
		ret = get_max_id(id, sql_sel_max_id_message_tb);
	if(ret == XLIVE_OK)
		(*p_value)["ID"] = id;
	return (ErrorNo)ret;
}

ErrorNo CDBUser::query_file_change_message(std::shared_ptr<Json::Value> p_value )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		USES_CONVERSION;
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_query_all_from_file_change_message);
		SQLite::Result result = m_ExcST->query();
		SQLite::ResultPtr presult(result);

		file_change_message_table_data sel_data;
		ret = presult->fetchRow(
			 sel_data.m_id,sel_data.m_pxid, sel_data.m_xid, 
			 sel_data.m_space_type, sel_data.m_name, sel_data.m_op_time,
			 sel_data.m_file_type, sel_data.m_op_type, sel_data.m_content,
			 sel_data.m_state);

		while(SQLITE_ROW == ret)
		{
			p_value->append(sel_data.to_json());
			ret = presult->fetchRow(
				sel_data.m_id, sel_data.m_pxid, sel_data.m_xid, 
				sel_data.m_space_type, sel_data.m_name, sel_data.m_op_time, 
				sel_data.m_file_type, sel_data.m_op_type, sel_data.m_content,
				sel_data.m_state);
		} 
	}
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE == ret || SQLITE_ROW == ret)
		ret = XLIVE_OK;
	else
		ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::delete_one_item_from_file_change_message( const std::shared_ptr<Json::Value> p_value )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_del_one_message_base_id);
		m_ExcST->bind(1, (Int64)(*p_value)["ID"].asInt64());
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::get_max_id( uint64& max_id ,const std::string& sql)
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql.c_str());
		SQLite::Result result = m_ExcST->query();
		SQLite::ResultPtr presult(result);

		file_change_message_table_data sel_data;
		ret = presult->fetchRow(max_id);
	}
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE == ret || SQLITE_ROW == ret)
		ret = XLIVE_OK;
	else
		ret |= XLIVE_SQLITE_MASK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::clear_file_change_message( std::shared_ptr<Json::Value> p_value )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_clear_message_table);
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::insert_to_history_task( std::shared_ptr<Json::Value> p_value )
{
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_insert_historyTask);
		m_ExcST->bind(1, (Int64)(*p_value)["CTIME"].asInt64());
		m_ExcST->bind(2, (Int64)(*p_value)["XID"].asInt64());
		m_ExcST->bind(3, (*p_value)["PXID"].asInt64());
		m_ExcST->bind(4, (*p_value)["PATH"].asString(), SQLITE_TRANSIENT);
		m_ExcST->bind(5, (*p_value)["SHA1"].asString(), SQLITE_TRANSIENT);
		m_ExcST->bind(6, (*p_value)["FILE_TYPE"].asInt());
		m_ExcST->bind(7,  (*p_value)["FILE_SIZE"].asInt64());
		m_ExcST->bind(8, (*p_value)["CURENT_SIZE"].asInt64());
		m_ExcST->bind(9, (*p_value)["ERROR_CODE"].asInt());
		m_ExcST->bind(10,(*p_value)["TASK_TYPE"].asInt());
		string path = (*p_value)["PATH"].asString();
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}
Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	uint64 id;
	if(ret == XLIVE_OK)
		ret = get_max_id(id, sql_sel_max_id_from_history_task);
	if(ret == XLIVE_OK)
		(*p_value)["ID"] = id;
	return (ErrorNo)ret;
}

ErrorNo CDBUser::delete_one_item_from_history_task( const std::shared_ptr<Json::Value> p_value )
{
	UInt32 ret = SQLITE_OK;
	Int64 ctime = (*p_value)["CTIME"].asInt64();
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_del_item_from_history_task);
		m_ExcST->bind(1, (Int64)(*p_value)["CTIME"].asInt64());
		m_ExcST->bind(2, (*p_value)["PATH"].asString());
		std::string path = (*p_value)["PATH"].asString();
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::update_cur_size_in_history_task( const std::shared_ptr<Json::Value> p_value )
{
	//return XLIVE_OK;
	Int64 ctime = (*p_value)["CTIME"].asInt64();
	Int64 xid = (*p_value)["XID"].asInt64();
	xid = 0;
	Int64 cur_size = (*p_value)["CURENT_SIZE"].asInt64();
	std::string file_path = (*p_value)["PATH"].asString();
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_upd_cur_size_in_history_task);
		m_ExcST->bind(1, cur_size);
		m_ExcST->bind(2, ctime);
		//m_ExcST->bind(3, xid);
		m_ExcST->bind(3, file_path);
		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}

ErrorNo CDBUser::update_error_code_in_history_task( const std::shared_ptr<Json::Value> p_value )
{
	//return XLIVE_OK;
	Int64 ctime = (*p_value)["CTIME"].asInt64();
	Int64 xid = (*p_value)["XID"].asInt64();
	xid = 0;
	int error_code = (*p_value)["ERROR_CODE"].asInt();
	std::string file_path = (*p_value)["PATH"].asString();
	UInt32 ret = SQLITE_OK;
	//m_DBlock.Enter();
	{
		m_ExcST->reset();
		m_ExcST = m_db.prepare(sql_upd_error_code_in_history_task);
		m_ExcST->bind(1, error_code);
		m_ExcST->bind(2, ctime);
		//m_ExcST->bind(3, xid);
		m_ExcST->bind(3, file_path);

		ret = m_ExcST->exec();
		ERROR_CHECK_BOOL(SQLITE_DONE == ret);
	}

Exit0:
	m_ExcST->release();
	//m_DBlock.Leave();
	if (SQLITE_DONE != ret)
		ret |= XLIVE_SQLITE_MASK;
	else
		ret = XLIVE_OK;

	return (ErrorNo)ret;
}
