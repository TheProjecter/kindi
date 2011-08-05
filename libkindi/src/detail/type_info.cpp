// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 0 $
// ***************************************************************************

#include "kindi/detail/type_info.hpp"
#include "kindi/detail/demangle.hpp"
#include <ostream>

bool kindi::detail::type_info::operator<( type_info const & rhs ) const
								{
	return m_info->before( *rhs.m_info ) ? true : false;
}

bool kindi::detail::type_info::operator==( type_info const & rhs ) const
									{
	return !( *this < rhs ) && !( rhs < *this );
}

bool kindi::detail::type_info::operator!=( type_info const & rhs ) const
									{
	return !( *this == rhs );
}

std::string kindi::detail::type_info::name() const
{
	return m_info->name();
}

kindi::detail::type_info::type_info()
	: m_info( &typeid(kindi::type_wrapper<void>::type) )
{
}

std::ostream& kindi::detail::operator<<( std::ostream & os, const type_info & info )
{
	return os << detail::demangle( info.name() );
}
