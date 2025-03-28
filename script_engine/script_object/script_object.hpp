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
	object( const std::string& name,const engine::ptr& ngn_ptr  )
		: _ngn_ptr{ ngn_ptr }
		, _name{ name }
	{
	}

	/////////////////////////////////////////////////////////////////////
	virtual ~object() { }

protected:
	virtual void
	self_register() const // can throw if name already exist
	{
		if ( _ngn_ptr )
			{
				( *_ngn_ptr ) [ _name ] = this;
				_ngn_ptr->script("if (print) do print('" +_name + " registered')");
			} 
	}

	virtual void
	self_unregister() const
	{
		if ( _ngn_ptr )
			{
				( *_ngn_ptr ) [ _name ] = this;
				_ngn_ptr->script( "if (print) do print('" + _name + " unregistered')" );
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
