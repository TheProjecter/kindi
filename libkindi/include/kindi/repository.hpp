// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/abstract_provider.hpp"
#include "kindi/detail/build_info.hpp"

#include "kindi/type_info.hpp"
#include "kindi/traits/wrapped_type.hpp"

#include <map>
#include <vector>
#include <stdexcept>

#include <boost/shared_ptr.hpp>

// debug includes
#ifdef DIF_DEBUG
#include <string>
#include <sstream>
#include <boost/format.hpp>
#endif

namespace kindi
{
	class repository
	{
	public:
		/**
		 * declares a new type in the type repository
		 * @deprecated
		 */
		template <typename T>
		void declare_type();
		
		/**
		 * adds a new build_info in the type repository
		 */
		template <typename T, typename BuildProperties>
		void add( const detail::build_info<T, BuildProperties>& rBuilder );
		
		/**
		 * create a structure containing all the necessary information to instanciate a type.
		 * next thing to do is to register it on the repository
		 * @return the construction info
		 */
		template <typename T>
		detail::build_info<T> type();
		
		/**
		 * construct an instance of a type
		 * @return pointer to the new instance
		 */
		template <typename T>
		T* construct();

	private:
		/**
		 * get the provider associated to this type
		 * @return a pointer to the build_info
		 * @throw std::runtime_exception if the type could not be found
		 */
		template <typename T>
		provider<T>* get_provider();

	private:
		typedef std::map<type_info, boost::shared_ptr<abstract_base_provider> > types_map_t;
		types_map_t m_mapTypes;
	};
}

