// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "stacktrace.hpp"


#if defined(__GNUC__)

#include <execinfo.h>
#include <stdio.h>

// there is some serious ugliness involved in this function
std::string stacktrace( unsigned int frames_to_skip )
{
	std::string str;

	void* stack_addrs[50];
	int trace_size = backtrace( stack_addrs, 50 );
	char** stack_strings = backtrace_symbols( stack_addrs, trace_size );
	
	// skip frames_to_skip stack frames 
	str += "[bt] backtrace:\n";
	for( int i = frames_to_skip; i < trace_size; ++i )
	{
		// for some super weird reason, using a stringstream or boost::format
		// makes the leak disappear !
		char tmp[4096];
		sprintf( tmp, "[bt] #%d %s\n", i-frames_to_skip, stack_strings[i] );
		str += tmp;
	}
	
	free( stack_strings );

	return str;
}

#else // #if defined(__GNUC__)
std::string stacktrace( unsigned int frames_to_skip )
{
	return "stack trace only available with gcc";
}
#endif