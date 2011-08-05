// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/type_wrapper.hpp"

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>

namespace kindi
{
	namespace traits
	{
		BOOST_MPL_HAS_XXX_TRAIT_DEF( kindi_construction_descriptor )

		template <typename T>
		struct nested_type_decl
		{
			typedef typename T::kindi_construction_descriptor::type_decl type;
		};

		template <typename T>
		struct type_decl
		{
			typedef typename boost::mpl::if_<
				has_kindi_construction_descriptor<T>,
				nested_type_decl<T>,
				type_wrapper<T>
			>::type::type type;
		};

		template <typename T>
		struct nested_constructor
		{
			typedef typename T::kindi_construction_descriptor::constructor_decl type;
		};

		template <typename T>
		struct constructor
		{
			typedef typename boost::mpl::if_<
				has_kindi_construction_descriptor<T>,
				nested_constructor<T>,
				type_wrapper<void()>
			>::type::type type;
		};
	} // ns traits
} // ns kindi
