#include "stdafx.h"
#include "../../XLiveShell/Editor/Editor.h"
#include "db_executor.h"
#include "content/klite_thread.h"
#include "dispatcher/core_dispatcher_mgrimpl.h"


class db_executor_callback
{
public:
	db_executor_callback(dispatch_key key)
		:m_key(key){}
	~db_executor_callback(){}

	void operator()(std::shared_ptr<Json::Value> p_val)
	{
		dispatcher::core_dispatcher_mgr::instance()->post_data(
			m_key, p_val);
	}
private:
	dispatch_key m_key;
};

db_executor::db_executor()
	:m_weak_ptr_factory_(this)
{
	dispatcher::core_dispatcher_mgr::instance()->resister_component(
		DB_EXECUTOR,std::bind(&db_executor::handle_dispatcher,this, std::placeholders::_1, std::placeholders::_2));
}

db_executor::~db_executor()
{

}

void db_executor::handle_dispatcher( int key, std::shared_ptr<Json::Value> p_value )
{
	if(key >DB_FILE_CHANGE_MESSAGE_TABLE_BEGIN && key < DB_FILE_CHANGE_MESSAGE_TABLE_END)
		handle_file_change_message_table(key, p_value);
	else if(key >DB_UPLOAD_FILE_TABLE_BEGIN && key < DB_UPLOAD_FILE_TABLE_END )
		handle_upload_file_table(key, p_value);
    else if(key >UP_FILE_PARSER_BEGIN && key < UP_FILE_PARSER_END )
        handle_upload_file_parse_table(key, p_value);
    else if (key >DB_DOWNLOAD_FILE_TABLE_BEGIN && key < DB_DOWNLOAD_FILE_TABLE_END)
        handle_download_file_table(key, p_value); 
	else if (key >DB_COMPLETE_FILE_TABLE_BEGIN && key < DB_COMPLETE_FILE_TABLE_END)
		handle_complete_file_table(key, p_value); 
	else if (key >DB_USER_CONFIG_TABLE_BEGIN && key < DB_USER_CONFIG_TABLE_END)
		handle_user_config_table(key, p_value); 
    else if (key > DB_UPLOAD_FOLDER_TABLE_BEGIN && key < DB_UPLOAD_FOLDER_TABLE_END)
        handle_upload_folder_table(key, p_value);
    else if(key > DB_DOWNLOAD_FOLDER_PARSE_TABLE_BEGIN && key < DB_DOWNLOAD_FOLDER_PARSE_TABLE_END)
		handle_download_folder_parse_table(key, p_value);
	else if(key > DB_DOWNLOAD_FOLDER_TABLE_BEGIN && key < DB_DOWNLOAD_FOLDER_TABLE_END)
		handle_download_folder_table(key, p_value);
        ;
}

void db_executor::handle_file_change_message_table( int key, std::shared_ptr<Json::Value> p_value )
{
	switch(key)
	{
	case DB_INSERT_TO_FILE_CHANGE_MESSAGE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::insert_to_file_change_message_table, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value,
				db_executor_callback(UI_NEW_FILE_CHANGE_MESSAGE)) );
			break;
		}
	case DB_QUERY_FROM_FILE_CHANGE_MESSAGE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::query_from_file_change_message_table, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value,
				db_executor_callback(UI_HISTORY_FILE_CHANGE_MESSAGES)) );
			break;
		}
	case DB_READ_ONE_FILE_CHANGE_MESSAGE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::delete_from_file_change_message_table, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
	case DB_CLEAR_FILE_CHANGE_MESSAGE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::clear_file_change_message_table, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
	default:
		break;
	}
}

void db_executor::insert_to_file_change_message_table( std::shared_ptr<Json::Value> p_value,
	db_executor_callback call_back)
{
	//p_value既是输入也是输出参数
	_Module.GetUser().GetDb().message_tbl_insert(p_value);
	call_back(p_value);
}

void db_executor::delete_from_file_change_message_table( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().message_tbl_delete(p_value);
}

void db_executor::query_from_file_change_message_table( 
	const std::shared_ptr<Json::Value> p_value, 
	db_executor_callback call_back)
{
	std::shared_ptr<Json::Value> p_result(new Json::Value());
	_Module.GetUser().GetDb().message_tbl_load(p_result);
	call_back(p_result);
}

