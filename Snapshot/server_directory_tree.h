#pragma once

#include<vector>
#include<map>
#include <functional>
#include"xfile.h"
#include "directory_tree_node.h"

namespace kingfile
{
	namespace filesystem
	{
		class server_directory_tree
		{
		public:
			typedef directory_tree_node<uint64_t> node;
		public:
			server_directory_tree(void);
			~server_directory_tree(void);

			bool init(map<uint64_t, xfile_shared_ptr>& xfiles);
			void for_each(std::function<void(shared_ptr<server_directory_tree::node>)> func);
			void clear();
		private:
			bool _extract_path(xfile_shared_ptr spXfile, map<uint64_t, xfile_shared_ptr>& xfiles, vector<xfile_shared_ptr>& xfilePath);
			bool _insert(vector<xfile_shared_ptr>& xfilePath);
			void _full_path(vector<xfile_shared_ptr>& xfilePath);
			void _unparent(shared_ptr<node> spNode);
		private:
			shared_ptr<server_directory_tree::node> m_spRoot;
		};
	}
}