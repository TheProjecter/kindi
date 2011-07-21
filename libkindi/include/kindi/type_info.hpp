// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <typeinfo>
#include <string>
#include <iosfwd>

#include "type_wrapper.hpp"

namespace kindi
{
	/**
	 * Type information
	 */
	struct type_info
	{
		template <typename T>
		type_info( type_wrapper<T> );

		type_info();

		bool operator<( type_info const & rhs ) const;

		bool operator==( type_info const & rhs ) const;

		bool operator!=( type_info const & rhs ) const;

		std::string name() const;

	private:
		const std::type_info *m_info;
	};

	template <typename T>
	type_info::type_info( type_wrapper<T> )
		: m_info( &typeid(typename type_wrapper<T>::type) )
	{
	}

	std::ostream&
	operator<<( std::ostream & os, const type_info & );
} // ns kindi
