// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

class ExSTestClass
{
public:

	ExSTestClass();

	ExSTestClass( const ExSTestClass& rhs );

	const ExSTestClass& operator=( const ExSTestClass& rhs );

	bool operator==( const ExSTestClass& rhs ) const;

	~ExSTestClass();
};
