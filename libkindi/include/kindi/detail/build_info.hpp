// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include "kindi/detail/build_properties.hpp"

#include "kindi/traits/constructor.hpp"
#include "kindi/traits/wrapped_type.hpp"

#include "kindi/detail/debug_template.hpp"

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/function_types/function_arity.hpp>

namespace kindi
{
	namespace detail
	{
		/**
		 * Default properties.
		 */
		template <typename Implementation>
		struct default_build_properties : build_properties<boost::mpl::false_, Implementation>{};
		
		/**
		 * Struct holding information needed to build type T and it's dependencies.
		 */
		template <typename T, typename BuildProperties = default_build_properties<T> >
		class build_info
		{
			/**
			 * self type
			 */
			typedef build_info<T, BuildProperties> self_type;

			/**
			 * Type that is built by this Builder
			 */
			typedef T builtType_t;

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
			 * modifies the properties of this build_info type (produces a new type).
			 * for the sake of saving a few characters!
			 * see BuildProperties::new_properties for the gory details
			 */
			template <typename BuildInfo, typename NewPropertyTag, typename NewPropertyType>
			struct new_binfo
			{
				typedef typename BuildProperties::template new_properties<NewPropertyTag, NewPropertyType>::type new_properties_t;
				typedef build_info<typename BuildInfo::builtType_t, new_properties_t> type;
			};
			
			/**
			 * sets an instance for the type
			 * @param pInstance instance
			 * @return a new build_info object with the correct instance
			 */
			typename new_binfo<self_type, has_instance_tag, boost::mpl::true_>::type instance( T* pInstance ) const
			{
				typename new_binfo<self_type, has_instance_tag, boost::mpl::true_>::type b( *this );
				b.m_instance = pInstance;
				return b;
			}
			
			/**
			 * sets an implementation for the type
			 * @return a new build_info object with the correct implementation
			 */
			template <typename Implementation>
			typename new_binfo<self_type, implementation_tag, Implementation>::type implementation() const
			{
				return typename new_binfo<self_type, implementation_tag, Implementation>::type( *this );
			}
			
			/**
			 * pointer to the instance if any (see provider_with_instance)
			 */
			T* m_instance;
		};
	} // ns detail
} // ns kindi
