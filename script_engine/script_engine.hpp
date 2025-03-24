#pragma once
#define SOL_ALL_SAFETIES_ON 1

#include <memory>
#include <sol/sol.hpp>

namespace script
{
/////////////////////////////////////////////////////////////////////
class engine final : public std::enable_shared_from_this< engine >
{
public:
	using ptr = std::shared_ptr< engine >;

	static ptr
	make_real_engine()
	{
		return std::shared_ptr< engine >( new engine{} );
	};

	/////////////////////////////////////////////////////////////////////
	static ptr
	make_proxy_engine( std::weak_ptr< engine > parent, sol::environment env )
	{
		return std::shared_ptr< engine >( new engine{ parent, env } );
	}

	/////////////////////////////////////////////////////////////////////
	bool
	is_proxy() const
	{
		return std::holds_alternative< proxy_engine_data >( _data );
	}

	/////////////////////////////////////////////////////////////////////
	bool
	is_real() const
	{
		return std::holds_alternative< real_engine_data >( _data );
	}

	/////////////////////////////////////////////////////////////////////
	template < typename... Args >
	void
	open_libraries( Args&&... args )
	{
		if ( is_real() )
			{
				std::get< real_engine_data >( _data ).open_libraries(
					std::forward< Args >( args )... );
			}
		else { throw std::runtime_error( "Cannot open libraries on a proxy engine" ); }
	}

