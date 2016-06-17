#ifndef SQL_DEFINE_H
#define SQL_DEFINE_H
/********************************************************************
	created:	2015/08/31
	created:	31:8:2015   18:20
	filename: 	D:\KSYUN\new_client\pclite\src\include\db\sql_define.h
	file path:	D:\KSYUN\new_client\pclite\src\include\db
	file base:	sql_define
	file ext:	h
	author:		zhaohaibin
	
	purpose:	定义sql语句
*********************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////file change message table相关sql语句/////////////////////////////

//向file change message table插入一条数据

#define	sql_create_message_tbl		"CREATE TABLE IF NOT EXISTS file_change_message(\
																					ID	INTEGER	PRIMARY KEY AUTOINCREMENT,\
																					P_XID	INTEGER,\
																					XID	INTEGER,\
																					SPACE_TYPE	INTEGER,\
																					NAME		VACHAR(260)       COLLATE NOCASE,\
																					OP_TIME	INTEGER,\
																					FILE_TYPE	INTEGER,\
																					OP_TYPE		INTEGER,\
																					CONTENT	VACHAR(260),\
																					OPERATOR   VACHAR(260),\
																					STATE		INTEGER)"

#define sql_insert_to_file_change_message		"insert into file_change_message(\
																				P_XID,\
																				XID,\
																				SPACE_TYPE,\
																				NAME,\
																				OP_TIME,\
																				FILE_TYPE,\
																				OP_TYPE,\
																				CONTENT,\
																				STATE) values(\
																				@P_XID,\
																				@XID,\
																				@SPACE_TYPE,\
																				@NAME,\
																				@OP_TIME,\
																				@FILE_TYPE,\
																				@OP_TYPE,\
																				@CONTENT,\
																				@STATE)"

//从file change message table中查询全部数据
#define sql_query_all_from_file_change_message	"select * from file_change_message"

//从file change message table中删除一条数据，基于ID
#define sql_del_one_message_base_id		"delete from file_change_message where ID = @ID"

//将file change message table清空
#define sql_clear_message_table		"delete from file_change_message"

//从 file change message table中查询最大的ID
#define sql_sel_max_id_message_tb			"select max(ID) from file_change_message"


///////////////////////////////////////////////////////////////////////history_taks表相关sql语句///////////////////////////////////////////
#define  sql_create_history_task			"CREATE TABLE IF NOT EXISTS history_task(\
																	id             INTEGER         PRIMARY KEY ,\
																	task_ctime     INTEGER						  ,\
																	file_id      INTEGER,\
																	parent_id    INTEGER,\
																	file_path    VACHAR(260)       COLLATE NOCASE,\
																	sha1         VACHAR(42)        COLLATE NOCASE,\
																	is_folder      INTEGER                       ,\
																	file_size      INTEGER                       ,\
																	cur_size       INTEGER                       ,\
																	error_code     INTEGER                       ,\
																	task_type      INTEGER                       ,\
																	file_version		INTEGER						,\
																	name				VACHAR(260)       COLLATE NOCASE,\
																	space_type		INTEGER)"

#define sql_insert_historyTask		"insert into history_task(task_ctime, file_id, parent_id, file_path, sha1, is_folder, file_size,\
															cur_size, error_code, task_type) \
															values(@task_ctime, @file_id, @parent_id, @file_path, @sha1, @is_folder, @file_size, \
															@cur_size, @error_code, @task_type)"

#define sql_sel_max_id_from_history_task		"select max(ID) from history_task"

#define sql_del_item_from_history_task	"delete from history_task where task_ctime=@task_ctime and file_path=@file_path"

#define  sql_upd_cur_size_in_history_task		"update history_task set cur_size=@cur_size where task_ctime=@task_ctime and file_path=@file_path"

#define  sql_upd_error_code_in_history_task		"update history_task set error_code=@error_code where task_ctime=@task_ctime and file_path=@file_path"

#define  sql_drop_history_task_table "DROP TABLE history_task"

#define sql_load_history_task_table             "select * from history_task order by task_ctime desc"

//////////////////////////////////////////////////////////////////db version 表相关sql 语句//////////////////////////////////////////////////
#define sql_create_db_version	"CREATE TABLE IF NOT EXISTS db_version_info( version		INTEGER)"
#define sql_get_version				"select version from db_version_info"
#define sql_insert_db_version	"insert into db_version_info(version)values(@version)"
#define sql_update_db_version	"update db_version_info set version=@version"

/////////////////////////////////////////////////////////////////upload_file表相关sql语句/////////////////////////////////////////////////////
//创建upload_file表
#define sql_create_upload_file	"CREATE TABLE IF NOT EXISTS upload_file (\
						TASKID INTEGER PRIMARY KEY NOT NULL,\
						C_TIME INTEGER, \
						PARENT_XID INTEGER, \
						LOCAL_PATH varchar(2048), \
						NAME	varchar(256),\
						SPACE_TYPE INTEGER,\
						FILE_TYPE INTEGER, \
						STATUS INTEGER, \
						FILE_SIZE INTEGER, \
						TRANSFER_SIZE INTEGER, \
						ERROR_CODE INTEGER, \
						REMOTE_PATH varchar(256), \
                        TASK_ROOT_LOCAL_PATH varchar(256))"

//select所有文件
#define select_all_upload_file "select * from upload_file"

//获取 最大的 id
#define  sel_max_id_upload_file "select max(TASKID) from upload_file"

#define sql_upload_file_table_add_field "ALTER TABLE upload_file ADD COLUMN TASK_ROOT_LOCAL_PATH varchar(256)"

/////////////////////////////////////////////////////////////////download_file表相关sql语句/////////////////////////////////////////////////////
//创建download_file表
#define sql_create_download_file_table	"CREATE TABLE IF NOT EXISTS download_file (\
                                TASKID INTEGER PRIMARY KEY NOT NULL, \
                                TASK_CTIME INTEGER, \
                                FILE_XID INTEGER,\
                                PARENT_XID INTEGER, \
                                FILE_VER INTEGER,\
                                LOCAL_PATH varchar(2048), \
                                NAME	varchar(256),\
                                SPACE_TYPE INTEGER,\
                                FILE_TYPE INTEGER, \
                                STATUS INTEGER, \
                                FILE_SIZE INTEGER, \
                                TRANSFER_SIZE INTEGER, \
                                ERROR_CODE INTEGER)"

//select所有文件
#define select_alltable_download_file "select * from download_file"

//获取 最大的 id
#define  sel_maxid_download_file "select max(TASKID) from download_file"


/////////////////////////////////////////////////////////////////complete_file表相关sql语句/////////////////////////////////////////////////////
//创建download_file表
#define sql_create_complete_file_table	"CREATE TABLE IF NOT EXISTS complete_file (\
                                    TASKID INTEGER PRIMARY KEY NOT NULL, \
                                    TASK_CTIME INTEGER, \
                                    FILE_XID INTEGER, \
                                    PARENT_XID INTEGER, \
                                    FILE_VER INTEGER,\
                                    FILE_SHA_FLAG VACHAR(42)   COLLATE NOCASE,\
                                    LOCAL_PATH varchar(2048), \
                                    NAME	varchar(256),\
                                    SPACE_TYPE INTEGER,\
                                    FILE_TYPE INTEGER, \
                                    STATUS INTEGER, \
                                    FILE_SIZE INTEGER, \
                                    TRANSFER_SIZE INTEGER, \
                                    ERROR_CODE INTEGER, \
									CTASK_TYPE INTEGER)"

//select所有文件
#define select_all_from_complete_file_table "select * from complete_file order by TASKID desc limit 500"

//获取 最大的 id
#define  sel_maxid_complete_file "select max(TASKID) from complete_file"

//创建user_config表
#define sql_create_userconfig__table	"CREATE TABLE IF NOT EXISTS user_config (\
										NAME varchar(256) PRIMARY KEY NOT NULL,VALUE varchar(1024))"

/////////////////////////////////////////////////////////////////upload_folder表相关sql语句/////////////////////////////////////////////////////
//创建upload_folder表
#define sql_create_upload_folder_table	"CREATE TABLE IF NOT EXISTS upload_folder (\
task_id INTEGER PRIMARY KEY NOT NULL, \
task_ctime INTEGER, \
root_xid INTEGER,\
parent_xid INTEGER, \
xid INTEGER,\
full_folder_path varchar(2048))"

//select 所有记录
#define select_all_from_upload_folder_table "select * from upload_folder"

//获取 最大的 id
#define  sel_maxid_from_upload_folder_table "select max(task_id) from upload_folder"


#define del_all_record_from_upload_folder_table "delete from upload_folder"



/////////////////////////////////////////////////////////////////upload_folder 枚举表相关sql语句/////////////////////////////////////////////////////
//创建upload_file_parse表
#define sql_create_upload_file_parse_table	"CREATE TABLE IF NOT EXISTS upload_file_parse (\
task_id INTEGER PRIMARY KEY NOT NULL, \
parent_xid INTEGER, \
space_type INTEGER,\
root_folder varchar(2048),\
full_item_path varchar(2048))"

//select 所有记录
#define select_all_from_upload_file_parse_table "select * from upload_file_parse"

//获取 最大的 id
#define  sel_maxid_from_upload_file_parse_table "select max(task_id) from upload_file_parse"


#define del_all_record_from_upload_file_parse_table "delete from upload_file_parse"

////////////////////////////////////download_folder_parse
#define sql_create_download_folder_parse_table	"CREATE TABLE IF NOT EXISTS download_folder_parse (\
					ID INTEGER PRIMARY KEY NOT NULL, \
					C_TIME INTEGER, \
					PXID INTEGER, \
					XID INTEGER,\
					ROOT_XID INTEGER, \
					NAME	varchar(256),\
					LOCAL_PATH varchar(2048), \
					SPACE_TYPE INTEGER,\
					ERROR_CODE INTEGER)"

#define sql_sel_max_id_dow_folder_parse_table			"select max(ID) from download_folder_parse"

#define sql_sel_all_download_folder_parse_table	"select * from download_folder_parse"

////////////////////////////////////download_folder
#define sql_create_download_folder_table	"CREATE TABLE IF NOT EXISTS download_folder (\
					XID INTEGER PRIMARY KEY NOT NULL, \
					C_TIME INTEGER, \
					PXID INTEGER, \
					NAME	varchar(256),\
					LOCAL_PATH varchar(2048), \
					SPACE_TYPE INTEGER,\
					STATUS INTEGER, \
					ERROR_CODE INTEGER)"
#define sql_sel_all_download_folder_table	"select * from download_folder"
#endif//SQL_DEFINE_H
