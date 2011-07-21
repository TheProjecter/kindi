// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#ifdef DIF_DEBUG
#include <string>
#endif

namespace kindi
{
	/**
	 * abstract base class for providers
	 */
	class abstract_base_provider
	{
	public:
		/**
		 * Constructs an instance
		 */
		virtual void* construct_untyped() const = 0;

		#ifdef DIF_DEBUG
		//virtual std::string printDependencies() const = 0;
		#endif

		virtual ~abstract_base_provider(){}
	};

	/**
	 * abstract typed base class for providers
	 */
	template <typename ConstructedType>
	class abstract_typed_provider : public abstract_base_provider
	{
	public:
		/**
		 * Constructs an instance
		 */
		virtual void* construct_untyped() const
		{
			return construct();
		}
		
		/**
		 * Constructs an instance
		 * @return pointer to the new instance
		 */
		virtual ConstructedType* construct() const = 0;

		virtual ~abstract_typed_provider(){}
	};
} // ns kindi