void db_executor::clear_file_change_message_table( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().message_tbl_clear();
}

bool db_executor::query_upload_task_exist(std::shared_ptr<Json::Value> p_value)
{
    return _Module.GetUser().GetDb().upload_file_item_query(p_value);
}

void db_executor::remove_duplicated_upload_task(std::shared_ptr<Json::Value> p_value, std::shared_ptr<Json::Value> p_task_filtered)
{
    _Module.GetUser().GetDb().remove_duplicated_upload_file_item(p_value, p_task_filtered);
}

void db_executor::handle_upload_file_table( int key, std::shared_ptr<Json::Value> p_value )
{
	switch(key)
	{
	case DB_UPLOAD_FILE_TABLE_INSERT:
		{
            if ( p_value->size() > 0)
            {
               // OutputDebugStringA("@UploadFlow:db_executor::handle_upload_file_table---> notify DB thread to insert DB item begin!");
                KLiteThread::PostTask(KLiteThread::DB,
                    FROM_HERE,
                    base::Bind(
                    &db_executor::upload_file_tb_insert, 
                    m_weak_ptr_factory_.GetWeakPtr(), 
                    p_value/*p_task_filtered*/,
                    db_executor_callback(DB_UPLOAD_FILE_TABLE_NEW_ITEMS_INSERTED)) );
               // OutputDebugStringA("@UploadFlow:db_executor::handle_upload_file_table---> notify DB thread to insert DB item end!");
            }

			break;
		}
	case DB_UPLOAD_FILE_TABLE_DELETE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::upload_file_tb_del, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
	case DB_UPLOAD_FILE_TABLE_UPDATE_CUR_SIZE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::upload_file_tb_update_cur_size, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
	case  DB_UPLOAD_FILE_TABLE_UPDATE_ERROR_CODE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::upload_file_tb_update_error_code, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
	case  DB_UPLOAD_FILE_TABLE_UPDATE_STATUS_CODE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::upload_file_tb_update_status_code, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
	case DB_UPLOAD_FILE_TABLE_LOAD:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
					&db_executor::upload_file_tb_load,
					m_weak_ptr_factory_.GetWeakPtr(),
					p_value,
					db_executor_callback(UI_TASK_ADD_ITEM)));
			break;
		}
	case DB_UPLOAD_FILE_TABLE_CLEAR:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::upload_table_clear,
				m_weak_ptr_factory_.GetWeakPtr()));
			break;
		}
	case DB_UPLOAD_FILE_TABLE_READ_ITEMS:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::upload_file_tb_read_items,
				m_weak_ptr_factory_.GetWeakPtr(),
				p_value,
				db_executor_callback(UI_TASK_ADD_ITEM)));
			break;
		}
	default:
		{
			break;
		}
	}
}

void db_executor::upload_file_tb_insert(
	std::shared_ptr<Json::Value> p_value,
	db_executor_callback call_back )
{
//    char temp[1024];
//    ::memset(temp, 0, 1024);
//    sprintf(temp, "@UploadFlow:db_executor::upload_file_tb_insert begin! [threadID=%d]", ::GetCurrentThreadId());  
//    OutputDebugStringA(temp);
    // filter the duplicated upload file task.

	//验证任务是否已添加存在bug，向不使用
  //  std::shared_ptr<Json::Value> p_task(new Json::Value);      
    //remove_duplicated_upload_task(p_value, p_task);  
	//return;

    //if (p_task->size() > 0 )
    {
       // OutputDebugStringA("@UploadFlow:db_executor::upload_file_tb_insert begin!");
		std::shared_ptr<Json::Value> sp_items(new Json::Value());
		(*sp_items)["count"] = p_value->size();
	    _Module.GetUser().GetDb().upload_file_insert(/*p_value*/p_value);
        //////////////////////////////////////////////////////////////////////////
        _Module.GetUser().GetDb().upload_file_parse_insert(/*p_value*/p_value);
        //////////////////////////////////////////////////////////////////////////
	    call_back(sp_items);
      //  OutputDebugStringA("@UploadFlow:db_executor::upload_file_tb_insert end!");
    }
//    OutputDebugStringA("@UploadFlow:db_executor::upload_file_tb_insert end!");
}

void db_executor::upload_file_tb_del( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().upload_file_delete(p_value);
}

