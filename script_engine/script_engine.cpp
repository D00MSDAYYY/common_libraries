#include "script_engine.hpp"

script::engine::engine() { }

script::engine
script::engine::wrap( const sol::environment& env )
{
	return script::engine{env};
}

void
script::engine::add_object( const object& obj )
{
	obj.self_register( *this );
}

void
script::engine::remove_object( const object& obj )
{
	obj.self_unregister( *this );
}

void
script::engine::do_code_globally( const std::string& script )
{
	if ( _env ) _state.script( script, _env.value() );
	else _state.script( script );
}

void
script::engine::do_code_separately( const std::string& script )
{
	sol::state state;
	state.script(script);
}
