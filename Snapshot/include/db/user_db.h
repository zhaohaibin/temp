/********************************************************************
	created:	2015/09/24
	created:	24:9:2015   15:18
	filename: 	D:\KSYUN\new_client\pclite\src\include\db\user_db.h
	file path:	D:\KSYUN\new_client\pclite\src\include\db
	file base:	user_db
	file ext:	h
	author:		zhaohaibin
	
	purpose:	
*********************************************************************/

#ifndef USER_DB_H
#define USER_DB_H
#include <memory>
#include <map>
#include "..\db_table.h"

using namespace std;
enum DB_TABLE
{
	TABLE_SNAPSHOT
};

class user_db
{
public:
	user_db(){}
	~user_db(){}

	// db_path need utf8 encode
	int initializeDb(const std::string& db_path);

	shared_ptr<db_table> getTable(DB_TABLE table);
private:
	int initializeTable();
	
private:
	sqlit_db m_db;
	map<DB_TABLE, shared_ptr<db_table>>	 m_tables;
};
#endif //USER_DB_H
