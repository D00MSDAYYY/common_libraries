#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "script_engine.hpp"

#include <memory>
#include <sol/sol.hpp>

namespace script
{
class engine;
class object;

class object
{
public:
	object( const std::string& name, const engine::ptr& ngn_ptr )
		: _name{ name }
		, _ngn_ptr{ ngn_ptr }
	{
	}

	/////////////////////////////////////////////////////////////////////
	virtual ~object() { }

protected:
	virtual void
	self_register() const
		= 0;
	virtual void
	self_unregister() const
		{
			(*_ngn_ptr)[_name] = sol::lua_nil;
		}

private:
	object( const object& obj ) = delete;

	object&
	operator= ( const object& obj )
		= delete;

	/////////////////////////////////////////////////////////////////////

	const std::string		  _name{};
	const engine::ptr _ngn_ptr{};
};
} // namespace script
