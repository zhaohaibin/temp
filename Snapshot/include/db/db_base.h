/********************************************************************
created:	2015/09/24
created:	24:9:2015   14:16
filename: 	D:\KSYUN\new_client\pclite\src\include\db\db_base.h
file path:	D:\KSYUN\new_client\pclite\src\include\db
file base:	db_base
file ext:	h
author:		zhaohaibin

purpose:	
*********************************************************************/
#ifndef DB_BASE_H
#define DB_BASE_H

#include "CrossPlatformConfig.h"
#include <string>
#include <cstdint>
#include <sqlite3/sqlite3.h>


class sqlit_blob
{
public:
	void append(const char* p_buf, unsigned int buf_len)
	{
		m_val.append(p_buf, buf_len);
	}
	void append(const std::string& str)
	{
		m_val.append(str);
	}
	uint64_t size()
	{
		return m_val.size();
	}

	const char* get()
	{
		return m_val.c_str();
	}

    void reset()
    {
        m_val.clear();
    }
private:
	std::string m_val;
};

class sqlit_db
{
public:
	sqlit_db();
	~sqlit_db();

public:
	//int initialize_db(const std::wstring& db_path);
	//db_path need utf8 encode
    int initialize_db(const std::string& db_path);

	//用于执行 sql语句(创建表，增，删，改)
	int exec_sql(const std::string& sql);
	int exec_sql(const char* sql);

	////////////////////////////////////////////////////////
	//step_1 sqlit_prepare
	//bind---根据需要选择
	//step_2 perform
	//step_3 release
	int sqlit_prepare(const char* sql)
	{
		int ret;
		ret = sqlite3_reset(m_p_sqlit_stmt);
		ret = sqlite3_prepare_v2( m_p_sqlite, sql, -1, &m_p_sqlit_stmt, 0 );
		return ret;
	}
	int perform()
	{
		return sqlite3_step(m_p_sqlit_stmt);
	}

	//////////////////////////////////////////////////事务相关接口////////////////////////////////////////
	//step_1	begin_transaction
	//step_2	其他数据库操作
	//step_3	没有错误commit_transaction否则rollback_transaction
	//开始一个事务
	int begin_transaction()
	{
		return sqlite3_exec ( m_p_sqlite , "begin transaction" , 0 , 0 ,0 ); 
	}

	//提交事务
	int commit_transaction()
	{
		return sqlite3_exec ( m_p_sqlite , "commit transaction" , 0 , 0 , 0 );
	}

	//回滚事务
	int rollback_transaction()
	{
		return  sqlite3_exec ( m_p_sqlite , "rollback transaction" , 0 , 0 , 0 );
	}

	////////////////////////////////////////////////////查询相关接口
	//step_1 exec_query
	//step_2 fetch_row
	//step_3 release
	int exec_query(const std::string& sql)
	{
		int ret;
		ret = sqlite3_reset(m_p_sqlit_stmt);
		ret = sqlite3_prepare_v2( m_p_sqlite, sql.c_str(), -1, &m_p_sqlit_stmt, 0 );
		return ret;
	}

	int exec_query(const char* sql)
	{
		int ret;
		ret = sqlite3_reset(m_p_sqlit_stmt);
		ret = sqlite3_prepare_v2( m_p_sqlite, sql, -1, &m_p_sqlit_stmt, 0 );
		return ret;
	}

	template<typename T>
	int fetch_row(T& val)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val, m_p_sqlit_stmt, 0);
		}
		return ret;
	}

	template<typename T1, typename T2>
	int fetch_row(T1& val_1, T2& val_2)
	{	
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, T6& val_6)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, T6& val_6, T7& val_7)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5,
		T6& val_6, T7& val_7, T8& val_8, T9& val_9)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9, typename T10>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, T11& val_11)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9, typename T10, 
		typename T11, typename T12>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5,
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12, T13& val_13)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
			get(val_13, m_p_sqlit_stmt, 12);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12, T13& val_13, T14& val_14)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
			get(val_13, m_p_sqlit_stmt, 12);
			get(val_14, m_p_sqlit_stmt, 13);
		}
		return ret;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14,typename T15>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12, T13& val_13, T14& val_14, T15& val_15)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
			get(val_13, m_p_sqlit_stmt, 12);
			get(val_14, m_p_sqlit_stmt, 13);
			get(val_15, m_p_sqlit_stmt, 14);

		}
		return ret;
	}
	int release()
	{
		int ret = 0;
		if(m_p_sqlit_stmt)
		{
			ret = sqlite3_finalize(m_p_sqlit_stmt);
			m_p_sqlit_stmt = 0;
		}
		return ret;
	}

private:
	//定义取出当前行中的指定列的值
	template<typename T>
	void get(T& val, sqlite3_stmt* p_stmt, const int index){}

	void get(int& val, sqlite3_stmt* p_stmt, const int index)
	{
		val = sqlite3_column_int(p_stmt, index);
	}

	void get(int64_t& val, sqlite3_stmt* p_stmt, const int index)
	{
		val =  sqlite3_column_int64(p_stmt, index);
	}

	void get(uint64_t& val, sqlite3_stmt* p_stmt, const int index)
	{
		val = sqlite3_column_int64(p_stmt, index);
	}

	void get(double& val, sqlite3_stmt* p_stmt, const int index)
	{
		val = sqlite3_column_double(p_stmt, index);
	}

	void get(std::string& val , sqlite3_stmt* p_stmt, const int index)
	{
		val.clear();
		val.append((const char*)sqlite3_column_text(p_stmt, index), sqlite3_column_bytes(p_stmt, index) );
	}

	void get(sqlit_blob& val, sqlite3_stmt* p_stmt, const int index)
	{
        val.reset();
        const char* pSrc = (const char*)sqlite3_column_blob(p_stmt, index);
        int srcLen = sqlite3_column_bytes(p_stmt, index);
        if(pSrc != NULL && srcLen != 0 )
        {
            val.append(pSrc,srcLen);
        }
		//val.append((const char*)sqlite3_column_blob(p_stmt, index), sqlite3_column_bytes(p_stmt, index) );
	}
public:
	//定义bind
	int bind(int index, sqlit_blob& blob, sqlite3_destructor_type destructor =  SQLITE_TRANSIENT)
	{
		return sqlite3_bind_blob(m_p_sqlit_stmt, index, blob.get(), blob.size(), destructor);
	}

	int bind(int index, const std::string& str, sqlite3_destructor_type destructor =  SQLITE_TRANSIENT)
	{
		return sqlite3_bind_text(m_p_sqlit_stmt, index, str.c_str(), str.size(), destructor);
	}
private:
	sqlite3* m_p_sqlite;
	sqlite3_stmt* m_p_sqlit_stmt;
};
#endif