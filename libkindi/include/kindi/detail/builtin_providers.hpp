// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <kindi/provider.hpp>

#include "kindi/detail/construction_helper.hpp"

namespace kindi
{
	namespace detail
	{
		template <typename ConstructedType, typename Implementation>
		class generic_provider : public provider<ConstructedType>
		{
			boost::shared_ptr<ConstructedType> construct_impl( boost::true_type /* type _is not_ bound to an other */ ) const
			{
				return  boost::shared_ptr<ConstructedType>( 
					detail::construction_helper<Implementation, typename traits::constructor<Implementation>::type >()( m_r ) );
			}
			boost::shared_ptr<ConstructedType> construct_impl( boost::false_type /* type _is_ bound to an other */ ) const
			{
				return m_r.get_provider<Implementation>()->construct();
			}
			
		public:
			generic_provider( kindi::detail::repository& r )
			:m_r( r )
			{
			}
			
			virtual boost::shared_ptr<ConstructedType> construct() const
			{
				return construct_impl( boost::is_same<ConstructedType, Implementation>() );
			}
			
		private:
			kindi::detail::repository& m_r;
		};
		
		template <typename ConstructedType>
		class provider_with_instance : public provider<ConstructedType>
		{
		public:
			provider_with_instance( boost::shared_ptr<ConstructedType> instance )
			:m_instance( instance )
			{
			}
			
			virtual boost::shared_ptr<ConstructedType> construct() const
			{
				return m_instance;
			}
			
			virtual ~provider_with_instance(){}
			
		private:
			boost::shared_ptr<ConstructedType> m_instance;
		};
		
		class provider_provider : public provider_with_instance<abstract_base_provider>
		{
		public:
			provider_provider( boost::shared_ptr<abstract_base_provider> pProvider )
			:provider_with_instance( pProvider )
			{}
			
			virtual ~provider_provider(){}
		};
	} // ns detail
} // ns kindi
