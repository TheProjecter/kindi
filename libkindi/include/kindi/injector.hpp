// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/type.hpp"
#include "kindi/detail/repository_impl.hpp"

namespace kindi
{
	class injector
	{
	public:
		injector()
		{
		}
		
		template <typename T, typename BuildProperties>
		void add( const detail::build_info<T, BuildProperties>& rBuilder )
		{
			m_r.add( rBuilder );
		}

		template <typename T>
		T* construct()
		{
			return m_r.construct<T>();
		}
		
		bool operator ==( const injector& rhs )
		{
			return m_r == rhs.m_r;	
		}

	private:
		
	private:
		detail::repository m_r;
	};
}
