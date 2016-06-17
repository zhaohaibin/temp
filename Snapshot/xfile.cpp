#include "xfile.h"
#include <boost/filesystem.hpp>

namespace kingfile
{
	namespace filesystem
	{
		xfile::xfile()
			: m_path("")
			, m_name("")
			, m_mtime(0)
			, m_size(0)
			, m_version(0)
			, m_opVer(0)
			, m_xid(0)
			, m_pxid(0)
			, m_type(XFILE_FOLDER)
		{
		}

		xfile::~xfile()
		{
		}

		shared_ptr<xfile> xfile::create( const string& path )
		{
			shared_ptr<xfile> spXfile(new xfile());
			try
			{
				bool isDir = boost::filesystem::is_directory(path);
				if(isDir)
				{
					spXfile->m_type = XFILE_FOLDER;
				}else
				{
					spXfile->m_type = XFILE_FILE;
					spXfile->m_size  = boost::filesystem::file_size(path);

				}
				spXfile->m_path = path;
				int pos = path.rfind('\\');
				spXfile->m_name = path.substr(pos+1,path.size()-pos-1);
				spXfile->m_mtime = boost::filesystem::last_write_time(path);
			}catch(boost::filesystem::filesystem_error error)
			{
				spXfile.reset();
			}
			return spXfile;
		}

		bool xfile::is_directory()
		{
			return m_type == XFILE_FOLDER;
		}
	}
}