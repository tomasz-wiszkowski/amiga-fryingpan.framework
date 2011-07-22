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

#ifndef _GENERIC_MAPARRAY_H
#define _GENERIC_MAPARRAY_H

#include "Types.h"

/**
 * \file MapArray.h
 * \brief (Relatively) fast bitmap array that will allocate index for your item (unsynchronized)
 */

namespace GenNS
{
    class MapArray
    {
    protected:
	int32	len;
	uint32*	bits;
	void**	array;

    protected:
	/*
	** \brief recalculate relative array index to an absolute one
	** in other words, negative array index is still okay, just counts
	** from the end of an array (reverse order).
	** \returns false if the index is outside bounds, or true otherwise.
	*/
	bool    IdxRelToAbs(int32 &idx) const;

    public:
	/*
	** \brief Create new array of size \b maxlen.
	** parameter must be greater than 0 and will be rounded up to nearest multiply of 32.
	*/
	MapArray(int32 maxlen);

	/*
	** \brief Destroy array
	*/
	virtual ~MapArray();

	/*
	** \brief Index operator for an array; returns storage/value at index \b idx
	*/
	virtual void* operator[](int32 idx) const;

	/*
	** \brief Return an item present at index \b idx
	*/
	virtual void* GetItem(int32 idx) const;

	/*
	** \brief Put a new item in an array. This will automatically allocate a new index for your item.
	** return value of -1 suggests you ran out of items.
	*/
	virtual int32 PutItem(void* item);

	/*
	** \brief Dispose item at location \b idx; this will clear allocation bit and return 
	** value stored under this location.
	*/
	virtual void* FreeItem(int32 idx);
	
	/*
	** \brief Allocate item. Return index and address via reference
	** useful if you need to know index before actually putting anything
	** returns false if array is full
	*/
	bool	AllocNextFreeItem(int32& idx, void**& ptr);

	/*
	** \brief Return allocation status of index \b idx
	*/
	bool IsAllocated(int32 idx) const;

	/*
	** \brief Return max array length
	*/
	uint32 Max() const;

	/*
	** \brief Return number of allocated elements
	*/
	uint32 Count() const;
    };
};

#endif
