// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "test_friendly_new.hpp"

#include <new>
#include <cstdlib>
#include <cstring>

#include "possible_failure.hpp"
#include "allocationtracker.hpp"

bool enable_failing_new::s_failing_new = false;

void* operator new( std::size_t size ) throw( std::bad_alloc )
{
	while( true )
	{
		if( enable_failing_new::s_failing_new == true )
			failure::simulatePossibleFailureT<std::bad_alloc>();
		
		void* result = std::malloc( size );
		std::memset( result, 0, size );
		if( result )
		{
			allocation_tracker::alloc( result, size );
			return result;
		}

		std::new_handler nh = std::set_new_handler( 0 );
		std::set_new_handler( nh );  // put it back
		// this is clumsy, I know, but there's no portable way to query the current
		// new handler without replacing it
		// you don't have to use new handlers if you don't want to

		if( !nh )
			throw std::bad_alloc();
		nh();
	}
	return (void*)1;    // avoids a warning
}

void operator delete( void* ptr ) throw()
{
	if( ptr )
	{
		std::free( ptr );
		allocation_tracker::dealloc( ptr );
	}
}

void* operator new[]( std::size_t size ) throw( std::bad_alloc )
{
	return operator new( size );  // defer to non-array version
}

void operator delete[]( void* ptr ) throw()
{
	operator delete( ptr );  // defer to non-array version
}
