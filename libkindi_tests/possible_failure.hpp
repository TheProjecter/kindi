// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <limits>
#include <iostream>

#define ENABLE_FAILURES enable_failures ef_8f2ecb40ba3011e0962b0800200c9a66;

struct enable_failures
{
	enable_failures()
	{
		s_failures_enabled = true;
	}
	~enable_failures()
	{
		s_failures_enabled = false;
	}
	
	static bool s_failures_enabled; // maybe it should be elsewhere
};

#define DISABLE_AND_RESTORE_FAILURES disable_and_restore_failures drf_8f2ecb40ba3011e0962b0800200c9a66;

struct disable_and_restore_failures
{
	disable_and_restore_failures()
	{
		m_saved_state = enable_failures::s_failures_enabled;
		enable_failures::s_failures_enabled = false;
	}
	~disable_and_restore_failures()
	{
		enable_failures::s_failures_enabled = m_saved_state;
	}

	bool m_saved_state;
};

class failure
{
public:
	template <typename Exception>
	static void simulatePossibleFailureT( Exception ex = Exception() )
	{
		if( enable_failures::s_failures_enabled )
		{
			if( s_throw_counter-- == 0 ) // there is 2 ifs for debuging purposes
			{
				//std::cout << "failed once!\n";
				throw Exception();
			}
		}
	}
	
	static void simulatePossibleFailure()
	{
		simulatePossibleFailureT<int>( 42 );
	}
	
	static void set_throw_counter( unsigned long value )
	{
		s_throw_counter = value;
	}
	
	static unsigned long get_throw_counter()
	{
		return s_throw_counter;
	}
private:
	static unsigned long s_throw_counter;
};
