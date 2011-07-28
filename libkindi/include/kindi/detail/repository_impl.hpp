// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "kindi/detail/repository.hpp"

#include "kindi/type.hpp"

#include "kindi/traits/constructor.hpp"
#include "kindi/traits/wrapped_type.hpp"

#include "kindi/detail/auto_recursive_registration.hpp"
#include "kindi/detail/build_info.hpp"
#include "kindi/detail/builtin_providers.hpp"

#include "kindi/detail/debug_template.hpp"

namespace kindi
{
	namespace detail
	{
		template <typename T, typename Implementation>
		abstract_base_provider* get_provider( repository& r,
		                                      T* instance,
		                                      boost::mpl::false_ /* has instance */ )
		{
			return new generic_provider<T, Implementation>( r );
		}

		template <typename T, typename Implementation>
		abstract_base_provider* get_provider( repository& r,
		                                      T* instance,
		                                      boost::mpl::true_ /* has instance */ )
		{
			return new provider_with_instance<T>( instance );
		}
	} // ns detail
} // ns kindi

template <typename T>
void kindi::detail::repository::declare_type_if_unknown()
{
	add_if_unknown( type<T>() );
}

template <typename T, typename BuildProperties>
void kindi::detail::repository::add( const kindi::detail::build_info<T, BuildProperties>& build_info )
{
	type_info new_type_info = type_info( kindi::type_wrapper<T>() );
	
	typedef typename traits::constructor<T>::type constructor_t;
	
	// recursively register parameters types and bound types
	auto_recursive_registration<T, BuildProperties>( *this )();
	
	// creates the provider for the new type
	abstract_base_provider* pProvider =
		detail::get_provider<T, typename BuildProperties::implementation>( *this, build_info.m_instance,
			typename BuildProperties::has_instance() );
	
	// insert/override the provider for the new type
	m_mapTypes[ new_type_info].reset( pProvider );
	
	// insert/override the provider for the provider of the new type
	// allows the user to ask for a provider<Type>
	type_info new_type_provider_info = type_info( kindi::type_wrapper<provider<T> >() );
	m_mapTypes[ new_type_provider_info ].reset( new detail::provider_provider( pProvider ) );
}

template <typename T>
T* kindi::detail::repository::construct()
{
	// checks that the type is complete
	// issues a compilation error if it's not ( from boost::checked_delete )
	// we cannot build an incomplete type!
	typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
	(void) sizeof(type_must_be_complete);

	return get_provider<T>()->construct();
}

template <typename T>
kindi::provider<T>* kindi::detail::repository::get_provider()
{
	type_info searched_type = type_info( kindi::type_wrapper<T>() );

	types_map_t::iterator it = m_mapTypes.find( searched_type );
	if( it == m_mapTypes.end() )
	{
		throw std::runtime_error( (boost::format( "Unable to find type %s" ) % searched_type).str() );
	}

	return static_cast<provider<T>*>( it->second.get() );
}

template <typename T, typename BuildProperties>
void kindi::detail::repository::add_if_unknown( const kindi::detail::build_info<T, BuildProperties>& build_info )
{
	type_info new_type_info = type_info( kindi::type_wrapper<T>() );
	
	// first we search the map to see if the type is already in the map of know types
	types_map_t::const_iterator it = m_mapTypes.find( new_type_info );
	if( it != m_mapTypes.end() )
		return;	// nothing more to do
	
	add( build_info );
}
