// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include <kindi/constructor.hpp>
#include <kindi/injector.hpp>

#include <boost/test/unit_test.hpp>

#include "../test_friendly_new.hpp"

namespace
{
	class Abstract
	{
	public:
		virtual void doSmthing() = 0;
		virtual ~Abstract()
		{
		};
	};

	class Concrete: public Abstract
	{
	public:
		void doSmthing()
		{
			if( this == NULL )
				throw std::runtime_error( "this == NULL!" );
			m_n = 42;
		}
		virtual ~Concrete()
		{
		};
		int m_n;
	};
}
BOOST_AUTO_TEST_CASE( instance_of_concrete )
{
	kindi::injector inj;
	boost::shared_ptr<Concrete> pConcreteInstance( new Concrete() );
	inj.add( kindi::type<Concrete>().instance( pConcreteInstance ) );
	boost::shared_ptr<Abstract> p1 = inj.construct<Concrete>();
	boost::shared_ptr<Abstract> p2 = inj.construct<Concrete>();
	BOOST_REQUIRE( p1 == p2 );
	BOOST_REQUIRE( p2 == pConcreteInstance );
	p1->doSmthing();
	p2->doSmthing();
}

BOOST_AUTO_TEST_CASE( instance_of_abstract )
{
	kindi::injector inj;
	boost::shared_ptr<Concrete> pConcreteInstance( new Concrete() );
	inj.add( kindi::type<Abstract>().instance( pConcreteInstance ) );
	boost::shared_ptr<Abstract> p1 = inj.construct<Abstract>();
	boost::shared_ptr<Abstract> p2 = inj.construct<Abstract>();
	BOOST_REQUIRE( p1 == p2 );
	BOOST_REQUIRE( p2 == pConcreteInstance );
	p1->doSmthing();
	p2->doSmthing();
}
