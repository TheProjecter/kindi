// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

// inherited class
#include "kindi/traits/constructor.hpp"
#include "kindi/traits/wrapped_type.hpp"

#include "kindi/detail/debug_template.hpp"

#include <boost/mpl/if.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/function_types/function_arity.hpp>

#ifdef DIF_DEBUG
#include <string>
#include <sstream>
#endif

namespace kindi
{
	namespace detail
	{
		//tags
		struct has_instance_tag{};
		
		/**
		 * build properties.
		 * Holds compile time properties relevant for building a type
		 */
		template <typename HasInstance>
		struct build_properties
		{
			typedef HasInstance has_instance;
		};
		/**
		 * Default properties.
		 */
		struct default_build_properties : build_properties<boost::mpl::false_>{};
		
		/**
		 * Struct holding information needed to build type T and it's dependencies.
		 */
		template <typename T, typename BuildProperties = default_build_properties>
		class build_info
		{
			/**
			 * type used to check that the constructor declaration
			 * is given for the type and not inherited.
			 * avoirs problems like this :
			 * class A{ KINDI_CONSTRUCTOR( A, ( B* b) ) {} };
			 * class C : public A {};
			 * where C as a constructor declared with 1 parameter, but it is class A's constructor
			 * we don't want that to happen
			 */
		    class test_constructor_decl;

		public:
			build_info()
			: m_instance( NULL )
			{
			    // cheks that the constructor declaration was defined for this
			    //  particular type and not inherited
			    // maybe there is a simpler way to issue a compilation error here?
			    typedef typename boost::mpl::if_<
			              boost::is_same<T, typename traits::type_decl<T>::type>,
			            char,
			            test_constructor_decl
			            >::type missing_constructor_declaration_for_derived_type_cond1;
			    // now we discard the error if the inherited class has a default constructor
			    typedef typename boost::mpl::if_<
			    		boost::is_same<
			    			boost::mpl::int_<0>,
			    			boost::mpl::int_<boost::function_types::function_arity<builtType_constructor_t>::value> >,
			    		char,
			    		missing_constructor_declaration_for_derived_type_cond1
			    		>::type missing_constructor_declaration_for_derived_type;
			    // if you see an error here you likely have forgotten to declare the constructor
			    // in your class T (see compiler error to find out what T is)
			    (void) sizeof(missing_constructor_declaration_for_derived_type);
			}
			
			/**
			 * templated conversion constructor.
			 * allows building from arbitraty build_info<*> types
			 */
			template< typename OtherT, typename OtherBuildProperties>
			build_info( const build_info<OtherT, OtherBuildProperties>& rhs )
			: m_instance( rhs.m_instance )
			{
			}
			

			/**
			 * struct used to easily declare a new build_info<T,BuildProperties> variant with mutated BuildProperties.
			 * follows overloads for each property.
			 * think of it has a compile time setter.
			 */
			template <typename NewPropertyTag, typename NewPropertyType>
			struct new_properties
			{
				typedef build_info<T> type;
			};

			/**
			 * setter for build_properties::has_instance.
			 * see generic new_properties 
			 */
			template <typename NewPropertyType>
			struct new_properties<has_instance_tag, NewPropertyType>
			{
				typedef build_info<T, build_properties<NewPropertyType> > type;
			};
			
			/**
			 * sets an instance for the type
			 * @param pInstance instance
			 * @return a new build_info object with the correct instance
			 */
			typename new_properties<has_instance_tag, boost::mpl::true_>::type instance( T* pInstance ) const
			{
				typename new_properties<has_instance_tag, boost::mpl::true_>::type b( *this );
				b.m_instance = pInstance;
				return b;
			}

			/**
			 * Type that is built by this Builder
			 */
			typedef type_wrapper<T> builtType_t;

			/**
			 * constructor's type
			 * if there is no constructor defaults to 'void( void )'
			 */
			typedef typename traits::constructor<T>::type builtType_constructor_t;
			
			/**
			 * build properties
			 */
			typedef BuildProperties build_properties_t;
			
			/**
			 * pointer to the instance if any (see provider_with_instance)
			 */
			T* m_instance;
		};
	} // ns detail
} // ns kindi
