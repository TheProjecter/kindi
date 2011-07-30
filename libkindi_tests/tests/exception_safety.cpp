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

#include "../test_friendly_new.hpp"
#include "../ExSTestClass.hpp"
#include "../ExSChecks.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/scoped_ptr.hpp>

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

	class FailingConcrete: public Abstract, public ExSTestClass
	{
	public:
		void doSmthing()
		{
			if( this == NULL )
				throw std::runtime_error( "this == NULL!" );
			m_n = 42;
		}
		virtual ~FailingConcrete()
		{
		};
		int m_n;
	};

	class FailingConcreteDep: public ExSTestClass
	{
	public:
		KINDI_CONSTRUCTOR( FailingConcreteDep, (FailingConcrete* p) )
		: m_p( p )
		{
		}
		virtual ~FailingConcreteDep()
		{
		};
		boost::shared_ptr<FailingConcrete> m_p;
	};

	class FailingConcreteProviderDep: public ExSTestClass
	{
	public:
		KINDI_CONSTRUCTOR( FailingConcreteProviderDep, (kindi::provider<FailingConcreteDep>* p) )
		: m_p( p->construct() )
		{
		}
		virtual ~FailingConcreteProviderDep()
		{
		};
		boost::shared_ptr<FailingConcreteDep> m_p;
	};
}


void addFailingConcrete( kindi::injector& inj )
{
	inj.add( kindi::type<FailingConcrete>() );
}
void constructFailingConcrete( kindi::injector& inj )
{
	boost::scoped_ptr<FailingConcrete> p1( inj.construct<FailingConcrete>() );
}
BOOST_AUTO_TEST_CASE( ex_safety_simple )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &addFailingConcrete ) );

	inj.add( kindi::type<FailingConcrete>() );
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &constructFailingConcrete ) );
}

void addFailingConcreteDep( kindi::injector& inj )
{
	inj.add( kindi::type<FailingConcreteDep>() );
}
void constructFailingConcreteDep( kindi::injector& inj )
{
	boost::scoped_ptr<FailingConcreteDep> p1( inj.construct<FailingConcreteDep>() );
}
BOOST_AUTO_TEST_CASE( ex_safety_simple_tree )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &addFailingConcreteDep ) );

	inj.add( kindi::type<FailingConcreteDep>() );
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &constructFailingConcreteDep ) );
}

BOOST_AUTO_TEST_CASE( ex_safety_instance )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	
	inj.add( kindi::type<FailingConcrete>().instance( new FailingConcrete() ) );
	
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &constructFailingConcrete ) );
}

void constructAbstract( kindi::injector& inj )
{
	Abstract* p1( inj.construct<Abstract>() );
	p1 = p1 && NULL;
}
BOOST_AUTO_TEST_CASE( ex_safety_implementation )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	
	inj.add( kindi::type<Abstract>().implementation<FailingConcrete>() );
	
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &constructAbstract ) );
}

void constructFailingConcreteProvider( kindi::injector& inj )
{
	kindi::provider<FailingConcrete>* p1( inj.construct<kindi::provider<FailingConcrete> >() );
	p1 = p1 && NULL;
}
BOOST_AUTO_TEST_CASE( ex_safety_provider )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	
	inj.add( kindi::type<FailingConcrete>() );
	
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &constructFailingConcreteProvider ) );
}

void addFailingConcreteProviderDep( kindi::injector& inj )
{
	inj.add( kindi::type<FailingConcreteProviderDep>() );
}
void constructFailingConcreteProviderDep( kindi::injector& inj )
{
	boost::scoped_ptr<FailingConcreteProviderDep> p1( inj.construct<FailingConcreteProviderDep>() );
}
BOOST_AUTO_TEST_CASE( ex_safety_provider_tree )
{
	kindi::injector inj;
	inj.add( kindi::type<Concrete>() );
	
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &addFailingConcreteProviderDep ) );

	inj.add( kindi::type<FailingConcreteProviderDep>() );
	BOOST_REQUIRE_NO_THROW( StrongCheck( inj, &constructFailingConcreteProviderDep ) );
}
