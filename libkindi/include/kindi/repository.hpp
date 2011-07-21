// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "kindi/repository_fwd.hpp"

#include "kindi/detail/build_info.hpp"
#include "kindi/provider.hpp"

#include "kindi/traits/constructor.hpp"
#include "kindi/traits/basic_type.hpp"
#include "kindi/traits/remove_provider.hpp"
#include "kindi/traits/wrapped_type.hpp"

#include "kindi/detail/debug_template.hpp"

#include <boost/mpl/if.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/function_types/parameter_types.hpp>

namespace kindi
{
	namespace detail
	{
		template <typename T, typename Constructor>
		class recursive_registration
		{
		public:
			recursive_registration( kindi::repository& r )
				: m_r( r )
			{
			}
			
			void operator()()
			{
				for_each_ctor_parameter( register_parameter( m_r ) );
			}
		
		private:
			template <typename F>
			void for_each_ctor_parameter( const F& f ) const
			{
				using namespace boost::mpl::placeholders;	// for _1
		
				// this builds a compile time sequence containing the parameters types
				typedef typename boost::function_types::parameter_types<Constructor>::type constructor_parameter_types_t;
				// now we transform the sequence to wrap the parameters types in a type_wrapper
				// we do this so that boost::mpl::for_each won't try to instanciate parameters 
				// because it'll blow if one of the parameters types is not default constructible !
				typedef typename boost::mpl::transform<constructor_parameter_types_t, traits::wrapped_type<_1> >::type constructor_wrapped_parameter_types_t;
		
				// now step through the wrapped parameters types to print them out to the stream
				boost::mpl::for_each<constructor_wrapped_parameter_types_t>( f );
			}
		
			struct register_parameter
			{
				register_parameter( kindi::repository& r )
					: m_r( r )
				{
				}
		
				template <typename U>
				void operator()( type_wrapper<U> /*twu*/ )
				{
					// first, if U is a provider, get the provided type
					typedef typename traits::remove_provider<typename traits::basic_type<U>::type>::type provided_type;
					// register it
					m_r.declare_type<typename traits::basic_type<provided_type>::type>();
				}
				
				kindi::repository& m_r;
			};
			
		private:
			/**
			 * Reference to the types repository
			 */
			kindi::repository& m_r;
		};

		template <typename T, typename Constructor>
		abstract_base_provider* get_provider( repository& r,
		                                      T* instance,
		                                      boost::mpl::false_ /* has instance */ )
		{
			return new provider<T, Constructor>( r );
		}

		template <typename T, typename Constructor>
		abstract_base_provider* get_provider( repository& r,
		                                      T* instance,
		                                      boost::mpl::true_ /* has instance */ )
		{
			return new provider_with_instance<T>( instance );
		}
		
	} // ns detail
} // ns kindi

template <typename T>
void kindi::repository::declare_type()
{
	add( type<T>() );
}

template <typename T, typename BuildProperties>
void kindi::repository::add( const kindi::detail::build_info<T, BuildProperties>& rBuilder )
{
	type_info new_type_info = type_info( kindi::type_wrapper<T>() );

	// first we search the map to see if the type is already in the map of know types
	types_map_t::const_iterator it = m_mapTypes.find( new_type_info );
	if( it != m_mapTypes.end() )
		return;	// nothing more to do
	
	typedef typename detail::build_info<T, BuildProperties>::builtType_constructor_t constructor_t;
	
	// recursively register parameters types
	detail::recursive_registration<T, constructor_t> rr( *this );
	rr();
	
	// creates the provider for the new type
	abstract_base_provider* pProvider = detail::get_provider<T, constructor_t>( *this, rBuilder.m_instance, typename BuildProperties::has_instance() );
	
	// insert the provider for the new type
	m_mapTypes.insert( std::make_pair( new_type_info, pProvider ) );
	
	// insert a the provider for the provider of the new type
	// allows the user to ask for a provider<Type>
	type_info new_type_provider_info = type_info( kindi::type_wrapper<abstract_typed_provider<T> >() );
	m_mapTypes.insert( std::make_pair( new_type_provider_info, new provider_provider( pProvider ) ) );
}

template <typename T>
kindi::detail::build_info<T> kindi::repository::type()
{
	return detail::build_info<T>();
}

template <typename T>
T* kindi::repository::construct()
{
	// checks that the type is complete
	// issues a compilation error if it's not ( from boost::checked_delete )
	// we cannot build an incomplete type!
	typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
	(void) sizeof(type_must_be_complete);

	return get_provider<T>()->construct();
}

template <typename T>
kindi::abstract_typed_provider<T>* kindi::repository::get_provider()
{
	kindi::type_info searched_type = kindi::type_info( kindi::type_wrapper<T>() );

	types_map_t::iterator it = m_mapTypes.find( searched_type );
	if( it == m_mapTypes.end() )
	{
		throw std::runtime_error( (boost::format( "Unable to find type %s" ) % searched_type).str() );
	}

	return static_cast<abstract_typed_provider<T>*>( it->second.get() );
}