void db_executor::upload_file_tb_update_cur_size( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().upload_file_update_curl_size(p_value);
}

void db_executor::upload_file_tb_update_error_code( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().upload_file_update_error_code(p_value);
}
void db_executor::upload_file_tb_update_status_code(std::shared_ptr<Json::Value> p_value)
{
	_Module.GetUser().GetDb().upload_file_update_status_code(p_value);
}

void db_executor::upload_file_tb_load( std::shared_ptr<Json::Value> p_val , db_executor_callback call_back)
{
	std::shared_ptr<Json::Value> sp_re(new Json::Value());
	_Module.GetUser().GetDb().upload_file_load(p_val, sp_re);
	call_back(sp_re);
}

void db_executor::upload_file_tb_read_items( std::shared_ptr<Json::Value>sp_val, db_executor_callback call_back )
{
	std::shared_ptr<Json::Value>sp_re(new Json::Value());
	_Module.GetUser().GetDb().upload_file_read_items(sp_val, sp_re);
	call_back(sp_re);
}


//////////////////////////////////////////////////////////////////////////
bool db_executor::query_download_task_exist(std::shared_ptr<Json::Value> p_value)
{
    return _Module.GetUser().GetDb().download_file_item_query(p_value);
}

void db_executor::remove_duplicated_download_task(std::shared_ptr<Json::Value> p_value, std::shared_ptr<Json::Value> p_filter)
{
    _Module.GetUser().GetDb().remove_duplicated_download_file_item(p_value, p_filter);
}

void db_executor::handle_download_file_table(int key, std::shared_ptr<Json::Value> p_value)
{
    switch(key)
    {
    case DB_DOWNLOAD_FILE_TABLE_INSERT:
        {           
            if ( p_value->size() > 0 )
            {
                KLiteThread::PostTask(KLiteThread::DB,
                    FROM_HERE,
                    base::Bind(
                    &db_executor::download_file_tb_insert, 
                    m_weak_ptr_factory_.GetWeakPtr(), 
                    p_value,
                    db_executor_callback(DB_DOWNLOAD_FILE_TABLE_NEW_ITEMS_INSERTED)) );
            }               
            break;
        }
	case DB_DOWNLOAD_FILE_TABLE_READ_ITEMS:
		{
			std::shared_ptr<Json::Value> sp_re(new Json::Value());
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::download_file_tb_read_items,
				m_weak_ptr_factory_.GetWeakPtr(),
				p_value,
				sp_re,
				db_executor_callback(UI_TASK_ADD_DOWNLOAD_ITEM)));
			break;
		}
    case DB_DOWNLOAD_FILE_TABLE_DELETE:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::download_file_tb_del, 
                m_weak_ptr_factory_.GetWeakPtr(), 
                p_value) );
            break;
        }
    case DB_DOWNLOAD_FILE_TABLE_UPDATE_CUR_SIZE:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::download_file_tb_update_cur_size, 
                m_weak_ptr_factory_.GetWeakPtr(), 
                p_value) );
            break;
        }
    case  DB_DOWNLOAD_FILE_TABLE_UPDATE_ERROR_CODE:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::download_file_tb_update_error_code, 
                m_weak_ptr_factory_.GetWeakPtr(), 
                p_value) );
            break;
        }
	case  DB_DOWNLOAD_FILE_TABLE_UPDATE_STATUS_CODE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::download_file_tb_update_status_code, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
    case DB_DOWNLOAD_FILE_TABLE_LOAD:
        {
			std::shared_ptr<Json::Value> sp_re(new Json::Value());
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::download_file_tb_load,
                m_weak_ptr_factory_.GetWeakPtr(),
                p_value,
				sp_re,
                db_executor_callback(UI_TASK_ADD_DOWNLOAD_ITEM)));
            break;
        }
	case DB_DOWNLOAD_FILE_TABLE_CLEAR:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::download_table_clear,
				m_weak_ptr_factory_.GetWeakPtr()));
			break;
		}
    default:
        {
            break;
        }
    }
}


void db_executor::download_file_tb_insert(
    std::shared_ptr<Json::Value> p_value,
    db_executor_callback call_back )
{
    std::shared_ptr<Json::Value> p_task(new Json::Value);               
    //remove_duplicated_download_task(p_value, p_task);
    if (p_value->size() > 0 )
    {
		std::shared_ptr<Json::Value> sp_items(new Json::Value());
		(*sp_items)["count"] = p_value->size();
        _Module.GetUser().GetDb().download_file_insert(p_value);
        call_back(sp_items);
    }   
}

