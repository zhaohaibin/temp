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
//     /*�޸ı�����
//      ����һ����¼��ʱ>=200ms�������ϴ������ļ�������ɺ󣬻�ķ��൱һ����ʱ����ִ�в�����ɱ��Ӷ��û������ļ������ϴ�ʱ����ΪDB�߳�һֱæ���޷���Ӧ��
//      ���»�ܳ�һ��ʱ���ڽ�����û����Ӧ���Ѵ˿��عرպ󣬴���ΪΪ4ms. ���ܴ�����������ٳ��ֵȴ�ʱ�䡣
//         */
//     return exec_sql("PRAGMA synchronous=OFF");// �ر�ÿ��������Ӳ��ͬ��������������ܻ������Ĭ��ΪFULL��
// #endif
//     return 0;
// }


int sqlit_db::initialize_db(const std::string& db_path)
{
    sqlite3_open_v2(db_path.c_str(), &m_p_sqlite, sql_openmode, 0);
	return exec_sql("PRAGMA synchronous=OFF");// �ر�ÿ��������Ӳ��ͬ��������������ܻ������Ĭ��ΪFULL��
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

