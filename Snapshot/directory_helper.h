#pragma once

#include "directory_tree.h"
#include "server_directory_tree.h"

namespace kingfile
{
	namespace filesystem
	{
		namespace directory_helper
		{
			shared_ptr<directory_tree> conv_to_directory_tree(shared_ptr<server_directory_tree> spTree);

			void _insert_to_directory_tree(shared_ptr<server_directory_tree::node> spNode, shared_ptr<directory_tree> spTree);
		}
	}
}