void db_executor::download_file_tb_del( std::shared_ptr<Json::Value> p_value )
{
    _Module.GetUser().GetDb().download_file_delete(p_value);
}

void db_executor::download_file_tb_update_cur_size( std::shared_ptr<Json::Value> p_value )
{
    _Module.GetUser().GetDb().download_file_update_curl_size(p_value);
}

void db_executor::download_file_tb_update_error_code( std::shared_ptr<Json::Value> p_value )
{
    _Module.GetUser().GetDb().download_file_update_error_code(p_value);
}
void db_executor::download_file_tb_update_status_code( std::shared_ptr<Json::Value> p_value )
{
	  _Module.GetUser().GetDb().download_file_update_status_code(p_value);
}

void db_executor::download_file_tb_load( std::shared_ptr<Json::Value> p_val , std::shared_ptr<Json::Value> sp_re , db_executor_callback call_back)
{
    _Module.GetUser().GetDb().download_file_load(p_val, sp_re);
    call_back(sp_re);
}

void db_executor::download_file_tb_read_items( std::shared_ptr<Json::Value>sp_val, std::shared_ptr<Json::Value>sp_re, db_executor_callback call_back )
{
	_Module.GetUser().GetDb().download_file_read_items(sp_val, sp_re);
	call_back(sp_re);
}

void db_executor::handle_complete_file_table(int key, std::shared_ptr<Json::Value> p_value)
{
	switch(key)
	{
	case DB_COMPLETE_FILE_TABLE_INSERT:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::complete_file_tb_insert, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );           
			break;
		}
	case DB_COMPLETE_FILE_TABLE_DELETE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::complete_file_tb_del, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value) );
			break;
		}
		break;
	case DB_COMPLETE_FILE_TABLE_LOAD:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::complete_file_tb_load,
				m_weak_ptr_factory_.GetWeakPtr(),
				p_value,
				db_executor_callback(UI_TASK_ADD_COMPLETE_ITEM)));
			break;
		}
		break;
	case DB_COMPLETE_FILE_TABLE_CLEAR:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::complete_table_clear,
				m_weak_ptr_factory_.GetWeakPtr()));
			break;
		}
		break;
	}
}
void db_executor::complete_file_tb_insert( std::shared_ptr<Json::Value> p_value)
{
//    char temp[1024];
//    ::memset(temp, 0, 1024);
//    sprintf(temp, "@UploadFlow:db_executor::complete_file_tb_insert begin! [threadID=%d]", ::GetCurrentThreadId());  
//    OutputDebugStringA(temp);

	_Module.GetUser().GetDb().complete_file_insert(p_value);

//   OutputDebugStringA("@UploadFlow:db_executor::complete_file_tb_insert end!");
}
void db_executor::complete_file_tb_del( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().complete_file_delete(p_value);
}
void db_executor::complete_file_tb_load( std::shared_ptr<Json::Value> p_val,db_executor_callback call_back)
{
	_Module.GetUser().GetDb().complete_file_load_all(p_val);
	call_back(p_val);
}
void db_executor::handle_user_config_table(int key, std::shared_ptr<Json::Value> p_value)
{
	switch(key)
	{
	case DB_USER_CONFIG_QUERY:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::handle_user_config_query, 
				m_weak_ptr_factory_.GetWeakPtr(), 
				p_value,db_executor_callback(UI_SHOW_MSG_COUNT)) );           
			break;
		}
	case DB_USER_CONFIG_UPDATE:
		{
			KLiteThread::PostTask(KLiteThread::DB,
				FROM_HERE,
				base::Bind(
				&db_executor::handle_user_config_update,
				m_weak_ptr_factory_.GetWeakPtr(),
				p_value));
			break;
		}
	default:
		break;
	}
}
void db_executor::handle_user_config_update( std::shared_ptr<Json::Value> p_value)
{
	_Module.GetUser().GetDb().updateconfig(p_value);
}
void db_executor::handle_user_config_query( std::shared_ptr<Json::Value> p_value ,db_executor_callback call_back)
{
	_Module.GetUser().GetDb().queryconfig(p_value);
	call_back(p_value);
}
// 这里仅仅只有DB的操作。
void db_executor::handle_upload_folder_table(int key, std::shared_ptr<Json::Value> p_value)
{
    switch(key)
    {
    case DB_UPLOAD_FOLDER_TABLE_INSERT:
        {  
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_folder_tb_insert, 
                m_weak_ptr_factory_.GetWeakPtr(), 
                p_value));
            break;
        }
    case DB_UPLOAD_FOLDER_TABLE_DELETE:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_folder_tb_del, 
                m_weak_ptr_factory_.GetWeakPtr(), 
                p_value) );
            break;
        }   
    case DB_UPLOAD_FOLDER_TABLE_LOAD:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_folder_tb_load,
                m_weak_ptr_factory_.GetWeakPtr(),
                p_value,
                db_executor_callback(UI_TASK_ADD_UPLOAD_FOLDER_ITEM)));
            break;
        }
    case DB_UPLOAD_FOLDER_TABLE_DELETE_ALL:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_folder_tb_delete_all,
                m_weak_ptr_factory_.GetWeakPtr()));
            break;
        }
    case DB_UPLOAD_FOLDER_TABLE_SYNC:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_folder_tb_sync,
                m_weak_ptr_factory_.GetWeakPtr(),
                p_value));
            break;
        }
    default:
        {
            break;
        }
    }
}

