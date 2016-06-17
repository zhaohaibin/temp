#define USING_LOG
#include "db_base.h"

//#include "shelllog\log.h"

#define sql_openmode						(SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_WAL | SQLITE_OPEN_FULLMUTEX)

sqlit_db::sqlit_db()
	:m_p_sqlit_stmt(0)
	,m_p_sqlite(0)
{

}

sqlit_db::~sqlit_db()
{
	if(m_p_sqlite)
		sqlite3_close(m_p_sqlite);
	release();
}


// int sqlit_db::initialize_db(const std::wstring& db_path )
// {
// #ifdef _WIN32
// 	/*return*/ sqlite3_open_v2(base::WideToUTF8(db_path).c_str(), &m_p_sqlite, sql_openmode, 0);
//     /*修改背景：
//      插入一条记录耗时>=200ms，导致上传大量文件任务完成后，会耗费相当一部门时间来执行插入完成表。从而用户再拖文件进来上传时，因为DB线程一直忙，无法响应，
//      导致会很长一段时间内界面上没有响应。把此开关关闭后，大致为为4ms. 性能大幅提升。不再出现等待时间。
//         */
//     return exec_sql("PRAGMA synchronous=OFF");// 关闭每个事务即与硬盘同步，否则插入性能会很慢。默认为FULL。
// #endif
//     return 0;
// }


int sqlit_db::initialize_db(const std::string& db_path)
{
    sqlite3_open_v2(db_path.c_str(), &m_p_sqlite, sql_openmode, 0);
	return exec_sql("PRAGMA synchronous=OFF");// 关闭每个事务即与硬盘同步，否则插入性能会很慢。默认为FULL。
}

int sqlit_db::exec_sql( const std::string& sql )
{
	char * pszErrMsg = NULL;
	int iRev = sqlite3_exec(m_p_sqlite, sql.c_str(), 0, 0,&pszErrMsg);
	if (pszErrMsg)
	{
		//std::wstring error_info = base::UTF8ToWide(pszErrMsg);
		//error_info += L" sql = ";
		//error_info += base::UTF8ToWide(sql);
		//LOG_ERROR2(error_info.c_str(), error_info.c_str());
		sqlite3_free(pszErrMsg);
	}
	return iRev;
}

int sqlit_db::exec_sql( const char* sql )
{
	char * pszErrMsg = NULL;
	int iRev = sqlite3_exec(m_p_sqlite, sql, 0, 0,&pszErrMsg);
	if (pszErrMsg)
	{
		//OutputDebugStringA(pszErrMsg);
		//std::wstring error_info = base::UTF8ToWide(pszErrMsg);
		//error_info += L" sql = ";
		//error_info += base::UTF8ToWide(sql);
		//LOG_ERROR2(error_info.c_str(), error_info.c_str());
		sqlite3_free(pszErrMsg);
	}
	return iRev;
}

