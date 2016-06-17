#pragma once
#include <map>
#include <string>
#include <memory>
#include <cstdint>
#include "xfile.h"
using namespace std;

namespace kingfile
{
	namespace filesystem
	{
		class directory_tree_node
		{
		public:
			directory_tree_node(void);
			~directory_tree_node(void);

		public:
			static std::shared_ptr<directory_tree_node> create(std::shared_ptr<directory_tree_node> spParentNode, shared_ptr<xfile> spXfile);

		public:
			void set_xfile(shared_ptr<xfile> spXfile);
			shared_ptr<xfile> file();
			std::shared_ptr<directory_tree_node> parent();

			void insert_sub_node(std::shared_ptr<directory_tree_node> spNode);
			void erase_sub_node(const string& path);
			map<string, std::shared_ptr<directory_tree_node>>& sub_items();

			std::shared_ptr<directory_tree_node> find(const string& path);

			void clear()
			{
// 				m_spParentNode.reset();
// 				map<string, std::shared_ptr<directory_tree_node>>::iterator it = m_subItems.begin();
// 				for(; it != m_subItems.end(); ++it)
// 				{
// 					it->second->clear();
// 				}
// 				m_subItems.clear();
			}
			void reset_parent()
			{
				m_spParentNode.reset();
			}
		private:
			shared_ptr<xfile>											m_spXfile;
			map<string, std::shared_ptr<directory_tree_node>>		m_subItems;
			std::shared_ptr<directory_tree_node>							m_spParentNode;
		};
	}
}