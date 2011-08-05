// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "type_wrapper.hpp"

#define KINDI_CONSTRUCTION_DESCRIPTOR( TypeName, Signature )					\
	class kindi_construction_descriptor											\
	{																			\
	public:																		\
		typedef kindi::type_wrapper< TypeName >::type type_decl;				\
		typedef kindi::type_wrapper< void Signature >::type constructor_decl;	\
	};																			\
	
#define KINDI_CONSTRUCTOR( TypeName, Signature )								\
	KINDI_CONSTRUCTION_DESCRIPTOR( TypeName, Signature )						\
	TypeName Signature
