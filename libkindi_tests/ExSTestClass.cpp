// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "ExSTestClass.hpp"

#include "possible_failure.hpp"

ExSTestClass::ExSTestClass()
{
	failure::simulatePossibleFailure();
}

ExSTestClass::ExSTestClass( const ExSTestClass & rhs )
{
	failure::simulatePossibleFailure();
}

const ExSTestClass & ExSTestClass::operator =( const ExSTestClass & rhs )
{
	failure::simulatePossibleFailure();
	return *this;
}

bool ExSTestClass::operator ==( const ExSTestClass & rhs ) const
								{
	failure::simulatePossibleFailure();
	return true;
}

ExSTestClass::~ExSTestClass()
{
}

