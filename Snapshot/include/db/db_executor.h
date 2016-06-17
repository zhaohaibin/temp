#ifndef DB_EXECUTOR_H
#define DB_EXECUTOR_H
/********************************************************************
	created:	2015/08/31
	created:	31:8:2015   17:42
	filename: 	D:\KSYUN\new_client\pclite\src\include\db\db_executor.h
	file path:	D:\KSYUN\new_client\pclite\src\include\db
	file base:	db_executor
	file ext:	h
	author:		zhaohaibin
	
	purpose:	
*********************************************************************/
#include "JsonCpp/json.h"
#include "base/memory/weak_ptr.h"
#include <memory>


//db_executor用于执行数据库操作

class db_executor_callback;

class db_executor
{
public:
	db_executor();
	~db_executor();

protected:
	void handle_dispatcher(int key, std::shared_ptr<Json::Value> p_value);

private:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////file change message table处理函数///////////////////////////////////
	//处理file change message table
	void handle_file_change_message_table(int key, std::shared_ptr<Json::Value> p_value);

	void insert_to_file_change_message_table(
		std::shared_ptr<Json::Value> p_value, 
		db_executor_callback call_back);

	void delete_from_file_change_message_table(std::shared_ptr<Json::Value> p_value);

	void clear_file_change_message_table(std::shared_ptr<Json::Value> p_value);

	void query_from_file_change_message_table(
		const std::shared_ptr<Json::Value> p_value,
		db_executor_callback call_back);

	/////////////////////////////////////history task表处理函数//////////////////////////////////////////////////////
	void handle_upload_file_table(int key, std::shared_ptr<Json::Value> p_value);
    bool query_upload_task_exist(std::shared_ptr<Json::Value> p_value);
    void remove_duplicated_upload_task(std::shared_ptr<Json::Value> p_value, std::shared_ptr<Json::Value> p_task_filtered);
	void upload_file_tb_insert(std::shared_ptr<Json::Value> p_value, db_executor_callback call_back);
	void upload_file_tb_del(std::shared_ptr<Json::Value> p_value);
	void upload_file_tb_update_cur_size(std::shared_ptr<Json::Value> p_value);
	void upload_file_tb_update_error_code(std::shared_ptr<Json::Value> p_value);
	void upload_file_tb_update_status_code(std::shared_ptr<Json::Value> p_value);
	void upload_file_tb_load(std::shared_ptr<Json::Value> p_val, db_executor_callback call_back);
	void upload_file_tb_read_items(std::shared_ptr<Json::Value>sp_val, db_executor_callback call_back);
    UInt64 query_upload_file_tb_row_count();


    //////////////////////////////////////////////////////////////////////////
    // upload file parse table 
    void upload_file_parse_tb_load(std::shared_ptr<Json::Value> p_val, db_executor_callback call_back);
    void handle_upload_file_parse_table( int key, std::shared_ptr<Json::Value> p_value );
    void upload_file_parse_table_clear();
    void upload_file_parse_tb_del(std::shared_ptr<Json::Value> p_val);

    //////////////////////////////////////////////////////////////////////////
    void handle_upload_folder_table(int key, std::shared_ptr<Json::Value> p_value);   
    void upload_folder_tb_insert(std::shared_ptr<Json::Value> p_value);
    void upload_folder_tb_del(std::shared_ptr<Json::Value> p_value);   
    void upload_folder_tb_load(std::shared_ptr<Json::Value> p_val, db_executor_callback call_back);// 启动时，加载到cache内存。
    void upload_folder_tb_sync(std::shared_ptr<Json::Value> p_val);
    void upload_folder_tb_delete_all();

    //////////////////////////////////////////////////////////////////////////
    void handle_download_file_table(int key, std::shared_ptr<Json::Value> p_value);
    bool query_download_task_exist(std::shared_ptr<Json::Value> p_value);
     void remove_duplicated_download_task(std::shared_ptr<Json::Value> p_value, std::shared_ptr<Json::Value> p_filter);
    void download_file_tb_insert( std::shared_ptr<Json::Value> p_value, db_executor_callback call_back );
	void download_file_tb_read_items(std::shared_ptr<Json::Value>sp_val, std::shared_ptr<Json::Value>sp_re, db_executor_callback call_back);
    void download_file_tb_del( std::shared_ptr<Json::Value> p_value );
    void download_file_tb_update_cur_size( std::shared_ptr<Json::Value> p_value );
    void download_file_tb_update_error_code( std::shared_ptr<Json::Value> p_value );
	void download_file_tb_update_status_code( std::shared_ptr<Json::Value> p_value );
    void download_file_tb_load( std::shared_ptr<Json::Value> p_val , std::shared_ptr<Json::Value> sp_re, db_executor_callback call_back);

	void handle_complete_file_table(int key, std::shared_ptr<Json::Value> p_value);
	void complete_file_tb_insert( std::shared_ptr<Json::Value> p_value);
	void complete_file_tb_del( std::shared_ptr<Json::Value> p_value );
	void complete_file_tb_load( std::shared_ptr<Json::Value> p_val, db_executor_callback call_back);
   
	void handle_user_config_table(int key, std::shared_ptr<Json::Value> p_value);
	void handle_user_config_update( std::shared_ptr<Json::Value> p_value);
	void handle_user_config_query( std::shared_ptr<Json::Value> p_value,db_executor_callback call_back );

	/////////////////////////////////////////////////////////////////////////////////////////////////
	void handle_download_folder_parse_table(int key, std::shared_ptr<Json::Value> p_value);
	void download_folder_parse_tb_insert( std::shared_ptr<Json::Value> p_value, db_executor_callback call_back);
	void download_folder_parse_tb_delete(std::shared_ptr<Json::Value> p_value);
	void download_folder_parse_tb_load(std::shared_ptr<Json::Value> p_value, db_executor_callback call_back);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	void handle_download_folder_table(int key, std::shared_ptr<Json::Value> p_value);
	void download_folder_tb_insert(std::shared_ptr<Json::Value> p_value, db_executor_callback call_back);
	void download_folder_tb_delete(std::shared_ptr<Json::Value> p_value);
	void download_table_clear();
	void upload_table_clear();
	void complete_table_clear();
	void download_folder_tb_load(std::shared_ptr<Json::Value> p_value, db_executor_callback call_back);
   


private:
	base::WeakPtrFactory<db_executor> m_weak_ptr_factory_;
};
#endif

