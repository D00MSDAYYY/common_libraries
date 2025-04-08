#pragma once

#include <string>
#include <vector>

namespace actions_tree
{

using tags_t = const std::vector< std::string >;

template < typename >
class node;

template < typename DATA_TYPE >
struct node_args
{
	const std::string				 name;
	tags_t							 tags		 = {};
	std::string						 description = {};
	DATA_TYPE						 data		 = {};
	std::vector< node< DATA_TYPE > > children	 = {};
	node< DATA_TYPE >*				 _parent	 = {};
};

template < typename DATA_TYPE >
struct node
{
	node( node_args< DATA_TYPE > args )
		: _name{ args.name }
		, _tags{ args.tags }
		, _description{ args.description }
		, _data{ args.data }
		, _children{ args.children }
	{
	}

	const std::string				 _name;
	tags_t							 _tags;
	std::string						 _description;
	DATA_TYPE						 _data;
	std::vector< node< DATA_TYPE > > _children;
	node< DATA_TYPE >*				 _parent{ nullptr };

	const std::string
	get_full_path_name() const
	{
		std::string full_path_name{ _name };
		for ( auto p{ _parent }; p != nullptr; p = p->_parent )
			{
				if ( !p->_name.empty() ) full_path_name = p->_name + "_" + full_path_name;
			}
		return full_path_name;
	}
};

} // namespace actions_tree

