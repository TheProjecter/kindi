// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/traits/wrapped_type.hpp"
#include "kindi/traits/basic_type.hpp"
#include "kindi/detail/type_info.hpp"
#include "kindi/detail/repository.hpp"

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>

#include <boost/format.hpp>

#include <boost/mpl/int.hpp>

#include <stdexcept>

namespace kindi
{
	namespace detail
	{
		/**
		 * Constructor helpers, there is one overload for each value of N (number of args)
		 */
		template <typename T, typename F, int N>
		struct construction_helper_impl
		{
			T* operator()( kindi::detail::repository& /*r*/ )
			{
				throw std::runtime_error( (boost::format( "Too much arguments in constructor of %s" ) % type_info( typename traits::wrapped_type<T>::type() ) ).str() );
			}
		};
		
		template <typename T, typename F>
		struct construction_helper_impl<T, F, 0>
		{
			T* operator()( kindi::detail::repository& /*r*/ )
			{
				return new T();
			}
		};
		
		template <typename T, typename F>
		struct construction_helper_impl<T, F, 1>
		{
			T* operator()( kindi::detail::repository& r )
			{
				typedef boost::function_types::parameter_types<F> params;
				
				typedef typename boost::mpl::at<params, boost::mpl::int_<0> >::type param0_t;
				typedef typename traits::basic_type<param0_t>::type param0_basic_t;
				
				param0_basic_t* pParam0 = static_cast<param0_basic_t*>( r.construct<param0_basic_t>() );
				
				return new T( pParam0 );
			}
		};
		
		template <typename T, typename F>
		struct construction_helper_impl<T, F, 2>
		{
			T* operator()( kindi::detail::repository& r )
			{
				typedef boost::function_types::parameter_types<F> params;
				
				typedef typename boost::mpl::at<params, boost::mpl::int_<0> >::type param0_t;
				typedef typename traits::basic_type<param0_t>::type param0_basic_t;
				typedef typename boost::mpl::at<params, boost::mpl::int_<1> >::type param1_t;
				typedef typename traits::basic_type<param1_t>::type param1_basic_t;
				
				param0_basic_t* pParam0 = static_cast<param0_basic_t*>( r.construct<param0_basic_t>() );
				param1_basic_t* pParam1 = static_cast<param1_basic_t*>( r.construct<param1_basic_t>() );
				
				return new T( pParam0, pParam1 );
			}
		};
		
		//TODO: implémenter les autres nb de parametres
		
		template <typename T, typename F>
		struct construction_helper
		{
			T* operator()( kindi::detail::repository& r )
			{
				typedef boost::function_types::function_arity<F> arity;
				
				return construction_helper_impl<T, F, arity::value>()( r );
			}
		};

		
	} // ns detail
} // ns kindi
