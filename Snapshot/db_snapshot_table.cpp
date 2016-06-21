#include "db_snapshot_table.h"

#define tabCreateSql "CREATE TABLE IF NOT EXISTS snapshot ( local_path VARCHAR (260) PRIMARY KEY, space_type INT, name VARCHAR (260), type INT, role INT, size INTEGER, xid INTEGER, pxid INTEGER, version INTEGER, sha1 VARCHAR (42), mtime DATETIME, opver INTEGER)"
#define tableClearSql "delete from snapshot"
#define tableInsertSql "insert into snapshot ( local_path, space_type, name, type, role, size, xid, pxid, version, sha1, mtime, opver) values ('%s', %d, '%s', %d, %d, %llu, %llu, %llu, %llu, '%s', %llu, %llu)"
db_snapshot_table::db_snapshot_table(sqlit_db& db)
	: db_table(db)
{
	m_tableCreateSql = tabCreateSql;
	m_tableClearSql   = tableClearSql;
}


db_snapshot_table::~db_snapshot_table(void)
{
}

void db_snapshot_table::insert( const vector<boost::any>& dataVec )
{
	int ret = m_db.begin_transaction();
	for(int i = 0; i < dataVec.size() ; ++i )
	{
		xfile_shared_ptr spXfile = boost::any_cast<xfile_shared_ptr>(dataVec[i]);
		string sql = _insert_sql(spXfile);
		ret = m_db.exec_sql(sql);
	}
	ret = m_db.commit_transaction();
}

int db_snapshot_table::read_all( vector<boost::any>& dataVec )
{
	string sql = "select * from snapshot";
	int ret = _read(sql, dataVec);
	return ret;
}


void db_snapshot_table::read( vector<boost::any>& dataVec )
{
	
}

std::string db_snapshot_table::_insert_sql( xfile_shared_ptr spXfile )
{
	char buffer[2048];
	sprintf(buffer, tableInsertSql,
		spXfile->m_path.c_str(), 
		spXfile->m_space_type,
		spXfile->m_name.c_str(), 
		spXfile->m_type,
		spXfile->m_role,
		spXfile->m_size,
		spXfile->m_xid,
		spXfile->m_pxid,
		spXfile->m_version,                 
		spXfile->m_sha1.c_str(),
		spXfile->m_mtime,
		spXfile->m_opVer);
	return buffer;
}

int db_snapshot_table::_read( const string sql, vector<boost::any>& dataVec )
{
	int ret = m_db.exec_query(sql);
	if(ret == SQLITE_OK)
	{
		xfile_shared_ptr spXfile(new xfile());
		while( m_db.fetch_row(
			spXfile->m_path,
			spXfile->m_space_type,
			spXfile->m_name,
			(int&)spXfile->m_type, 
			spXfile->m_role,
			spXfile->m_size,
			spXfile->m_xid,
			spXfile->m_pxid,
			spXfile->m_version,
			spXfile->m_sha1,
			spXfile->m_mtime,
			spXfile->m_opVer
			) == SQLITE_ROW)
		{
			dataVec.push_back(spXfile);
			spXfile.reset(new xfile());
		}
	}
	m_db.release();
	return ret;
}