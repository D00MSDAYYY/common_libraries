#pragma once

#include <string>
#include <variant>
#include <vector>

// TODO! replace this library in the future with more consistent one

namespace actions_tree
{

using tags = const std::vector< std::string >;

template < typename >
class node;

template < typename TYPE >
using ch_node_ptr = std::shared_ptr< node< TYPE > >;

template < typename TYPE >
using p_node_ptr = std::weak_ptr< node< TYPE > >;

template < typename TYPE >
struct node_args
{
	const std::string				   name;
	tags							   tags		   = {};
	std::vector< ch_node_ptr< TYPE > > children	   = {};
	p_node_ptr< TYPE >				   parent	   = {};
	std::string						   description = {};
	TYPE							   data		   = {};
};

template < typename TYPE >
struct node
{
	// TODO! add methods  add_child, add_tag for checking for duplications and circular
	// dependices

	node( std::string name,
		  tags		  tags							  = {},
		  std::vector< ch_node_ptr< TYPE > > children = {},
		  p_node_ptr< TYPE > parent					  = {},
		  std::string description					  = {},
		  TYPE data									  = {} )
		: _name{ name }
		, _tags{ tags }
		, _parent{ parent }
		, _children{ children }
		, _description{ description }
	{
	}

	node( node_args< TYPE > args )
		: node( std::move( args.name ),
				std::move( args.tags ),
				std::move( args.children ),
				std::move( args.parent ),
				std::move( args.description ) )
	{
	}

	const std::string				   _name;
	tags							   _tags;
	p_node_ptr< TYPE >				   _parent;
	std::vector< ch_node_ptr< TYPE > > _children;
	std::string						   _description;
};

} // namespace actions_tree

