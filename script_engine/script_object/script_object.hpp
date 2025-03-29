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
	template < typename T >
	void
	self_register( T* ptr )
	{
		auto is_ok{ static_cast< script::object* >( ptr ) };

		if ( is_ok )
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
								std::runtime_error(
									"Script object with the name '" + _name
									+ "' is already registered in engine" );
							}
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
		else
			{
				throw std::runtime_error(
					"Passed pointer to a function script::object::self_register(T* ptr) "
					"can't be onverted to a base class pointer" );
			}
	}

	virtual void
	self_register() const // can throw if name already exist
		= 0;

	virtual void
	self_unregister() const
	{
		if ( _ngn_ptr )
			{
				_ngn_ptr->globals() [ _name ] = sol::lua_nil;
				_ngn_ptr->script( "if (print) then print('[unregistered]\t" + _name
								  + "') end" );
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
