#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

// TODO! replace this library in the future with more consistent one

namespace actions_tree
{

using tags_t = const std::vector< std::string >;

template < typename >
class node;

template < typename TYPE >
struct node
{
	node( std::string name,
		  tags_t	  tags					   = {},
		  std::string description			   = {},
		  TYPE data							   = {},
		  std::vector< node< TYPE > > children = {} )
		: _name{ name }
		, _tags{ tags }
		, _description{ description }
		, _data{ data }
		, _children{ children }
	{
	}

	const std::string			_name;
	tags_t						_tags;
	std::vector< node< TYPE > > _children;
	std::string					_description;
	TYPE						_data;
};

} // namespace actions_tree

