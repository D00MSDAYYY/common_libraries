#pragma once

#include <string>
#include <vector>

// TODO! replace this library in the future with more consistent one

namespace actions_tree
{
class node;
using tag_t		  = std::string;
using tags		  = std::vector< tag_t >;
using ch_node_ptr = std::shared_ptr< node >;
using p_node_ptr  = std::weak_ptr< node >;

struct node_args // passed args 'lua-style table' for better readability
{
	const std::string		   name;
	tags					   tags		   = {};
	std::string				   script	   = {}; // TODO mb remove to make more generic class (and allow to subclass and add if needed)
	std::vector< ch_node_ptr > children	   = {};
	p_node_ptr				   parent	   = {};
	std::string				   description = {};
};

struct node
{
	const std::string		   _name;
	tags					   _tags; // (like "modifying", "unsafe", "deprecated" etc.)
	std::string				   _script;
	p_node_ptr				   _parent;
	std::vector< ch_node_ptr > _children;
	std::string				   _description;

	// TODO! add methods  add_child, add_tag for checking for duplications and circular
	// dependices

	node( std::string name,
		  tags		  tags					  = {},
		  std::string script				  = {},
		  std::vector< ch_node_ptr > children = {},
		  p_node_ptr parent					  = {},
		  std::string description			  = {} )
		: _name{ name }
		, _tags{ tags }
		, _script{ script }
		, _parent{ parent }
		, _children{ children }
		, _description{ description }
	{
	}

	node( node_args args )
		: node( std::move( args.name ),
				std::move( args.tags ),
				std::move( args.script ),
				std::move( args.children ),
				std::move( args.parent ),
				std::move( args.description ) )
	{
	}
};
} // namespace actions_tree
