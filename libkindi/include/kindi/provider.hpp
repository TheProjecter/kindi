// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <kindi/abstract_provider.hpp>

#include "kindi/detail/construction_helper.hpp"

namespace kindi
{
	template <typename ConstructedType, typename Constructor>
	class generic_provider : public provider<ConstructedType>
	{
	public:
		generic_provider( kindi::repository& r )
		:m_r( r )
		{
		}
		
		virtual ConstructedType* construct() const
		{
			return detail::construction_helper<ConstructedType, Constructor>()( m_r );
		}
		
	private:
		kindi::repository& m_r;
	};
	
	template <typename ConstructedType>
	class provider_with_instance : public provider<ConstructedType>
	{
	public:
		provider_with_instance( ConstructedType* instance )
		:m_instance( instance )
		{
		}
		
		virtual ConstructedType* construct() const
		{
			return m_instance;
		}
		
	private:
		ConstructedType* m_instance;
	};
	
	class provider_provider : public provider_with_instance<abstract_base_provider>
	{
	public:
		provider_provider( abstract_base_provider* pProvider )
		:provider_with_instance( pProvider )
		{}
		
		virtual ~provider_provider(){}
	};
}
