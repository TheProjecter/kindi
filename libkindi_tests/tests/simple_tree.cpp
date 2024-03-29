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
	class DefaultConstructed
	{
	public:
		void doSmthing()
		{
			if( this == NULL )
				throw std::runtime_error( "this == NULL!" );
			m_n = 42;
		}
		int m_n;
	};

	class OneParamConstructor
	{
	public:
		KINDI_CONSTRUCTOR( OneParamConstructor, (boost::shared_ptr<DefaultConstructed> pDefaultConstructed) )
			: m_p( pDefaultConstructed )
		{
		}
		boost::shared_ptr<DefaultConstructed> m_p;
	};

	class TwoParamConstructor
	{
	public:
		KINDI_CONSTRUCTOR( TwoParamConstructor,
		                   (boost::shared_ptr<DefaultConstructed> pDefaultConstructed1, boost::shared_ptr<DefaultConstructed> pDefaultConstructed2) )
			: 	m_p1( pDefaultConstructed1 ),
				m_p2( pDefaultConstructed2 )
		{
		}
		boost::shared_ptr<DefaultConstructed> m_p1;
		boost::shared_ptr<DefaultConstructed> m_p2;
	};

	class TwoNonTrivialParamConstructor
	{
	public:
		KINDI_CONSTRUCTOR( TwoNonTrivialParamConstructor, (boost::shared_ptr<TwoParamConstructor> pTwoParamConstructor1, boost::shared_ptr<TwoParamConstructor> pTwoParamConstructor2) )
			: 	m_p1( pTwoParamConstructor1 ),
				m_p2( pTwoParamConstructor2 )
		{
		}
		boost::shared_ptr<TwoParamConstructor> m_p1;
		boost::shared_ptr<TwoParamConstructor> m_p2;
	};

	class OneParamConstructorNonPod
	{
	public:
		KINDI_CONSTRUCTOR( OneParamConstructorNonPod, (boost::shared_ptr<DefaultConstructed> pDefaultConstructed) )
			: m_p( pDefaultConstructed )
		{
		}
		~OneParamConstructorNonPod()
		{
		}
		boost::shared_ptr<DefaultConstructed> m_p;
	};

	class TwoNonTrivialParamConstructorNonPod
	{
	public:
		KINDI_CONSTRUCTOR( TwoNonTrivialParamConstructorNonPod, (boost::shared_ptr<OneParamConstructorNonPod> pOneParamConstructorNonPod, boost::shared_ptr<TwoParamConstructor> pTwoParamConstructor) )
			: 	m_p1( pOneParamConstructorNonPod ),
				m_p2( pTwoParamConstructor )
		{
		}
		~TwoNonTrivialParamConstructorNonPod()
		{
		}
		boost::shared_ptr<OneParamConstructorNonPod> m_p1;
		boost::shared_ptr<TwoParamConstructor> m_p2;
	};
}
BOOST_AUTO_TEST_CASE( default_constructor )
{
	kindi::injector inj;
	inj.add( kindi::type<DefaultConstructed>() );
	boost::shared_ptr<DefaultConstructed> p = inj.construct<DefaultConstructed>();
	BOOST_REQUIRE( p != NULL );
	p->doSmthing();
}

BOOST_AUTO_TEST_CASE( one_parameter_constructor )
{
	kindi::injector inj;
	inj.add( kindi::type<OneParamConstructor>() );
	boost::shared_ptr<OneParamConstructor> p = inj.construct<OneParamConstructor>();
	BOOST_REQUIRE( p != NULL );
	BOOST_REQUIRE( p->m_p != NULL );
	p->m_p->doSmthing();
}

BOOST_AUTO_TEST_CASE( two_parameter_constructor )
{
	kindi::injector inj;
	inj.add( kindi::type<TwoParamConstructor>() );
	boost::shared_ptr<TwoParamConstructor> p = inj.construct<TwoParamConstructor>();
	BOOST_REQUIRE( p != NULL );
	BOOST_REQUIRE( p->m_p1 != NULL );
	p->m_p1->doSmthing();
	BOOST_REQUIRE( p->m_p2 != NULL );
	p->m_p2->doSmthing();
}

BOOST_AUTO_TEST_CASE( two_non_trivial_parameter_constructor )
{
	kindi::injector inj;
	inj.add( kindi::type<TwoNonTrivialParamConstructor>() );
	boost::shared_ptr<TwoNonTrivialParamConstructor> p = inj.construct<TwoNonTrivialParamConstructor>();
	BOOST_REQUIRE( p != NULL );
	BOOST_REQUIRE( p->m_p1 != NULL );
	BOOST_REQUIRE( p->m_p1->m_p1 != NULL );
	p->m_p1->m_p1->doSmthing();
	BOOST_REQUIRE( p->m_p1->m_p2 != NULL );
	p->m_p1->m_p2->doSmthing();
	BOOST_REQUIRE( p->m_p2 != NULL );
	BOOST_REQUIRE( p->m_p2->m_p1 != NULL );
	p->m_p2->m_p1->doSmthing();
	BOOST_REQUIRE( p->m_p2->m_p2 != NULL );
	p->m_p2->m_p2->doSmthing();
}

BOOST_AUTO_TEST_CASE( two_non_trivial_parameter_constructor_non_pod )
{
	kindi::injector inj;
	inj.add( kindi::type<TwoNonTrivialParamConstructorNonPod>() );
	boost::shared_ptr<TwoNonTrivialParamConstructorNonPod> p = inj.construct<TwoNonTrivialParamConstructorNonPod>();
	BOOST_REQUIRE( p != NULL );
	BOOST_REQUIRE( p->m_p1 != NULL );
	BOOST_REQUIRE( p->m_p1->m_p != NULL );
	p->m_p1->m_p->doSmthing();
	BOOST_REQUIRE( p->m_p2 != NULL );
	BOOST_REQUIRE( p->m_p2->m_p1 != NULL );
	p->m_p2->m_p1->doSmthing();
	BOOST_REQUIRE( p->m_p2->m_p2 != NULL );
	p->m_p2->m_p2->doSmthing();
}

