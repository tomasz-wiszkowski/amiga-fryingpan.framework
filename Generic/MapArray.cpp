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

#include "MapArray.h"
#include "Debug.h"
#include <LibC/LibC.h>

using namespace GenNS;
	
bool MapArray::IdxRelToAbs(int32 &idx) const
{
    /*
    ** TODO: this is called way too often.
    */
    ASSERTS((idx < len) && ((-idx) < len), "Array index out of bounds");

    if (idx < 0)
	idx = len - idx;

    if (idx < 0)
	return false;
    if (idx >= len)
	return false;

    return true;
}

MapArray::MapArray(int32 maxlen)
{
    len = maxlen;

    ASSERTS (len > 0, "Array length must be greater than 0!");
    if (len <= 0)
	len = 32;
    len = (len + 31) & (~31);
    bits = new uint32[len >> 5];
    array = new void*[len];

    for (maxlen = 0; maxlen < (len >> 5); maxlen++)
	bits[maxlen] = 0;

    for (maxlen = 0; maxlen < len; maxlen++)
	array[maxlen] = 0;
}

MapArray::~MapArray()
{
    delete [] array;
    delete [] bits;
}

void* MapArray::operator[](int32 idx) const
{
    if (!IdxRelToAbs(idx))
	return 0;

    if (IsAllocated(idx))
	return array[idx];
    return 0;
}

void* MapArray::GetItem(int32 idx) const
{
    return (*this)[idx];
}

int32 MapArray::PutItem(void* item)
{
    int32 idx = -1;
    void** dat;
    if (AllocNextFreeItem(idx, dat))
	*dat = item;
    return idx;
}

void* MapArray::FreeItem(int32 idx)
{
    if (IsAllocated(idx))
    {
	/*
	** it's okay to address array directly here
	** keep in mind that we need to keep track of
	** invalid array accesses
	*/
	void* ret = array[idx];
	bits[idx >> 5] &= ~(1 << (idx & 31));
	array[idx] = 0;
	return ret;
    }

    return 0;
}

bool MapArray::AllocNextFreeItem(int32& idx, void**& ptr)
{
    int i;
    int x;
    int sh=16;
    int m;
    int b=0;

    for (i=0; i<(len>>5); i++)
    {
	if (bits[i] != ~0ul)
	    break;
    }

    ASSERTS(i < (len >> 5), "No free store to add elements!");
    if (i == (len >> 5))
	return false;

    x = bits[i];

    while (sh != 0)
    {
	/*
	** calc mask, 0xffff, 0xff, 0xf, 0x5, 0x1
	*/
	m = (1 << sh) - 1;

	/*
	** check which part to verify
	** update bit number
	*/
	if ((x & m) == m)
	    b += sh, x >>= sh;
	sh >>= 1;
    }

    bits[i] |= (1 << b);
    idx = (i << 5) | b;
    ptr = &array[idx];
    return true;
}

bool MapArray::IsAllocated(int32 idx) const
{
    if (!IdxRelToAbs(idx))
	return false;

    return (0 != (bits[idx >> 5] & (1 << (idx & 31))));
}

uint32 MapArray::Max() const
{
    return len;
}

uint32 MapArray::Count() const
{
    int cnt = 0;
    int t;
    int i;

    for (i = 0; i < (len >> 5); i++)
    {
	t = bits[i];
	t = ((t & 0xaaaaaaaa) >> 1) + (t & 0x55555555);
	t = ((t & 0xcccccccc) >> 2) + (t & 0x33333333);
	t = ((t & 0xf0f0f0f0) >> 4) + (t & 0x0f0f0f0f);
	t = ((t & 0xff00ff00) >> 8) + (t & 0x00ff00ff);
	t = ((t & 0xffff0000) >> 16) + (t & 0x0000ffff);
	cnt += t;
    }

    return cnt;
}
