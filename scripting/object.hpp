#pragma once
#include "engine.hpp"

namespace scripting
{
class object
{
public:
	object( const engine::ptr& ngn_ptr )
		: _ngn_ptr{ ngn_ptr }
	{
	}

	object( object&& ) = default;

	virtual ~object() { }

	virtual const std::string
	class_name() const
	{
		return "";
	}

	virtual sol::object
	make_lua_object_from_this() const
	{
		return sol::make_object( _ngn_ptr->lua_state(), this );
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
	}

	const engine::ptr _ngn_ptr{};

private:
	object( const object& obj ) = delete;

	object&
	operator= ( const object& obj )
		= delete;
};

} // namespace scripting

