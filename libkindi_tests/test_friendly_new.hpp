// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <new>

#define ENABLE_FAILING_NEW enable_failing_new efn_8f2ecb40ba3011e0962b0800200c9a66;

struct enable_failing_new
{
	enable_failing_new()
	{
		s_failing_new = true;
	}
	~enable_failing_new()
	{
		s_failing_new = false;
	}
	
	static bool s_failing_new; // maybe it should be elsewhere
};

void* operator new(std::size_t size) throw(std::bad_alloc) ;

void operator delete(void* ptr) throw();

void* operator new[](std::size_t size) throw(std::bad_alloc);

void operator delete[](void* ptr) throw();
