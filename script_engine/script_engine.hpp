#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

//! TODO create some aux function to log creation/deletion of objects ("shortcuts")
class script_engine
{
public:
	script_engine();
	script_engine( const script_engine& se )			 = delete;
	script_engine& operator= ( const script_engine& se ) = delete;

	void		   add_object( const std::string& name, sol::table object );
	void		   remove_object(const std::string& name);
	void		   do_code_globally( std::string script );
	void		   do_code_separately( std::string script );
	std::string	   get_stack_dump();

private:
	sol::state _state;
};
