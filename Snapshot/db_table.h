#pragma once
#include <vector>
#include <string>
#include <boost/any.hpp>
#include "db\db_base.h"
using namespace std;
class sqlit_db;
class db_table
{
public:
	db_table(sqlit_db& db);
	~db_table(void);

	virtual int initialize();
	virtual void clear();

	virtual void insert(const vector<boost::any>& dataVec)	= 0;
	virtual int read_all(vector<boost::any>& dataVec)				= 0;
	virtual void read(vector<boost::any>& dataVec)				= 0;
	
	
protected:
	sqlit_db& m_db;
	string	m_tableCreateSql;
	string	m_tableClearSql;
};

