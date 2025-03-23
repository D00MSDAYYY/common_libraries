#include "script_engine.hpp"

std::shared_ptr< script::real_engine >
script::real_engine::create()
{
	return std::shared_ptr< real_engine >{ new real_engine{} };
}

std::shared_ptr< script::proxy_engine >
script::real_engine::make_proxy( sol::environment env )
{
	return proxy_engine::create( shared_from_this(), env );
}

std::shared_ptr< script::proxy_engine >
script::proxy_engine::create( std::weak_ptr< engine > ngn, sol::environment env )
{
	return std::shared_ptr< proxy_engine > { new proxy_engine{ ngn, env } };
}
