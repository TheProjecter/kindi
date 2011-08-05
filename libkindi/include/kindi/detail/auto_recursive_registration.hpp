// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/detail/repository.hpp"

#include "kindi/traits/remove_provider.hpp"
#include "kindi/traits/basic_type.hpp"

#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/is_same.hpp>

namespace kindi
{
	namespace detail
	{
		template <typename T, typename BuildProperties>
		class auto_recursive_registration
		{
		public:
			auto_recursive_registration( kindi::detail::repository& r )
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
			void registerBoundType( F registrationFunctor, boost::true_type /* bound type is same as self */ ) const
			{
				// do nothing
				// we don't want to register it since we're already doing that
			}
			template <typename F>
			void registerBoundType( F registrationFunctor, boost::false_type /* bound type is different from self */ ) const
			{
				// we might need to register it if it's unknown
				registrationFunctor( type_wrapper<typename BuildProperties::implementation>() );
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

	} // ns detail
} // ns kindi
