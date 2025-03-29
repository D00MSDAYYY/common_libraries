#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "script_engine.hpp"

#include <memory>
#include <optional>
#include <sol/sol.hpp>

namespace script
{
class engine;
class object;

class object
{
public:
	object( const std::string&			name,
			const engine::ptr&			ngn_ptr,
			std::optional< sol::table > prnt_tbl = std::nullopt )
		: _ngn_ptr{ ngn_ptr }
		, _name{ name }
		, _prnt_tbl{ prnt_tbl }
	{
	}

	virtual ~object()
	{
		std::cout << "in\t" << _name << "\t destructor" << std::endl;
		std::cout << "use count of " << _ngn_ptr->lua_state()
				  << " engine : " << _ngn_ptr.use_count() << std::endl;
		object::self_unregister();
	}

protected:
	template < typename T >
	void
	self_register( T* ptr )
	{
		script::object* obj_ptr = dynamic_cast< script::object* >( ptr );
		if ( !obj_ptr )
			{
				throw std::runtime_error(
					"Passed pointer cannot be converted to script::object pointer" );
			}

		if ( !_ngn_ptr ) { throw std::runtime_error( "Parent engine is not available" ); }

		if ( _name.empty() )
			{
				throw std::runtime_error( "Object name cannot be empty" );
			}

		sol::table target_table = _prnt_tbl.value_or( _ngn_ptr->globals() );

		if ( target_table [ _name ] != sol::lua_nil )
			{
				throw std::runtime_error(
					"script::object with name '" + _name + "' is already registered in "
					+ ( _prnt_tbl ? "parent table" : "global namespace" ) );
			}

		target_table [ _name ] = this;

		_ngn_ptr->script( "if print then print('["
						  + std::string( _prnt_tbl ? "table" : "global" )
						  + " registration] " + _name + "') end" );
	}

	virtual void
	self_register() // can throw if name already exist
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

	const engine::ptr				  _ngn_ptr{};
	const std::string				  _name{};
	// const sol::table				  _slf_tbl{};
	const std::optional< sol::table > _prnt_tbl{};

private:
	object( const object& obj ) = delete;
	object&
	operator= ( const object& obj )
		= delete;
};
} // namespace script