void db_executor::upload_folder_tb_insert(std::shared_ptr<Json::Value> p_value)
{   
    _Module.GetUser().GetDb().upload_folder_insert(p_value);
}

void db_executor::upload_folder_tb_del(std::shared_ptr<Json::Value> p_value)
{
    _Module.GetUser().GetDb().upload_folder_delete(p_value);
}

void db_executor::upload_folder_tb_load(std::shared_ptr<Json::Value> p_val, db_executor_callback call_back)
{
    _Module.GetUser().GetDb().upload_folder_load_all(p_val);
    call_back(p_val);
}

void db_executor::upload_folder_tb_delete_all()
{ 
    _Module.GetUser().GetDb().upload_folder_delete_all();
}


UInt64 db_executor::query_upload_file_tb_row_count()
{
    return _Module.GetUser().GetDb().query_upload_file_row_count();
}

void db_executor::upload_folder_tb_sync(std::shared_ptr<Json::Value> p_val)
{
    // here，need check whether the info is take role for the left task in upload_file table. if there is one item in the upload file table, keep it.
    if (query_upload_file_tb_row_count() > 0)
    {
        return;// 暂不支持此功能。保留简单逻辑：只要upload_file table中有数据（即有任务，则不更新该table。）
    }
    // sync begin.
    upload_folder_tb_delete_all();
    // 2. fill in the server folder info.
}

void db_executor::handle_download_folder_parse_table( int key, std::shared_ptr<Json::Value> p_value )
{
	switch(key)
	{
	case DB_DOWNLOAD_FOLDER_PARSE_TABLE_INSERT:
		KLiteThread::PostTask(KLiteThread::DB,
			FROM_HERE,
			base::Bind(
			&db_executor::download_folder_parse_tb_insert, 
			m_weak_ptr_factory_.GetWeakPtr(), p_value, db_executor_callback(DOW_FOLDER_PARSER_FOLDERS) ) );
		break;
	case DB_DOWNLOAD_FOLDER_PARSE_TABLE_DELETE:
		KLiteThread::PostTask(KLiteThread::DB,
			FROM_HERE,
			base::Bind(
			&db_executor::download_folder_parse_tb_delete, 
			m_weak_ptr_factory_.GetWeakPtr(), p_value));
		break;
	case DB_DOWNLOAD_FOLDER_PARSE_TABLE_LOAD:
		KLiteThread::PostTask(KLiteThread::DB,
			FROM_HERE,
			base::Bind(&db_executor::download_folder_parse_tb_load,
			m_weak_ptr_factory_.GetWeakPtr(), p_value, db_executor_callback(DOW_FOLDER_PARSE_LOAD_COMPLETE)));
	default:
		break;
	}
}

void db_executor::download_folder_parse_tb_insert( std::shared_ptr<Json::Value> p_value, db_executor_callback call_back )
{
	_Module.GetUser().GetDb().download_folder_parse_insert(p_value);
	call_back(p_value);
}

