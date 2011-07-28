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
#include "kindi/detail/build_info.hpp"
#include "kindi/detail/builtin_providers.hpp"

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
		template <typename T, typename BuildProperties>
		class recursive_registration
		{
		public:
			recursive_registration( kindi::detail::repository& r )
				: m_r( r )
			{
			}
			
			void operator()()
			{
				register_type f( m_r );
				
				// register each parameter type
				for_each_ctor_parameter( f );
				// register bound type
				// do not register if bound type is equal to self
				registerBoundType( f, boost::is_same<T, typename BuildProperties::implementation>() );
			}
		
		private:
			template <typename F>
			void registerBoundType( F f, boost::true_type ) const
			{
				//do nothing
			}
			template <typename F>
			void registerBoundType( F f, boost::false_type ) const
			{
				f( type_wrapper<typename BuildProperties::implementation>() );
			}
			
			template <typename F>
			void for_each_ctor_parameter( const F& f ) const
			{
				using namespace boost::mpl::placeholders;	// for _1
		
				typedef typename traits::constructor<T>::type constructor_t;
				
				// this builds a compile time sequence containing the parameters types
				typedef typename boost::function_types::parameter_types<constructor_t>::type constructor_parameter_types_t;
				// now we transform the sequence to wrap the parameters types in a type_wrapper
				// we do this so that boost::mpl::for_each won't try to instanciate parameters 
				// because it'll blow if one of the parameters types is not default constructible !
				typedef typename boost::mpl::transform<constructor_parameter_types_t, traits::wrapped_type<_1> >::type constructor_wrapped_parameter_types_t;
		
				// now step through the wrapped parameters types to print them out to the stream
				boost::mpl::for_each<constructor_wrapped_parameter_types_t>( f );
			}
		
			struct register_type
			{
				register_type( kindi::detail::repository& r )
					: m_r( r )
				{
				}
		
				template <typename U>
				void operator()( type_wrapper<U> /*twu*/ )
				{
					// first, if U is a provider, get the provided type
					typedef typename traits::remove_provider<typename traits::basic_type<U>::type>::type provided_type;
					// register it
					m_r.declare_type_if_unknown<typename traits::basic_type<provided_type>::type>();
				}
				
				kindi::detail::repository& m_r;
			};
			
		private:
			/**
			 * Reference to the types repository
			 */
			kindi::detail::repository& m_r;
		};

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
	detail::recursive_registration<T, BuildProperties>( *this )();
	
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