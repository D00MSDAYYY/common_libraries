#pragma once

#include "engine.hpp"

#include <string>

////////////////////////
// //
// aux macroses    //
// //
////////////////////////
#define CLASS_NAME_AS_STRING( T ) static constexpr std::string _class_name{ #T };

#define MAKE_LUA_OBJECT_FROM_THIS()                                                      \
	virtual sol::object make_lua_object_from_this() const override                       \
	{                                                                                    \
		if ( _ngn_ptr ) return sol::make_object( ( *_ngn_ptr )->lua_state(), this );     \
		else                                                                             \
			{                                                                            \
				std::cerr << "scripting engine is not set" << std::endl;                 \
				return sol::lua_nil;                                                     \
			}                                                                            \
	}

#define STATIC_REGISTER_IN_LUA()                                                         \
	static void register_in_lua( const scripting::engine::ptr& ngn_ptr );

namespace scripting
{
class object
{
public:
	object( std::optional< const engine::ptr > ngn_ptr = std::nullopt )
		: _ngn_ptr{ ngn_ptr }
	{
	}

	virtual ~object() = default;

	virtual sol::object
	make_lua_object_from_this() const
	{
		if ( _ngn_ptr ) return sol::make_object( ( *_ngn_ptr )->lua_state(), this );
		else
			{
				std::cerr << "scripting engine is not set" << std::endl;
				return sol::lua_nil;
			}
	};

	static void
	register_in_lua( const engine::ptr& ngn_ptr )
	{
		throw std::runtime_error{ "i forgot to 'override' this static function" };
	};

	template < typename CLASS >
	static bool
	can_register_in_lua( const engine::ptr& ngn_ptr )
	{
		auto class_name{ CLASS::_class_name }; // check if class have class_name string
											   // (use macros CLASS_NAME_AS_STRING)

		if ( class_name.empty() )
			{
				std::cerr << "class name is not set" << std::endl;
				return false;
			}

		else if ( ngn_ptr->globals() [ class_name ] != sol::lua_nil )
			{
				std::cerr << "class with name '" + class_name
								 + "' is already registered in global namespace "
								   "or current environment"
						  << std::endl;
				return false;
			}
		else { return true; }
	}

protected:
	std::optional< const engine::ptr > _ngn_ptr{};
};

} // namespace scripting

