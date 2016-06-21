#include "directory_tree_node.h"
#include "xfile.h"

namespace kingfile
{
	namespace filesystem
	{
// 		directory_tree_node::directory_tree_node(void)
// 		{
// 		}
// 
// 
// 		directory_tree_node::~directory_tree_node(void)
// 		{
// 
// 		}
// 
// 		std::shared_ptr<directory_tree_node> directory_tree_node::create( std::shared_ptr<directory_tree_node> spParentNode, shared_ptr<xfile> spXfile )
// 		{
// 			std::shared_ptr<directory_tree_node> spNode(new directory_tree_node);
// 			spNode->m_spParentNode	= spParentNode;
// 			spNode->m_spXfile				=	spXfile;
// 			return spNode;
// 		}
// 
// 		void directory_tree_node::set_xfile( shared_ptr<xfile> spXfile )
// 		{
// 			m_spXfile = spXfile;
// 		}
// 
// 		shared_ptr<xfile> directory_tree_node::file()
// 		{
// 			return m_spXfile;
// 		}
// 
// 		void directory_tree_node::insert_sub_node( std::shared_ptr<directory_tree_node> spNode )
// 		{
// 			m_subItems.insert(map<string, std::shared_ptr<directory_tree_node>>::value_type(spNode->file()->m_path, spNode));
// 		}
// 
// 		std::shared_ptr<directory_tree_node> directory_tree_node::find( const T& path )
// 		{
// 			map<string, std::shared_ptr<directory_tree_node>>::iterator it = m_subItems.find(path);
// 			if(it != m_subItems.end())
// 				return it->second;
// 			return NULL;
// 		}
// 
// 		void directory_tree_node::erase_sub_node( const T& key )
// 		{
// 			m_subItems.erase(path);
// 		}
// 
// 		map<string, std::shared_ptr<directory_tree_node>>& directory_tree_node::sub_items()
// 		{
// 			return m_subItems;
// 		}
// 
// 		std::shared_ptr<directory_tree_node> directory_tree_node::parent()
// 		{
// 			return m_spParentNode;
// 		}
// 
// 		void directory_tree_node::reset_parent()
// 		{
// 			m_spParentNode.reset();
// 		}

	}
}

