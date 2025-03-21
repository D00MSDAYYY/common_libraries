#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "script_engine.hpp"

#include <sol/sol.hpp>

namespace script
{
class engine;

class object
{
	// public:
	// 	object();
	// 	object( const object& obj )				= delete;
	// 	object& operator= ( const object& obj ) = delete;

	// 	void	self_register( const engine& ngn ) const;
	// 	void	self_unregister( const engine& ngn ) const;

	// private:

public:
	object( std::weak_ptr< sol::state > state )
		: _state{ state }
	{
	}

	std::weak_ptr< sol::state > _state{};
};
} // namespace script
