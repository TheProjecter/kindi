// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/provider.hpp"
#include "kindi/detail/build_info.hpp"

#include "kindi/detail/type_info.hpp"
#include "kindi/traits/wrapped_type.hpp"

#include <map>
#include <vector>
#include <stdexcept>

#include <boost/shared_ptr.hpp>

namespace kindi
{
	namespace detail
	{
		class repository
		{
		public:
			/**
			 * Constructor
			 */
			repository()
			:m_referent( this )
			{
			}
			
			/**
			 * copy constructor
			 * @param rhs right hand side
			 */
			repository( const repository& rhs )
			:m_mapTypes( rhs.m_mapTypes ),
			 m_referent( rhs.m_referent )
			{
			}
			
			/**
			 * assignement operator
			 * @param rhs right hand side, by value
			 * @return
			 */
			repository& operator =( repository rhs )
			{
				swap( rhs );
				return *this;
			}
			
			/**
			 * declares a new type in the type repository
			 * @deprecated
			 */
			template <typename T>
			void declare_type_if_unknown();
			
			/**
			 * adds a new build_info in the type repository
			 */
			template <typename T, typename BuildProperties>
			void add( const detail::build_info<T, BuildProperties>& rBuilder );
			
			/**
			 * construct an instance of a type
			 * @return pointer to the new instance
			 */
			template <typename T>
			boost::shared_ptr<T> construct();

			/**
			 * get the provider associated to this type
			 * @return a pointer to the build_info
			 * @throw std::runtime_error if the type could not be found
			 */
			template <typename T>
			provider<T>* get_provider();

			/**
			 * comparison operator.
			 * for tests purposes
			 * @param rhs right hand side
			 * @return true if the repositories are equal (aka contain the same types)
			 */
			bool operator ==( const repository& rhs )
			{
				return m_mapTypes == rhs.m_mapTypes;
			}
			
		private:
			template <typename T, typename BuildProperties>
			void add_if_unknown( const kindi::detail::build_info<T, BuildProperties>& build_info );

			 void swap( repository& rhs )
			 {
				 std::swap( m_referent, rhs.m_referent );
				 std::swap( m_mapTypes, rhs.m_mapTypes );
			 }
			 
		private:
			typedef std::map<type_info, boost::shared_ptr<abstract_base_provider> > types_map_t;
			types_map_t m_mapTypes;
			
			/**
			 * pointer to the real repository.
			 * it is used for classes which needs a pointer back to the repository
			 * avoids giving them a pointer to a temporary
			 */
			repository* m_referent;
		};
	}
}

