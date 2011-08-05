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

namespace kindi
{
	namespace traits
	{
		/**
		 * Wraps a type in a type_wrapper
		 */
		template <typename T>
		struct wrapped_type
		{
			typedef type_wrapper<T> type;
		};
	}
}
