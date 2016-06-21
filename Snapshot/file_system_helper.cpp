#include "file_system_helper.h"

namespace kingfile
{
	namespace filesystem
	{
		xfile_shared_ptr file_info( const std::string& path )
		{
			xfile_shared_ptr spXfile = create_xfile<xfile>(path);
			return spXfile;
		}

		bool list_directory( const std::string& path, 
			vector<xfile_shared_ptr>& subItemsVec,
			std::deque<xfile_shared_ptr>& subFolderQueue, 
			std::string& strError )
		{
			bool ret = false;
			try
			{
				ret = boost::filesystem::exists(path);
				if(ret == false)
				{
					strError = path + "不是一个合法的路径";
					return ret;
				}

				ret = boost::filesystem::is_directory(path);
				if(ret == false)
				{
					strError = path + "不是一个目录";
					return ret;
				}

				boost::filesystem::directory_iterator dir_itr = boost::filesystem::directory_iterator(path);
				boost::filesystem::directory_iterator end_iter;
				for ( ; dir_itr != end_iter; ++dir_itr)
				{
					xfile_shared_ptr spXfile = file_info(dir_itr->path().string());
					if(spXfile->is_directory())
						subFolderQueue.push_back(spXfile);
					subItemsVec.push_back(spXfile);
				}
			}catch(boost::filesystem::filesystem_error error)
			{
				strError = error.what();
				ret = false;
			}
			return ret;
		}

		bool enum_directory( const string& directory, vector<xfile_shared_ptr>& subItemsVec, std::string& strError )
		{
			bool ret = false;
			std::deque<xfile_shared_ptr> subFolderQueue;
			ret = list_directory(directory, subItemsVec, subFolderQueue, strError);
			if(ret)
			{
				for( ;subFolderQueue.empty() == false ;)
				{
					xfile_shared_ptr spDir = subFolderQueue.front();
					ret = list_directory(spDir->m_path, subItemsVec, subFolderQueue, strError);
					if(ret == false)
						return ret;
					subFolderQueue.pop_front();
				}
			}
			return ret;
		}

		bool make_dirs( const std::string& path, std::string& strError )
		{
			bool ret;
			try
			{
				ret = boost::filesystem::create_directories(path);
			}catch(boost::filesystem::filesystem_error error)
			{
				strError = error.what();
			}
			return ret;
		}

	}
}