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

protected:
	engine() { };
	engine( const engine& se )			   = delete;
	engine& operator= ( const engine& se ) = delete;
	virtual ~engine() { };
};

class real_engine : public engine
{
public:
	virtual std::shared_ptr< proxy_engine > make_proxy( sol::environment env ) { };

	virtual void							do_script( const std::string& script ) { };

	virtual std::string						get_stack_dump() { };

	template < typename T, typename... Args >
	friend std::shared_ptr< T > make_engine( Args&&... args );

protected:
	real_engine() { };
	virtual ~real_engine() { };


private:
	sol::state _state{};
};

class proxy_engine : public engine
{
public:
	virtual std::shared_ptr< proxy_engine > make_proxy( sol::environment env ){};

	virtual void							do_script( const std::string& script ){};

	virtual std::string						get_stack_dump(){};

	template < typename T, typename... Args >
	friend std::shared_ptr< T > make_engine( Args&&... args );

protected:
	proxy_engine( std::weak_ptr< engine > ngn, sol::environment env )
		: _prnt_ngn{ ngn }
		, _env{ env } { };
	virtual ~proxy_engine() { };

private:
	std::weak_ptr< engine > _prnt_ngn{};
	sol::environment		_env{};
};

template < typename T, typename... Args >
std::shared_ptr< T >
make_engine( Args&&... args )
{
	return std::shared_ptr< T >( new T( std::forward< Args >( args )... ) );
}

} // namespace script


