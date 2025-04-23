#pragma once
#include "engine.hpp"

namespace scripting
{
class object
{
public:
	object( std::optional< const engine::ptr& > ngn_ptr = std::nullopt )
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

	std::optional< const std::string > _class_name{ std::nullopt };

	virtual sol::object
	make_lua_object_from_this() const
	{
		if ( _ngn_ptr ) return sol::make_object( ( *_ngn_ptr )->lua_state(), this );
		else
			{
				std::cerr << "script engine is not set" << std::endl;
				return sol::lua_nil;
			}
	};

	static void
	register_in_lua( const engine::ptr& ngn_ptr );

protected:
	virtual void
	self_unregister()
	{
	}

	std::optional< const engine::ptr > _ngn_ptr{};
};

} // namespace scripting

