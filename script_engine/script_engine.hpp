#pragma once
#define SOL_ALL_SAFETIES_ON 1

#include <memory>
#include <sol/sol.hpp>

namespace script
{
class proxy_engine;

class engine
{
public:
	virtual std::shared_ptr< proxy_engine > make_proxy( sol::environment env )	   = 0;

	virtual void							do_script( const std::string& script ) = 0;

	virtual std::string						get_stack_dump()					   = 0;

	template < typename T, typename... Args >
	friend std::shared_ptr< T > make_engine( Args&&... args );
	virtual ~engine() { };

protected:
	engine() { };
	engine( const engine& se )			   = delete;
	engine& operator= ( const engine& se ) = delete;
};

class real_engine : public engine
{
public:
	virtual std::shared_ptr< proxy_engine > make_proxy( sol::environment env ) { };

	virtual void							do_script( const std::string& script ) { };

	virtual std::string						get_stack_dump() { };

	template < typename T, typename... Args >
	friend std::shared_ptr< T > make_engine( Args&&... args );
	virtual ~real_engine() { };

protected:
	real_engine() { };


private:
	sol::state _state{};
};

class proxy_engine : public engine
{
public:
	virtual std::shared_ptr< proxy_engine > make_proxy( sol::environment env ) { };

	virtual void							do_script( const std::string& script ) { };

	virtual std::string						get_stack_dump() { };

	template < typename T, typename... Args >
	friend std::shared_ptr< T > make_engine( Args&&... args );
	virtual ~proxy_engine() { };

protected:
	proxy_engine( std::weak_ptr< engine > ngn, sol::environment env )
		: _prnt_ngn{ ngn }
		, _env{ env } { };


private:
	std::weak_ptr< engine > _prnt_ngn{};
	sol::environment		_env{};
};

template < typename T, typename... Args >
std::shared_ptr< T >
make_engine( Args&&... args )
{
	return std::make_shared< T >( std::forward< Args >( args )... );
}

} // namespace script


