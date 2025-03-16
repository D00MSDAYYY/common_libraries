#include "script_engine.hpp"

script_engine::script_engine()
{
	_state.open_libraries( sol::lib::base,
						   sol::lib::package,
						   sol::lib::table,
						   sol::lib::string,
						   sol::lib::math );
}

void
script_engine::add_object( const std::string& name, sol::table object )
{
	_state[ name ] = object;
}

void
script_engine::remove_object( const std::string& name )
{
	_state[ name ] = sol::nil;
}

void
script_engine::do_code_globally( std::string script )
{
	try
		{
			_state.script( script );
		}
	catch ( const sol::error& e )
		{
			std::cerr << "Error executing global script: " << e.what() << std::endl;
		}
}

void
script_engine::do_code_separately( std::string script )
{
	sol::state local_state;				  
	local_state.open_libraries( sol::lib::base,
								sol::lib::package,
								sol::lib::table,
								sol::lib::string,
								sol::lib::math );

	try
		{
			local_state.script( script ); 
		}
	catch ( const sol::error& e )
		{
			std::cerr << "Error executing separate script: " << e.what() << std::endl;
		}
}

std::string
script_engine::get_stack_dump()
{
	sol::function traceback = _state[ "debug" ][ "traceback" ];
	return traceback();
}
