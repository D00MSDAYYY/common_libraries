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
struct node_args
{
	const std::string			name;
	tags_t						tags		= {};
	std::string					description = {};
	TYPE						data		= {};
	std::vector< node< TYPE > > children	= {};
};

template < typename TYPE >
struct node
{
	// TODO! add methods  add_child, add_tag for checking for duplications and circular
	// dependices

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

	node( node_args< TYPE > args )
		: node( std::move( args.name ),
				std::move( args.tags ),
				std::move( args.description ),
				std::move( args.data ),
				std::move( args.children ) )
	{
	}

	const std::string			_name;
	tags_t						_tags;
	std::string					_description;
	TYPE						_data;
	std::vector< node< TYPE > > _children;
};

} // namespace actions_tree

