// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

namespace kindi
{
	namespace traits
	{
		/**
		 * if we're trying to declare a provider<T>, we want to declare just T.
		 * provider<T> will be automatically declared to the types repository
		 */
		template <typename T>
		struct remove_provider
		{
			typedef T type;
		};
		
		/**
		 * specialisation of remove_provider for a provider type
		 */
		template <typename T>
		struct remove_provider<kindi::provider<T> >
		{
			typedef T type;
		};
	} // ns traits
} // ns kindi
