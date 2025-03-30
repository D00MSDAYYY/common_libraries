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
	object( const std::string& name, const engine::ptr& ngn_ptr )
		: _ngn_ptr{ ngn_ptr }
		, _name{ name }
	{
	}

	virtual ~object()
	{
		std::cout << "in\t" << _name << "\t destructor" << std::endl;
		std::cout << "use count of " << _ngn_ptr->lua_state()
				  << " engine : " << _ngn_ptr.use_count() << std::endl;
		object::self_unregister();
	}

	const std::string _name{};

	virtual const std::string
	class_name() // this func neaded to force users (programmers) to provide a class name
				 // which will be used in template self_register
		= 0;

protected:
	template < typename T >
	 void
	self_register( T* ptr )
	{
		auto is_ok = dynamic_cast< script::object* >( ptr );
		if ( !is_ok )
			{
				throw std::runtime_error(
					"Passed pointer cannot be converted to script::object pointer" );
			}

		const auto ngn_ptr{ ptr->_ngn_ptr };
		if ( !ngn_ptr ) { throw std::runtime_error( "Parent engine is not available" ); }

		const auto name{ ptr->_name };
		if ( name.empty() ) { throw std::runtime_error( "Object name cannot be empty" ); }

		const auto class_name{ ptr->class_name() };
		if ( class_name.empty() )
			{
				throw std::runtime_error( "Object name cannot be empty" );
			}
		if ( ngn_ptr->globals() [ class_name ] != sol::lua_nil )
			{
				throw std::runtime_error( "class with name '" + class_name
										  + "' is already registered in global namespace "
											"(current environment)" );
			}

		// if ( ngn_ptr->globals() [ name ] != sol::lua_nil ) // TODO i don't is this ok
		// check or not
		// 	{
		// 		throw std::runtime_error( "script::object with name '" + name
		// 								  + "' is already registered in global namespace "
		// 									"(current environment)" );
		// 	}
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

	const engine::ptr _ngn_ptr{};

	// const sol::table				  _slf_tbl{};
	// const std::optional< sol::table > _prnt_tbl{};

private:
	object( const object& obj ) = delete;
	object&
	operator= ( const object& obj )
		= delete;
};
} // namespace script
