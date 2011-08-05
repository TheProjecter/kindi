// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

// original code by David Abrahams

#pragma once

#include "possible_failure.hpp"
#include "allocationtracker.hpp"

#include <boost/format.hpp>

/*===================================================================================
 CheckInvariant
 
 EFFECTS:  Generalized function to check an invariant on a container. Specialize
 this for particular types if such a check is available.
 ====================================================================================*/
template <class T>
void CheckInvariant( const T& )
{
}

template <class Value, class Operation>
void PrintingStrongCheck( const Value& v, const Operation& op )
{
	try
	{
		StrongCheck( v, op );
	}
	catch( std::runtime_error& ex )
	{
		std::cerr << ex.what() << "\n";
		throw;
	};
}

/**
 * checks for the strong guarantie
 * @param v tested type. must implement the operator == to check it wasn't modified after an exception
 * @param op operation to perform on v
 */
template <class Value, class Operation>
void StrongCheck( const Value& v, const Operation& op )
{
	bool succeeded = false;
	for( long nextThrowCount = 0; !succeeded; ++nextThrowCount )
	{
		Value duplicate = v;
		try
		{
			failure::set_throw_counter( nextThrowCount );
			{
				ENABLE_FAILURES;
				ENABLE_FAILING_NEW;
				ENABLE_LEAK_DETECTION;
				op( duplicate ); // Try the operation
			}
			succeeded = true;
		}
		catch( ... ) // Catch all exceptions 
		{
			// test strong guarantee 
			bool unchanged = duplicate == v;
			if( !unchanged )
				throw( boost::format( "strong guaranty check failed" ) );

			// test no memory was leaked
			allocation_tracker::inspect();
		}
		// Specialize as desired for each container type, to check 
		// integrity. For example, size() == distance(begin(),end()) 
		CheckInvariant( v ); // Check any invariant 
	}
}
