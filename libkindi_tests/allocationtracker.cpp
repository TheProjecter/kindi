// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#include "allocationtracker.hpp"

#include "possible_failure.hpp"
#include "stacktrace.hpp"

#include <boost/format.hpp>
#include <stdexcept>


#define DISABLE_AND_RESTORE_LEAK_DETECTION disable_and_restore_leak_detection drld_8f2ecb40ba3011e0962b0800200c9a66;

struct allocation_tracker::disable_and_restore_leak_detection
{
	disable_and_restore_leak_detection()
	{
		m_saved_state = allocation_tracker::s_leak_detection;
		allocation_tracker::s_leak_detection = false;
	}
	~disable_and_restore_leak_detection()
	{
		allocation_tracker::s_leak_detection = m_saved_state;
	}

	bool m_saved_state;
};

bool allocation_tracker::s_leak_detection = false;


void allocation_tracker::start()
{
	s_leak_detection = true;
	s_allocation_maps().clear();
}

void allocation_tracker::alloc( void* addr, std::size_t size )
{
	if( !s_leak_detection )
		return;
	
	DISABLE_AND_RESTORE_FAILURES;
	DISABLE_AND_RESTORE_LEAK_DETECTION;
	
	allocation_info ai;
	ai.size = size;
	ai.bt = stacktrace( 3 );
	s_allocation_maps()[addr] = ai;
}

void allocation_tracker::dealloc( void* addr ) throw()
{
	if( !s_leak_detection )
		return;
	
	DISABLE_AND_RESTORE_LEAK_DETECTION;

	allocations_map_t::iterator it = s_allocation_maps().find( addr );
	if( it != s_allocation_maps().end() )
		s_allocation_maps().erase( it );
}

void allocation_tracker::inspect()
{
	DISABLE_AND_RESTORE_LEAK_DETECTION;

	if( s_allocation_maps().size() > 0 )
	{
		void* addr = s_allocation_maps().begin()->first;
		std::size_t size = s_allocation_maps().begin()->second.size;
		std::string bt = s_allocation_maps().begin()->second.bt;
		throw std::runtime_error( (boost::format("leak detected a memory address: %x [size:%d]\n%s") % addr % size % bt).str() );
	}
}

void allocation_tracker::stop()
{
	s_leak_detection = false;
}

allocation_tracker::allocations_map_t& allocation_tracker::s_allocation_maps()
{
	static allocations_map_t allocations_map = allocations_map_t();
	return allocations_map;
}
