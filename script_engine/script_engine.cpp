#include "script_engine.hpp"

script::engine::engine()
{
}

void
script::engine::add_object( const object& obj )
{
	obj.self_register(*this);
}

void
script::engine::remove_object( const object& obj )
{
	obj.self_unregister(*this);
}