	/////////////////////////////////////////////////////////////////////
	sol::function_result
	script( const std::string_view& code,
			const std::string&		chunk_name = "[string]",
			sol::load_mode mode				   = sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script( code,
																	 chunk_name,
																	 mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->script( code, _env, chunk_name, mode );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::protected_function_result
	script( const sol::string_view& code,
			const sol::environment& env,
			const std::string&		chunk_name = "[string]",
			sol::load_mode mode				   = sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script( code,
																	 env,
																	 chunk_name,
																	 mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						sol::environment tmp_env{
							_env.lua_state(), //! very bad workaround, but there is no way
											  //! to avoid this break of encapsulation
											  //! (check .lua_state() method in
											  //! state_view) to create an env
							sol::create
						};

						for ( const auto& [ key, value ] : env )
							{
								if ( _env [ key ] == value ) { tmp_env [ key ] = value; }
							}
						return prnt->script( code, tmp_env, chunk_name, mode );
					}
				else
					{
						throw std::runtime_error( "Parent engine is not available" );
					} // maybe return bad function result in future
			}
	}

	/////////////////////////////////////////////////////////////////////
	template < typename ErrorFunc >
	requires std::invocable< ErrorFunc, sol::protected_function_result >
			 sol::protected_function_result
			 script( const std::string_view& code,
					 ErrorFunc&&			 on_error,
					 const std::string&		 chunk_name = "[string]",
					 sol::load_mode mode				= sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script( code,
																	 on_error,
																	 chunk_name,
																	 mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->script( code, _env, on_error, chunk_name, mode );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	template < typename ErrorFunc >
	requires std::invocable< ErrorFunc, sol::protected_function_result >
			 sol::protected_function_result
			 script( const std::string_view& code,
					 const sol::environment& env,
					 ErrorFunc&&			 on_error,
					 const std::string&		 chunk_name = "[string]",
					 sol::load_mode mode				= sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script( code,
																	 env,
																	 on_error,
																	 chunk_name,
																	 mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						sol::environment tmp_env{ _env.lua_state(), sol::create };

						for ( const auto& [ key, value ] : env )
							{
								if ( _env [ key ] == value ) { tmp_env [ key ] = value; }
							}
						return prnt->script( code, tmp_env, on_error, chunk_name, mode );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::function_result
	script_file( const std::string& filename, sol::load_mode mode = sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script_file( filename,
																		  mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->script_file( filename,
												  _env,
												  mode ); //! dont forget about env here
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::protected_function_result
	script_file( const std::string&		 filename,
				 const sol::environment& env,
				 sol::load_mode			 mode = sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script_file( filename,
																		  env,
																		  mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						sol::environment tmp_env{ _env.lua_state(), sol::create };
						for ( const auto& [ key, value ] : env )
							{
								if ( _env [ key ] == value ) { tmp_env [ key ] = value; }
							}
						return prnt->script_file( filename, tmp_env, mode );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	template < typename ErrorFunc >
	requires std::invocable< ErrorFunc, sol::protected_function_result >
			 sol::protected_function_result
			 script_file( const std::string& filename,
						  ErrorFunc&&		 on_error,
						  sol::load_mode	 mode = sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script_file( filename,
																		  on_error,
																		  mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->script_file( filename, _env, on_error, mode );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	template < typename ErrorFunc >
	requires std::invocable< ErrorFunc, sol::protected_function_result >
			 sol::protected_function_result
			 script_file( const std::string&	  filename,
						  const sol::environment& env,
						  ErrorFunc&&			  on_error,
						  sol::load_mode		  mode = sol::load_mode::any )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).script_file( filename,
																		  env,
																		  on_error,
																		  mode );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						sol::environment tmp_env{ _env.lua_state(), sol::create };
						for ( const auto& [ key, value ] : env )
							{
								if ( _env [ key ] == value ) { tmp_env [ key ] = value; }
							}
						return prnt->script_file( filename, tmp_env, on_error, mode );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::object
	require( const std::string& key,
			 lua_CFunction		open_function,
			 bool				create_global = true )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).require( key,
																	  open_function,
																	  create_global );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						auto mod{ prnt->require( key, open_function, false ) };
						if ( create_global == true ) { _env [ key ] = mod; }
						return mod;
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::object
	require_script( const std::string& key,
					const std::string& code,
					bool			   create_global = true )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).require_script(
					key,
					code,
					create_global );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						auto mod{ prnt->require_script( key, code, false ) };
						if ( create_global == true ) { _env [ key ] = mod; }
						return mod;
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::object
	require_file( const std::string& key,
				  const std::string& file,
				  bool				 create_global = true )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).require_script(
					key,
					file,
					create_global );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						auto mod{ prnt->require_script( key, file, false ) };
						if ( create_global == true ) { _env [ key ] = mod; }
						return mod;
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	// sol::load_result
	// load( lua_Reader		 reader,
	// 	  void*				 data,
	// 	  const std::string& chunk_name = "[string]",
	// 	  sol::load_mode mode			= sol::load_mode::any );
	// /////////////////////////////////////////////////////////////////////
	// sol::load_result
	// load( const std::string_view& code,
	// 	  const std::string&	  chunk_name = "[string]",
	// 	  sol::load_mode mode				 = sol::load_mode::any );
	// /////////////////////////////////////////////////////////////////////
	// sol::load_result
	// load_buffer( const char*		buff,
	// 			 std::size_t		buffsize,
	// 			 const std::string& chunk_name = "[string]",
	// 			 sol::load_mode mode		   = sol::load_mode::any );
	// /////////////////////////////////////////////////////////////////////
	// sol::load_result
	// load_file( const std::string& filename, sol::load_mode mode = sol::load_mode::any
	// );
	/////////////////////////////////////////////////////////////////////
	sol::protected_function_result
	do_string( const std::string_view& code )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).do_string( code );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->do_string( code, _env );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::protected_function_result
	do_file( const std::string& filename )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).do_file( filename );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->do_file( filename, _env );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::protected_function_result
	do_string( const std::string_view& code, sol::environment env )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).do_string( code, env );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						sol::environment tmp_env{ _env.lua_state(), sol::create };
						for ( const auto& [ key, value ] : env )
							{
								if ( _env [ key ] == value ) { tmp_env [ key ] = value; }
							}
						return prnt->do_string( code, tmp_env );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::protected_function_result
	do_file( const std::string& filename, sol::environment env )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).do_file( filename, env );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						sol::environment tmp_env{ _env.lua_state(), sol::create };
						for ( const auto& [ key, value ] : env )
							{
								if ( _env [ key ] == value ) { tmp_env [ key ] = value; }
							}
						return prnt->do_file( filename, tmp_env );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::global_table
	globals() const
	{
		if ( is_real() ) { return std::get< real_engine_data >( _data ).globals(); }
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } ) { return _env; }
				else
					{
						throw std::runtime_error(
							"Parent engine is not available, globals corrupted" );
					}
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::table
	registry() const
	{
		if ( is_real() ) { return std::get< real_engine_data >( _data ).registry(); }
		else { throw std::runtime_error( "Can't return registry on proxy engine" ); }
	}

	/////////////////////////////////////////////////////////////////////
	void
	set_panic( lua_CFunction panic )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).set_panic( panic );
			}
		else { throw std::runtime_error( "Can't set panic function on proxy engine" ); }
	}

