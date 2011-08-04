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

	class ComposedType
	{
	public:
		KINDI_CONSTRUCTOR( ComposedType, (boost::shared_ptr<Concrete> pC) )
			: m_pC( pC )
		{
		}

		void doSmthing()
		{
			if( this == NULL )
				throw std::runtime_error( "this == NULL!" );
			m_pC->doSmthing();
		}

	private:
		boost::shared_ptr<Concrete> m_pC;
	};

	class TypeDependentOnComposedTypeProvider
	{
	public:
		KINDI_CONSTRUCTOR( TypeDependentOnComposedTypeProvider, (boost::shared_ptr<kindi::provider<ComposedType> > pC) )
			: 	m_pC( pC ),
				m_p1( m_pC->construct() ),
				m_p2( m_pC->construct() )
		{
		}

		void doSmthing()
		{
			if( this == NULL )
				throw std::runtime_error( "this == NULL!" );
			m_p1->doSmthing();
			m_p2->doSmthing();
		}

		boost::shared_ptr<kindi::provider<ComposedType> > m_pC;
		boost::shared_ptr<ComposedType> m_p1;
		boost::shared_ptr<ComposedType> m_p2;
	};
}

BOOST_AUTO_TEST_CASE( provider_for_concrete )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	boost::shared_ptr<kindi::provider<Concrete> > p = inj.construct<kindi::provider<Concrete> >();

	boost::shared_ptr<Abstract> p1 = p->construct();
	boost::shared_ptr<Abstract> p2 = p->construct();

	BOOST_REQUIRE( p1 != p2 );
	p1->doSmthing();
	p2->doSmthing();
}

BOOST_AUTO_TEST_CASE( provider_for_concrete_with_instance )
{
	kindi::injector inj;
	boost::shared_ptr<Concrete> pConcreteInstance( new Concrete() );
	inj.add( kindi::type<Concrete>().instance( pConcreteInstance ) );
	boost::shared_ptr<kindi::provider<Concrete> > p = inj.construct<kindi::provider<Concrete> >();

	boost::shared_ptr<Abstract> p1 = p->construct();
	boost::shared_ptr<Abstract> p2 = p->construct();

	BOOST_REQUIRE( p1 == p2 );
	BOOST_REQUIRE( p2 == pConcreteInstance );
	p1->doSmthing();
	p2->doSmthing();
}

BOOST_AUTO_TEST_CASE( provider_for_composed )
{
	kindi::injector inj;
	inj.add( kindi::type<ComposedType>() );
	boost::shared_ptr<kindi::provider<ComposedType> > p = inj.construct<kindi::provider<ComposedType> >();

	boost::shared_ptr<ComposedType> p1 = p->construct();
	boost::shared_ptr<ComposedType> p2 = p->construct();

	BOOST_REQUIRE( p1 != p2 );
	p1->doSmthing();
	p2->doSmthing();
}

BOOST_AUTO_TEST_CASE( provider_injection )
{
	kindi::injector inj;
	inj.add( kindi::type<TypeDependentOnComposedTypeProvider>() );
	boost::shared_ptr<TypeDependentOnComposedTypeProvider> p = inj.construct<TypeDependentOnComposedTypeProvider>();

	BOOST_REQUIRE( p->m_p1 != p->m_p2 );
	p->doSmthing();
}