void db_executor::download_folder_parse_tb_delete( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().download_folder_parse_delete(p_value);
}

void db_executor::download_folder_parse_tb_load( std::shared_ptr<Json::Value> p_value, db_executor_callback call_back )
{
	_Module.GetUser().GetDb().download_folder_parse_load(p_value);
	call_back(p_value);
}


void db_executor::handle_download_folder_table( int key, std::shared_ptr<Json::Value> p_value )
{
	switch(key)
	{
	case DB_DOWNLOAD_FOLDER_TABLE_INSERT:
		KLiteThread::PostTask(KLiteThread::DB,
			FROM_HERE,
			base::Bind(
			&db_executor::download_folder_tb_insert, 
			m_weak_ptr_factory_.GetWeakPtr(), p_value, db_executor_callback(DOW_FOLDER_FOLDERS) ) );
		break;
	case DB_DOWNLOAD_FOLDER_TABLE_DELETE:
		KLiteThread::PostTask(KLiteThread::DB,
			FROM_HERE,
			base::Bind(
			&db_executor::download_folder_tb_delete, 
			m_weak_ptr_factory_.GetWeakPtr(), p_value) );
		break;
	case DB_DOWNLOAD_FOLDER_TABLE_LOAD:
		KLiteThread::PostTask(KLiteThread::DB,
			FROM_HERE,
			base::Bind(&db_executor::download_folder_tb_load,
			m_weak_ptr_factory_.GetWeakPtr(), p_value, db_executor_callback(DOW_FOLDER_LOAD_COMPLETE)));
	default:
		break;
	}
}

void db_executor::download_folder_tb_insert( std::shared_ptr<Json::Value> p_value, db_executor_callback call_back )
{
	_Module.GetUser().GetDb().download_folder_insert(p_value);
	call_back(p_value);
}

void db_executor::download_folder_tb_delete( std::shared_ptr<Json::Value> p_value )
{
	_Module.GetUser().GetDb().download_folder_delete(p_value);
}
void db_executor::download_table_clear()
{
	_Module.GetUser().GetDb().download_file_tb_clear();
}
void db_executor::upload_table_clear()
{
	_Module.GetUser().GetDb().upload_table_clear();
    _Module.GetUser().GetDb().upload_file_parse_table_clear();
    _Module.GetUser().GetDb().upload_folder_delete_all();

}
void db_executor::complete_table_clear()
{
	_Module.GetUser().GetDb().complete_file_tb_clear();
}
void db_executor::download_folder_tb_load(std::shared_ptr<Json::Value> p_value, db_executor_callback call_back)
{
	_Module.GetUser().GetDb().download_folder_load(p_value);
	call_back(p_value);
}

//////////////////////////////////////////////////////////////////////////
void db_executor::handle_upload_file_parse_table( int key, std::shared_ptr<Json::Value> p_value )
{
    switch(key)
    {     
    case DB_UP_FILE_PARSER_LOAD_ALL:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_file_parse_tb_load,
                m_weak_ptr_factory_.GetWeakPtr(),
                p_value,
                db_executor_callback(DB_TASK_LOAD_UPLOAD_FILE_PARSE_ITEM)));
            break;
        }
    case DB_UP_FILE_PARSER_CLEAR:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_file_parse_table_clear,
                m_weak_ptr_factory_.GetWeakPtr()));
            break;
        } 
    case DB_UP_FILE_PARSER_ITEM_DELETE:
        {
            KLiteThread::PostTask(KLiteThread::DB,
                FROM_HERE,
                base::Bind(
                &db_executor::upload_file_parse_tb_del, 
                m_weak_ptr_factory_.GetWeakPtr(), 
                p_value) );
            break;
        }
    default:
        {
            break;
        }
    }
}

void db_executor::upload_file_parse_tb_load(std::shared_ptr<Json::Value> p_val, db_executor_callback call_back)
{
    _Module.GetUser().GetDb().upload_file_parse_load_all(p_val);
    call_back(p_val);   
}

// 删除对应信息的条目。
void db_executor::upload_file_parse_tb_del(std::shared_ptr<Json::Value> p_val)
{
    _Module.GetUser().GetDb().upload_file_parse_table_delete(p_val);
}

// 清除所有数据
void db_executor::upload_file_parse_table_clear()
{
    _Module.GetUser().GetDb().upload_file_parse_table_clear();   
}


