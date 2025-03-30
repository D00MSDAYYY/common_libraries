#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "script_engine.hpp"

#include <memory>
#include <optional>
#include <sol/sol.hpp>

namespace script
{
class engine;
class object;

class object
{
public:
	object( const std::string&			name,
			const engine::ptr&			ngn_ptr,
			std::optional< sol::table > prnt_tbl = std::nullopt )
		: _ngn_ptr{ ngn_ptr }
		, _name{ name }
		, _prnt_tbl{ prnt_tbl }
	{
	}

	virtual ~object()
	{
		std::cout << "in\t" << _name << "\t destructor" << std::endl;
		std::cout << "use count of " << _ngn_ptr->lua_state()
				  << " engine : " << _ngn_ptr.use_count() << std::endl;
	}

	const std::string _name{};

protected:
	virtual void
	self_register() // can throw if _name already exist
		= 0;

	virtual void
	self_unregister()
		= 0;

	const engine::ptr				  _ngn_ptr{};

	const std::optional< sol::table > _prnt_tbl{};

private:
	object( const object& obj ) = delete;
	object&
	operator= ( const object& obj )
		= delete;
};
} // namespace script
