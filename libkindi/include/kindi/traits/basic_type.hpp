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
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace kindi
{
	namespace traits
	{
		template <typename T>
		struct remove_shared_ptr
		{
			typedef T type;
		};
		template <typename T>
		struct remove_shared_ptr<boost::shared_ptr<T> >
		{
			typedef T type;
		};
		
		template <typename T>
		struct basic_type
		{
			typedef typename remove_shared_ptr<T>::type no_shptr;
			typedef typename boost::remove_pointer<no_shptr>::type no_ptr;
			typedef typename boost::remove_cv<no_ptr>::type no_cv;
			typedef typename boost::remove_reference<no_cv>::type type;
		};
	}
}
