#include "directory_helper.h"

namespace kingfile
{
	namespace filesystem
	{
		namespace directory_helper
		{
			shared_ptr<directory_tree> conv_to_directory_tree( shared_ptr<server_directory_tree> spTree )
			{
				shared_ptr<directory_tree> spDirectoryTree(new directory_tree());
				spTree->for_each(std::bind(_insert_to_directory_tree, std::placeholders::_1, spDirectoryTree));
				return spDirectoryTree;
			}

			void _insert_to_directory_tree( shared_ptr<server_directory_tree::node> spNode, shared_ptr<directory_tree> spTree )
			{
				spTree->insert(spNode->file());
			}
		}
	}
}
