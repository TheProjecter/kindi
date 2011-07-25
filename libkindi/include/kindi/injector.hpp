// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/repository_impl.hpp"

#ifdef DIF_DEBUG
#include <string>
#endif

namespace kindi
{
	class injector
	{
	public:
		injector()
		{
		}
		
		template <typename T>
		void add( const detail::build_info<T>& rBuilder )
		{
			m_r.add( rBuilder );
		}

		template <typename T>
		void declare_type()
		{
			m_r.declare_type<T>();
		}

		template <typename T>
		T* construct()
		{
			return m_r.construct<T>();
		}

	private:
		
	private:
		repository m_r;
	};
}
