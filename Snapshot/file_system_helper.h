#pragma once
#include <set>
#include <deque>
#include "xfile.h"
#include <boost/filesystem.hpp>

namespace kingfile
{
	namespace filesystem
	{
		xfile_shared_ptr file_info(const std::string& path);

		bool list_directory(const std::string& path, 
			vector<xfile_shared_ptr>& subItemsVec, 
			std::deque<xfile_shared_ptr>& subFolderQueue,
			std::string& strError);

		bool enum_directory(const string& directory, vector<xfile_shared_ptr>& subItemsVec, std::string& strError);

		bool make_dirs(const std::string& path, std::string& strError);
	}
}