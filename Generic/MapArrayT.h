/*
 * Amiga Generic Set - set of libraries and includes to ease sw development for all Amiga platforms
 * Copyright (C) 2004-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GENERIC_MAPARRAYT_H
#define _GENERIC_MAPARRAYT_H

#include "MapArray.h"
#include <LibC/LibC.h>

/**
 * \file MapArrayT.h
 * \brief (Relatively) fast bitmap array that will allocate index for your item (unsynchronized)
 */

namespace GenNS
{
    template <typename T>
    class MapArrayT
    {
    protected:
	MapArray    map;
    public:
	/*
	** \brief Create new array of size \b maxlen.
	** also checks if types fit in each other
	*/
	MapArrayT(int32 maxlen) :
	    map(maxlen)
	{
	    ASSERTS(sizeof(T) <= sizeof(void*), "Item size must NOT be greater than storage size");
	}

	virtual ~MapArrayT()
	{
	}

	virtual T operator[](int32 idx) const
	{
	    return (T)map.operator[](idx);
	}

	virtual T GetItem(int32 idx) const
	{
	    return (T)map.GetItem(idx);
	}

	int32 PutItem(T item)
	{
	    return map.PutItem((void*)item);
	}

	T FreeItem(int32 idx)
	{
	    return (T)map.FreeItem(idx);
	}

	bool AllocNextFreeItem(int32& idx, T*& ptr)
	{
	    return map.AllocNextFreeItem(idx, (void**&)ptr);
	}

	uint32 Max() const
	{
	    return map.Max();
	}

	uint32 Count() const
	{
	    return map.Count();
	}

	bool IsAllocated(int32 idx) const
	{
	    return map.IsAllocated(idx);
	}
    };
};

#endif

