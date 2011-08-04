// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "stacktrace.hpp"

#include <execinfo.h>

#include <sstream>
#include <boost/format.hpp>

#include <stdio.h>

// there is some serious ugliness involved in this function
std::string stacktrace( unsigned int frames_to_skip )
{
	void* trace[50];
	char** messages = NULL;
	int trace_size = 0;
	
	std::stringstream ss;
	std::string str;

	trace_size = backtrace( trace, 50 );
	messages = backtrace_symbols( trace, trace_size );
	
	// skip frames_to_skip stack frames 
	str += "[bt] backtrace:\n";
	for( int i = frames_to_skip; i < trace_size; ++i )
	{
		// for some super weird reason, using a stringstream or boost::format
		// makes the leak disappear !
		char tmp[500];
		sprintf( tmp, "[bt] #%d %s\n", i-frames_to_skip, messages[i] );
		str += tmp;
	}
	
	free( messages );

	return str;
}
