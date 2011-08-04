// ***************************************************************************
//  (C) Copyright Sébastien Débia 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://code.google.com/p/kindi/ for the library home page.
// ***************************************************************************

#pragma once

#include <cstddef>
#include <string>
#include <map>

#define ENABLE_LEAK_DETECTION enable_leak_detection eld_8f2ecb40ba3011e0962b0800200c9a66;

/**
 * 
 */
class allocation_tracker
{
public:
	
	/**
	 * starts recording allocations/deallocations.
	 * reset previously recorded stats.
	 * this is donne in start() and not stop() to allow inspecting 
	 * results after stopping leak detection
	 */
	static void start();
	
	/**
	 * track a new allocation
	 * @param addr address of the allocated memory
	 * @param size size of the allocated memory
	 */
	static void alloc( void* addr, std::size_t size );
	
	/**
	 * track a deallocation
	 * @param addr address of the allocated memory
	 */
	static void dealloc( void* addr ) throw();
	
	/**
	 * check that all the allocated memory has been freed.
	 * @throw std::runtime_error if some memory is leaked
	 */
	static void inspect();
	
	/**
	 * stops recording allocations/deallocations
	 */
	static void stop();
	
private:
	static bool s_leak_detection;
	
	struct disable_and_restore_leak_detection;
	friend struct disable_and_restore_leak_detection;
	
	struct allocation_info
	{
		std::size_t size;
		std::string bt;
	};
	typedef std::map<void*, allocation_info> allocations_map_t;
	static allocations_map_t& s_allocation_maps();
};

struct enable_leak_detection
{
	enable_leak_detection()
	{
		allocation_tracker::start();
	}
	~enable_leak_detection()
	{
		allocation_tracker::stop();
	}
};