	/////////////////////////////////////////////////////////////////////
	std::size_t
	memory_used() const
	{
		if ( is_real() ) { return std::get< real_engine_data >( _data ).memory_used(); }
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } ) { return prnt->memory_used(); }
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	void
	collect_garbage()
	{
		if ( is_real() ) { std::get< real_engine_data >( _data ).collect_garbage(); }
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } ) { return prnt->collect_garbage(); }
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	sol::table
	create_table( int narr = 0, int nrec = 0 )
	{
		if ( is_real() )
			{
				return std::get< real_engine_data >( _data ).create_table( narr, nrec );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						auto new_table{ prnt->create_table( narr, nrec ) };
						_env [ new_table ] = new_table;
						return new_table;
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	template < typename Key, typename Value, typename... Args >
	sol::table
	create_table( int narr, int nrec, Key&& key, Value&& value, Args&&... args )
	{
		if ( is_real() )
			{
				std::get< real_engine_data >( _data ).create_table( narr,
																	nrec,
																	key,
																	value,
																	args... );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						auto new_table{
							prnt->create_table( narr, nrec, key, value, args... )
						};
						_env [ new_table ] = new_table;
						return new_table;
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	template < typename... Args >
	sol::table
	create_table_with( Args&&... args )
	{
		if ( is_real() )
			{
				std::get< real_engine_data >( _data ).create_table_with( args... );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						auto new_table{ prnt->create_table_with( args... ) };
						_env [ new_table ] = new_table;
						return new_table;
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	// static sol::table
	// create_table( lua_State* L, int narr = 0, int nrec = 0 );
	/////////////////////////////////////////////////////////////////////
	// template < typename Key, typename Value, typename... Args >
	// static sol::table
	// create_table( lua_State* L,
	// 			  int		 narr,
	// 			  int		 nrec,
	// 			  Key&&		 key,
	// 			  Value&&	 value,
	// 			  Args&&... args );
	/////////////////////////////////////////////////////////////////////
	// template < typename T >
	// sol::table_proxy< const sol::global_table&, sol::detail::proxy_key_t< T > >
	// operator[] ( T&& key ) const
	// {
	// 	if ( is_real() )
	// 		{
	// 			return std::get< real_engine_data >( _data )
	// 				.globals() [ std::forward< T >( key ) ];
	// 		}
	// 	else
	// 		{
	// 			auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
	// 			if ( auto prnt{ _prnt_ptr.lock() } )
	// 				{
	// 					return _env [ std::forward< T >( key ) ];
	// 				}
	// 			else { throw std::runtime_error( "Parent engine is not available" ); }
	// 		}
	// }

	/////////////////////////////////////////////////////////////////////
	// template < typename T >
	// const sol::table_proxy< const sol::global_table&, sol::detail::proxy_key_t< T > >
	// operator[] ( T&& key ) const
	// {
	// 	if ( is_real() )
	// 		{
	// 			return std::get< real_engine_data >( _data )
	// 				.globals() [ std::forward< T >( key ) ];
	// 		}
	// 	else
	// 		{
	// 			auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
	// 			if ( auto prnt{ _prnt_ptr.lock() } )
	// 				{
	// 					return _env [ std::forward< T >( key ) ];
	// 				}
	// 			else { throw std::runtime_error( "Parent engine is not available" ); }
	// 		}
	// }
	/////////////////////////////////////////////////////////////////////
	template < typename T >
	sol::table_proxy< sol::global_table&, sol::detail::proxy_key_t< T > >
	operator[] ( T&& key )
	{
		// DONT CHANGE HERE ANYTHING IT COULD CAUSE NASTY BUGS AND RUNTIME ERROR
		auto glb = globals();
		auto tmp = glb [ key ];
		return tmp;
		// DONT CHANGE HERE ANYTHING IT COULD CAUSE NASTY BUGS AND RUNTIME ERROR
	}

	template < typename T >
	sol::table_proxy< const sol::global_table&, sol::detail::proxy_key_t< T > >
	operator[] ( T&& key ) const
	{
		// DONT CHANGE HERE ANYTHING IT COULD CAUSE NASTY BUGS AND RUNTIME ERROR
		auto glb = globals();
		auto tmp = glb [ key ];
		return tmp;
		// DONT CHANGE HERE ANYTHING IT COULD CAUSE NASTY BUGS AND RUNTIME ERROR
	}

	/////////////////////////////////////////////////////////////////////
	template < typename Class, typename... Args >
	sol::usertype< Class >
	new_usertype( Args&&... args )
	{
		if ( is_real() )
			{
				return globals().new_usertype< Class >( std::forward< Args >( args )... );
			}
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return _env.new_usertype< Class >(
							std::forward< Args >( args )... );
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	lua_State*
	lua_state() const
	{
		if ( is_real() ) { return std::get< real_engine_data >( _data ).lua_state(); }
		else
			{
				auto& [ _prnt_ptr, _env ]{ std::get< proxy_engine_data >( _data ) };
				if ( auto prnt{ _prnt_ptr.lock() } )
					{
						return prnt->lua_state(); // this breakes the encapsulation, but
												  // fixing this will take half of life
					}
				else { throw std::runtime_error( "Parent engine is not available" ); }
			}
	}

	/////////////////////////////////////////////////////////////////////
	~engine() { };

private:
	engine()
		: _data{ real_engine_data{} } { };
	engine( std::weak_ptr< engine > parent, sol::environment env )
		: _data{
			proxy_engine_data{ parent, env }
	} { };
	engine( const engine& se ) = delete;
	engine&
	operator= ( const engine& se )
		= delete;
	/////////////////////////////////////////////////////////////////////
	using proxy_engine_data = std::pair< std::weak_ptr< engine >, sol::environment >;
	using real_engine_data	= sol::state;
	std::variant< real_engine_data, proxy_engine_data > _data{};
};

} // namespace script
