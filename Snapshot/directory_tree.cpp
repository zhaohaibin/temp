#include <deque>
#include "directory_tree.h"
#include "xfile.h"

namespace kingfile
{
	namespace filesystem
	{
		directory_tree::directory_tree(void)
			: m_spRoot(new directory_tree::node())
		{
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot->set_xfile(spXfile);
		}

		directory_tree::~directory_tree(void)
		{
		}

		bool directory_tree::insert( shared_ptr<xfile> spXfile )
		{
			bool ret = false;
			shared_ptr<directory_tree::node> spParent = find_parent(spXfile);
			if(spParent)
			{
				shared_ptr<directory_tree::node> spNode = directory_tree::node::create(spParent, spXfile);
				spParent->insert_sub_node(spNode->file()->m_path, spNode);
				ret = true;
			}
			return false;
		}

		void directory_tree::erase( shared_ptr<xfile> spXfile )
		{
			shared_ptr<directory_tree::node> spParent = find_parent(spXfile);
			if(spParent)
			{
				_for_each(spParent, std::bind(&directory_tree::_unparent, this, std::placeholders::_1));
				spParent->erase_sub_node(spXfile->m_path);
			}
		}

		void directory_tree::for_each( std::function<void(shared_ptr<directory_tree::node>)> func )
		{
			deque<shared_ptr<directory_tree::node>>	dirNodeQueue;
			dirNodeQueue.push_back(m_spRoot);
			for(; dirNodeQueue.empty() == false;  )
			{
				shared_ptr<directory_tree::node> spDir = dirNodeQueue.front();
				dirNodeQueue.pop_front();
				map<string, std::shared_ptr<directory_tree::node>> subItems = spDir->sub_items();
				map<string, std::shared_ptr<directory_tree::node>>::iterator it = subItems.begin();
				for( ;  it != subItems.end(); ++it)
				{
					if(it->second->file()->m_type == XFILE_FOLDER)
						dirNodeQueue.push_back(it->second);
					func(it->second);
				}
			}
		}

		shared_ptr<directory_tree::node> directory_tree::find_parent( shared_ptr<xfile> spXfile )
		{
			string parentPaht = spXfile->m_path.substr(0, spXfile->m_path.rfind('/'));
			return _find(parentPaht);
		}

		shared_ptr<directory_tree::node> directory_tree::_find( const string& path )
		{
			vector<string> ancestorsPath;
			_path_split(path, '/', ancestorsPath);
			shared_ptr<directory_tree::node> spNode = m_spRoot;
			for(int i = 0; i < ancestorsPath.size(); ++i)
			{
				spNode = spNode->find(ancestorsPath[i]);
				if(!spNode)
					break;
			}
			return spNode;
		}

		void directory_tree::_for_each( 
			shared_ptr<directory_tree::node> spRoot, 
			std::function<void(shared_ptr<directory_tree::node>)> func )
		{
			if(spRoot->file()->is_directory() == false)
				return;

			deque<shared_ptr<directory_tree::node>>	dirNodeQueue;
			dirNodeQueue.push_back(spRoot);
			for(; dirNodeQueue.empty() == false;  )
			{
				shared_ptr<directory_tree::node> spDir = dirNodeQueue.front();
				dirNodeQueue.pop_front();
				map<string, std::shared_ptr<directory_tree::node>> subItems = spDir->sub_items();
				map<string, std::shared_ptr<directory_tree::node>>::iterator it = subItems.begin();
				for( ;  it != subItems.end(); ++it)
				{
					if(it->second->file()->m_type == XFILE_FOLDER)
						dirNodeQueue.push_back(it->second);
					func(it->second);
				}
			}
		}

		void directory_tree::_path_split( const string& path, char split, vector<string>& vec )
		{
			string splitPath;
			string::const_iterator it = path.begin();
			if(path.empty())
				return;
			for(; it != path.end(); ++it)
			{
				if(*it == split)
				{
					if(splitPath.length() > 0)
						vec.push_back(splitPath);
				}
				splitPath += *it;
			}
			vec.push_back(splitPath);
		}

		shared_ptr<directory_tree::node> directory_tree::find( const string& path )
		{
			return _find(path);
		}

		void directory_tree::clear()
		{
			for_each(std::bind(&directory_tree::_unparent,this, std::placeholders::_1));
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot.reset(new directory_tree::node());
			m_spRoot->set_xfile(spXfile);
		}

		void directory_tree::_unparent( shared_ptr<directory_tree::node> spNode )
		{
			spNode->reset_parent();
		}

		map<string, std::shared_ptr<directory_tree::node>> directory_tree::root_subitems()
		{
			return m_spRoot->sub_items();
		}
	}
}