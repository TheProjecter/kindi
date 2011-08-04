// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <boost/shared_ptr.hpp>

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
		virtual boost::shared_ptr<void> construct_untyped() const = 0;

		virtual ~abstract_base_provider(){}
	};

	/**
	 * abstract typed base class for providers
	 */
	template <typename ConstructedType>
	class provider : public abstract_base_provider
	{
	public:
		/**
		 * Constructs an instance
		 */
		virtual boost::shared_ptr<void> construct_untyped() const
		{
			return construct();
		}
		
		/**
		 * Constructs an instance
		 * @return pointer to the new instance
		 */
		virtual boost::shared_ptr<ConstructedType> construct() const = 0;

		virtual ~provider(){}
	};
} // ns kindi

