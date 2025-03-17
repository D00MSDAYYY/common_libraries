#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "script_object.hpp"

#include <sol/sol.hpp>

namespace script
{
class engine
{
public:
	engine();
	engine( sol::environment env );
	engine( const engine& se )				   = delete;
	engine&		operator= ( const engine& se ) = delete;

	engine		wrap( const sol::environment env );

	void		add_object( const object& obj );
	void		remove_object( const object& obj );

	void		do_code_globally( const std::string& script );
	void		do_code_separately( const std::string& script );

// TODO create api to inspect engine "internals" (first checking _env) to iterate over objects
// TODO create propagate_changes() dynamically change the passed objects (for example, from 'main_window' to 'sensor_page' and from 'sensor_page' to 'sensor')
	std::string get_stack_dump();

private:
	sol::state		 _state{};
	sol::environment _env{};
};


} // namespace script


