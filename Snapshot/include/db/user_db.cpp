#include "user_db.h"

#include <fstream>
#include <cassert>
#include <stdint.h>

#include "../db_snapshot_table.h"

int user_db::initializeDb( const std::string& db_path )
{
	int ret = m_db.initialize_db(db_path);
	ret = initializeTable();
	return ret;
}

int user_db::initializeTable()
{
	shared_ptr<db_table> spSnapshot(new db_snapshot_table(m_db));
	int ret = spSnapshot->initialize();
	if(ret == SQLITE_OK)
		m_tables.insert(map<DB_TABLE, shared_ptr<db_table>>::value_type(TABLE_SNAPSHOT, spSnapshot));

	return ret;
}

shared_ptr<db_table> user_db::getTable( DB_TABLE table )
{
	map<DB_TABLE, shared_ptr<db_table>>::iterator it = m_tables.find(table);
	if( it != m_tables.end())
		return it->second;
	return NULL;
}
