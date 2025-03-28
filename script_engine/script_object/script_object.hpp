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
		: _ngn_ptr{ ngn_ptr }
		, _name{ name }
	{
	}

	virtual ~object() { object::self_unregister(); }

protected:
	virtual void
	self_register(script::object* ptr) const // can throw if name already exist
	{
		if (ptr)
		{
			if ( _ngn_ptr )
				{
					if ( _ngn_ptr->globals() [ _name ] == sol::lua_nil )
						{
							_ngn_ptr->globals() [ _name ] = this;
							_ngn_ptr->script( "if (print) then print('" + _name
											  + " registered') end " );
						}
					else
						{
							std::runtime_error( "Script object with the name '" + _name
												+ "' is already registered in engine" );
						}
				}
			else { throw std::runtime_error( "Parent engine is not available" ); }
		}
		else { throw std::runtime_error( "Nullptr passed to script::object::self_register" ); }
	}

	virtual void
	self_unregister() const
	{
		std::cout << "in self_unregister() of " << _name << std::endl;
		if ( _ngn_ptr )
			{
				_ngn_ptr->globals() [ _name ] = sol::lua_nil;
				_ngn_ptr->script( "if (print) then print('" + _name
								  + " unregistered') end" );
			}
	}

	const engine::ptr _ngn_ptr{};
	const std::string _name{};

private:
	object( const object& obj ) = delete;
	object&
	operator= ( const object& obj )
		= delete;
};
} // namespace script
