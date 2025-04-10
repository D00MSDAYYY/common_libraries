#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include "script_engine.hpp"

#include <sol/sol.hpp>

namespace script
{
class engine;
class object;

class object
{
public:
	object( const std::string& name, const engine::ptr& ngn_ptr )
		: _ngn_ptr{ ngn_ptr }
		, _name{ name }
	{
	}

	virtual ~object() { object::self_unregister(); }

	const std::string _name{};

	virtual const std::string
	class_name() const
	{
		return "";
	}

	virtual sol::object
	make_lua_object_from_this() const 
	{
		throw std::runtime_error(
			"You don't override create_lua_object_from_this() but call it (virtual)" );
	};


protected:
	bool
	can_self_register()
	{
		if ( !_ngn_ptr )
			{
				std::cerr << "script engine is not available" << std::endl;
				return false;
			}

		else if ( _name.empty() )
			{
				std::cerr << "object name cannot be empty" << std::endl;
				return false;
			}

		else if ( class_name().empty() )
			{
				std::cerr << "class name cannot be empty" << std::endl;
				return false;
			}

		else if ( _ngn_ptr->globals() [ class_name() ] != sol::lua_nil )
			{
				std::cerr << "class with name '" + class_name()
								 + "' is already registered in global namespace "
								   "(current environment)"
						  << std::endl;
				return false;
			}
		else { return true; }
	}

	virtual void
	self_register()
		= 0;

	

	virtual void
	self_unregister()
	{
		if ( _ngn_ptr )
			{
				_ngn_ptr->globals() [ _name ] = sol::lua_nil;
				_ngn_ptr->script( "if (print) then print('[unregistered]\t" + _name
								  + "') end" );
			}
	}

	const engine::ptr _ngn_ptr{};

private:
	object( const object& obj ) = delete;
	object&
	operator= ( const object& obj )
		= delete;
};
} // namespace script
