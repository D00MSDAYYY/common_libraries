#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "script_engine.hpp"

#include <sol/sol.hpp>

namespace script
{
class engine;

class object
{
public:
	object( const std::string& name, const engine::ptr ngn )
		: _ngn{ ngn }
	{
	}

	virtual ~object() { }

	object( const object& obj ) = delete;
	object&
	operator= ( const object& obj )
		= delete;

protected:
	virtual void
	initialize() final
	{
		_self_register( _ngn );
	};

	virtual void
	uninialize() final
	{
		_self_unregister( _ngn );
	};

	virtual void
	_self_register( const engine::ptr& _ngn ) const
		= 0;
	virtual void
	_self_unregister( const engine::ptr& _ngn ) const
		= 0;

private:
	const engine::ptr _ngn;
};
} // namespace script
