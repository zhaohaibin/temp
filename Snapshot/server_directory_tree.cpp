#include <deque>
#include "server_directory_tree.h"

namespace kingfile
{
	namespace filesystem
	{
		server_directory_tree::server_directory_tree(void)
			: m_spRoot(new server_directory_tree::node())
		{
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot->set_xfile(spXfile);
		}

		server_directory_tree::~server_directory_tree(void)
		{
		}

		bool server_directory_tree::init( map<uint64_t, xfile_shared_ptr>& xfiles )
		{
			bool ret = true;
			map<uint64_t, xfile_shared_ptr>::iterator it = xfiles.begin();
			for( ; it != xfiles.end(); ++it)
			{
				vector<xfile_shared_ptr> xfilePath;
				ret = _extract_path(it->second, xfiles, xfilePath);
				_full_path(xfilePath);
				if(ret)
				{
					ret = _insert(xfilePath);
					if(ret == false)
						break;
				}
			}
			return ret;
		}

		bool server_directory_tree::_extract_path( xfile_shared_ptr spXfile, map<uint64_t, xfile_shared_ptr>& xfiles, vector<xfile_shared_ptr>& xfilePath )
		{
			bool ret = true;

			xfile_shared_ptr spLastXfile = spXfile;
			for(;;)
			{
				xfilePath.push_back(spLastXfile);
				uint64_t pxid = spLastXfile->m_pxid;
				if(pxid == 0)
					break;
				else
				{
					map<uint64_t, xfile_shared_ptr>::iterator it = xfiles.find(pxid);
					if(it == xfiles.end())
					{
						ret = false;
						break;
					}else
					{
						spLastXfile = it->second;
					}
				}
			}
			return ret;
		}

		bool server_directory_tree::_insert( vector<xfile_shared_ptr>& xfilePath )
		{
			bool ret = true;
			shared_ptr<server_directory_tree::node> spParentNode = m_spRoot;
			for(int i = xfilePath.size()-1; i >= 0; --i)
			{
				shared_ptr<server_directory_tree::node> spNode(new server_directory_tree::node());
				spNode->reset_parent(spParentNode);
				spNode->set_xfile(xfilePath[i]);

				shared_ptr<server_directory_tree::node> spFind = spParentNode->find(spNode->file()->m_xid);
				if( !spFind )
				{
					spParentNode->insert_sub_node(spNode->file()->m_xid, spNode);
					spFind = spNode;
				}
				spParentNode = spFind;
			}
			return ret;
		}

		void server_directory_tree::for_each( std::function<void(shared_ptr<server_directory_tree::node>)> func )
		{
			deque<shared_ptr<server_directory_tree::node>> dirNodeQueue;
			dirNodeQueue.push_back(m_spRoot);
			for(; dirNodeQueue.empty() == false; )
			{
				shared_ptr<server_directory_tree::node> spDir = dirNodeQueue.front();
				dirNodeQueue.pop_front();
				map<uint64_t, std::shared_ptr<server_directory_tree::node>> subItems = spDir->sub_items();
				map<uint64_t, std::shared_ptr<server_directory_tree::node>>::iterator it = subItems.begin();
				for(; it!= subItems.end(); ++it)
				{
					if(it->second->file()->m_type == XFILE_FOLDER)
						dirNodeQueue.push_back(it->second);
					func(it->second);
				}
			}
		}

		void server_directory_tree::clear()
		{
			for_each(std::bind(&server_directory_tree::_unparent,this, std::placeholders::_1));
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot.reset(new server_directory_tree::node());
			m_spRoot->set_xfile(spXfile);
		}

		void server_directory_tree::_unparent( shared_ptr<node> spNode )
		{
			spNode->reset_parent();
		}

		void server_directory_tree::_full_path( vector<xfile_shared_ptr>& xfilePath )
		{
			string path = "/";
			for( int i = xfilePath.size()-1; i >= 0; --i)
			{
				path += xfilePath[i]->m_name;
				xfilePath[i]->m_path = path;
				path += "/";
			}
		}
	}
}