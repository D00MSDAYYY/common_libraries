#pragma once
#define SOL_ALL_SAFETIES_ON 1

#include <memory>
#include <sol/sol.hpp>

namespace script
{
class proxy_engine;

/////////////////////////////////////////////////////////////////////
class engine : public std::enable_shared_from_this< engine >
{
public:
	virtual std::shared_ptr< proxy_engine >
	make_proxy( sol::environment env ) = 0;

	template < typename... Args >
	void
	open_libraries( Args&&... args );

	virtual ~engine() { };

protected:
	engine() { };
	engine( const engine& se ) = delete;
	engine&
	operator= ( const engine& se )
		= delete;
};

/////////////////////////////////////////////////////////////////////
class real_engine : public engine
{
public:
	static std::shared_ptr< real_engine >
	create();

	virtual std::shared_ptr< proxy_engine >
	make_proxy( sol::environment env ) override;

	virtual ~real_engine() { };

protected:
	real_engine() { };

private:
	sol::state _state{};
};

/////////////////////////////////////////////////////////////////////
class proxy_engine : public engine
{
public:
	static std::shared_ptr< proxy_engine >
	create( std::weak_ptr< engine > ngn, sol::environment env );

	virtual std::shared_ptr< proxy_engine >
	make_proxy( sol::environment env ) override;

	virtual ~proxy_engine() { };

protected:
	proxy_engine( std::weak_ptr< engine > ngn, sol::environment env )
		: _prnt_ngn{ ngn }
		, _env{ env } { };

private:
	std::weak_ptr< engine > _prnt_ngn;
	sol::environment		_env{};
};

/////////////////////////////////////////////////////////////////////
} // namespace script
