#pragma once
#include <set>
#include <deque>
#include <boost/filesystem.hpp>
#include "xfile.h"

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
	}
}

// class FileSystemHelper
// {
// protected:
// 	FileSystemHelper(void);
// public:
// 	~FileSystemHelper(void);
// 
// public:
// 	static xfile_shared_ptr fileInfo(const std::string& path);
// 	static bool directoryEnumSubItemsSet(const string& directory, vector<xfile_shared_ptr>& subItemsVec, std::string& strError);
// private:
// 	static bool directorySubItemsSet(const std::string& path, 
// 		vector<xfile_shared_ptr>& subItemsVec, 
// 		std::deque<xfile_shared_ptr>& subFolderQueue,
// 		std::string& strError);
// };

