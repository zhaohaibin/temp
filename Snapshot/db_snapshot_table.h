#pragma once
#include "db_table.h"
#include "xfile.h"
using namespace kingfile::filesystem;

class db_snapshot_table : public db_table 
{
public:
	db_snapshot_table(sqlit_db& db);
	~db_snapshot_table(void);

public:
	virtual void insert(const vector<boost::any>& dataVec);
	virtual int read_all(vector<boost::any>& dataVec);
	virtual void read(vector<boost::any>& dataVec);

private:
	string _insert_sql(xfile_shared_ptr spXfile);
	int _read(const string sql, vector<boost::any>& dataVec);
};

