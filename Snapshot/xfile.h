#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include "JsonCpp/json.h"
#include "role/role.h"

using namespace std;

namespace kingfile
{
	namespace filesystem
	{
		enum XFILE_TYPE
		{
			XFILE_FOLDER,
			XFILE_FILE
		};

		enum SPACE_TYPE
		{
			SPACE_PRIVATE,
			SPACE_PUBLIC
		};

		class xfile
		{
		public:
			xfile();
			~xfile();

		public:
			static shared_ptr<xfile> create(const string& path);

			bool is_directory();
			void to_json(Json::Value& jobj);
		public:
			string			m_name;
			string			m_path;
			uint64_t		m_size;
			uint64_t		m_mtime;
			uint64_t		m_version;
			uint64_t		m_opVer;
			uint64_t		m_xid;
			uint64_t		m_pxid;
			XFILE_TYPE	m_type;
			string			m_sha1;
			file_role::EKP_FILE_ROLE m_role;
			SPACE_TYPE m_space_type;
		};

		typedef  shared_ptr<xfile> xfile_shared_ptr;

		template<typename T>
		xfile_shared_ptr create_xfile(const string& path)
		{
			return T::create(path);
		}
	}
}

