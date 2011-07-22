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

#ifndef __GENERIC_CYCLEBUFFER_H
#define __GENERIC_CYCLEBUFFER_H

#include "Types.h"
#include "Synchronizer.h"

namespace GenNS
{
    class CycleBuffer : protected Synchronizer
    {
	void**	    bufs;	// send buffer (elements ready to use)
	void**	    bufr;	// receive buffer (elements with data)
	uint8	    sndx;	// sender index in array
	uint8	    rcvx;	// receiver index in array
	uint8	    sndsig;	// sender signal
	uint8	    rcvsig;	// receiver signal
	uint8	    maxx;	// max index
	uint8	    sndt;	// send buffer target index
	uint8	    rcvt;	// receive buffer target index
	uint8	    resvd2;
	struct Task*sender;	// sender task
	struct Task*recver;	// receiver task

    protected:
	void*	read(uint8& idx, uint8& tgt, void** arr, uint8 sig);
	void	write(uint8& tgt, void** arr, void* data, Task* t, uint8 sig);
    public:
	CycleBuffer(uint8 maxidx, iptr size);
	virtual ~CycleBuffer();

	virtual void	RegSender();	// register sender
	virtual void	RegReceiver();	// register receiver
	virtual void	UnregSender();	// unregister sender
	virtual void	UnregReceiver();// unregister receiver
	virtual void*	Alloc();	// alloc buffer (sender)
	virtual void	Send(void*);	// send buffer (sender)
	virtual void*	Receive();	// receive buffer (receiver)
	virtual void	Dispose(void*);	// dispose buffer (receiver)
    };
};

#endif

