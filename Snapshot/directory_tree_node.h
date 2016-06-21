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
		template<typename T>
		class directory_tree_node
		{
		public:
			directory_tree_node(void)
			{
			}

			~directory_tree_node(void)
			{
			}

		public:
			static std::shared_ptr<directory_tree_node> create(std::shared_ptr<directory_tree_node> spParentNode, shared_ptr<xfile> spXfile)
			{
				std::shared_ptr<directory_tree_node> spNode(new directory_tree_node);
				spNode->m_spParentNode	= spParentNode;
				spNode->m_spXfile				=	spXfile;
				return spNode;
			}

		public:
			void set_xfile(shared_ptr<xfile> spXfile)
			{
				m_spXfile = spXfile;
			}
			shared_ptr<xfile> file()
			{
				return m_spXfile;
			}
			std::shared_ptr<directory_tree_node> parent()
			{
				return m_spParentNode;
			}

			void insert_sub_node(const T& key, std::shared_ptr<directory_tree_node> spNode)
			{
				m_subItems.insert(map<T, std::shared_ptr<directory_tree_node>>::value_type(key, spNode));
			}
			void erase_sub_node(const T& key)
			{
				m_subItems.erase(key);
			}

			map<T, std::shared_ptr<directory_tree_node>>& sub_items()
			{
				return m_subItems;
			}

			std::shared_ptr<directory_tree_node> find(const T& key)
			{
				map<T, std::shared_ptr<directory_tree_node>>::iterator it = m_subItems.find(key);
				if(it != m_subItems.end())
					return it->second;
				return NULL;
			}

			void reset_parent()
			{
				m_spParentNode.reset();
			}

			void reset_parent(std::shared_ptr<directory_tree_node> spNode)
			{
				m_spParentNode = spNode;
			}

		private:
			shared_ptr<xfile>											m_spXfile;
			map<T, std::shared_ptr<directory_tree_node>>		m_subItems;
			std::shared_ptr<directory_tree_node>							m_spParentNode;
		};
	}
}