#pragma once
#include <vector>
#include <functional>
#include "directory_tree_node.h"

namespace kingfile
{
	namespace filesystem
	{
		class directory_tree
		{
		public:
			directory_tree(void);
			~directory_tree(void);

		public:
			void insert(shared_ptr<xfile> spXfile);
			void erase(shared_ptr<xfile> spXfile);
			void clear();
			void for_each(std::function<void(shared_ptr<directory_tree_node>)> func);
			shared_ptr<directory_tree_node> find_parent(shared_ptr<xfile> spXfile);
			shared_ptr<directory_tree_node> find(const string& path);
		private:
			shared_ptr<directory_tree_node> _find(const string& path);
			void _for_each(shared_ptr<directory_tree_node> spRoot, std::function<void(shared_ptr<directory_tree_node>)> func);

			void _path_split(const string& path, char split, vector<string>& vec);
			void _unparent(shared_ptr<directory_tree_node> spNode);
		private:
			shared_ptr<directory_tree_node>		m_spRoot;
		};
	}
}