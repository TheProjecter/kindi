// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/detail/build_info.hpp"

namespace kindi
{
	/**
	 * create a structure containing all the necessary information to instantiate a type.
	 * next thing to do is to register it on the repository
	 * @return the construction info
	 */
	template <typename T>
	detail::build_info<T> type()
	{
		return detail::build_info<T>();
	}
}
