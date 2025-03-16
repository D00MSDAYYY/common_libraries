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
	engine( const engine& se )				   = delete;
	engine&		operator= ( const engine& se ) = delete;

	void		add_object( const object& obj );
	void		remove_object( const object& obj );
	void		do_code_globally( std::string script );
	void		do_code_separately( std::string script );
	std::string get_stack_dump();

private:
	sol::state _state;
};
} // namespace script


