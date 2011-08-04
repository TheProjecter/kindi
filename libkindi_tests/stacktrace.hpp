// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

// karlphillip
// http://stackoverflow.com/questions/3151779/how-its-better-to-invoke-gdb-from-program-to-print-its-stacktrace/4611112#4611112

#pragma once

#include <string>

/**
 * get the current stack trace
 * @param frames_to_skip skip the given number of stack frames
 * @return a string object containing the stack trace
 */
std::string stacktrace( unsigned int frames_to_skip );
