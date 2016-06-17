#include "db_table.h"



db_table::db_table(sqlit_db& db)
	: m_db(db)
{
}


db_table::~db_table(void)
{
}

int db_table::initialize()
{
	return m_db.exec_sql(m_tableCreateSql.c_str());
}

void db_table::clear()
{
	m_db.exec_sql(m_tableClearSql.c_str());
}
