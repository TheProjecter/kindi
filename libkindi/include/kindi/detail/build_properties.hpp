// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

namespace kindi
{
	namespace detail
	{
		// properties tags
		struct has_instance_tag{};
		struct implementation_tag{};
		
		/**
		 * build properties.
		 * Holds compile time properties relevant for building a type
		 */
		template <typename HasInstance, typename Implementation>
		struct build_properties
		{
			typedef build_properties<HasInstance, Implementation> self_type;
			
			
			typedef HasInstance has_instance;
			typedef Implementation implementation;
			
			/**
			 * struct used to easily declare a new build_info<T,BuildProperties> variant with mutated BuildProperties.
			 * follows overloads for each property.
			 * think of it has a compile time setter.
			 */
			template <typename NewPropertyTag, typename NewPropertyType>
			struct new_properties
			{
				typedef self_type type;
			};

			/**
			 * setter for build_properties::has_instance.
			 * see generic new_properties 
			 */
			template <typename NewPropertyType>
			struct new_properties<has_instance_tag, NewPropertyType>
			{
				typedef build_properties<NewPropertyType, typename self_type::implementation> type;
			};

			/**
			 * setter for build_properties::implementation.
			 * see generic new_properties 
			 */
			template <typename NewPropertyType>
			struct new_properties<implementation_tag, NewPropertyType>
			{
				typedef build_properties<typename self_type::has_instance, NewPropertyType> type;
			};
		};
	} // ns detail
} // ns kindi
