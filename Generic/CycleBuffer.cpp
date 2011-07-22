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
#include "CycleBuffer.h"
#include <libclass/exec.h>
#include <Generic/Debug.h>

using namespace GenNS;

CycleBuffer::CycleBuffer(uint8 maxidx, iptr itemsize):
    Synchronizer()
{
    ASSERT(maxidx != 0);
    ASSERT(itemsize != 0);
    sndx = 0;
    rcvx = 0;
    sndt = 0;
    rcvt = 0;
    maxx = maxidx + 1;
    sndsig = 0;
    rcvsig = 0;
    sender = 0;
    recver = 0;
    bufs = new void*[maxx];
    bufr = new void*[maxx];
    for (sndt=0; sndt<maxidx; ++sndt)
	bufs[sndt] = new uint8[itemsize],
	bufr[sndt] = 0;

    /*
     * this is on purpose. max array len is maxidx + 1
     * this eases calculations
     */
    bufs[sndt] = 0;
    bufr[sndt] = 0;
}

CycleBuffer::~CycleBuffer()
{
    ASSERT(sender == 0);
    ASSERT(recver == 0);

    for (;rcvx != rcvt; rcvx = (rcvx + 1) % maxx)
	delete [] (uint8*)bufr[rcvx];
    for (;sndx != sndt; sndx = (sndx + 1) % maxx)
	delete [] (uint8*)bufs[sndx];

    delete [] bufs;
    delete [] bufr;

}

void CycleBuffer::RegSender()
{
    sndsig = Exec->AllocSignal(-1);
    sender = Exec->FindTask(0);
}

void CycleBuffer::UnregSender()
{
    Exec->FreeSignal(sndsig);
    sndsig = 0;
    sender = 0;
}

void CycleBuffer::RegReceiver()
{
    rcvsig = Exec->AllocSignal(-1);
    recver = Exec->FindTask(0);
}

void CycleBuffer::UnregReceiver()
{
    Exec->FreeSignal(rcvsig);
    rcvsig = 0;
    recver = 0;
}

void* CycleBuffer::read(uint8& idx, uint8& tgt, void** arr, uint8 sig)
{
    void* buf = 0;
    

    while(true)
    {
	Synchronizer::Acquire(); 
	if (idx != tgt)
	{
	    buf = arr[idx];
	    idx = (idx + 1) % maxx;
	}
	Synchronizer::Release();
	if (buf)
	    break;
	Exec->Wait(1 << sig);
    }

    /*
     * clear pending signals
     */
    Exec->SetSignal(0, 1 << sig);
    return buf;
}

void CycleBuffer::write(uint8& tgt, void** arr, void* data, Task* t, uint8 sig)
{
    /*
     * we should be able to send at all times
     */
    Synchronizer::Acquire();

    arr[tgt] = data;
    tgt = (tgt + 1) % maxx;

    Synchronizer::Release();

    /*
     * tell receiver there's something for it
     */
    if (t)
	Exec->Signal(t, 1<<sig);
}

void* CycleBuffer::Alloc()
{
    return read(sndx, sndt, bufs, sndsig);
}
	
void CycleBuffer::Send(void* m)
{
    write(rcvt, bufr, m, recver, rcvsig);
}

void* CycleBuffer::Receive()
{
    return read(rcvx, rcvt, bufr, rcvsig);
}

void CycleBuffer::Dispose(void* b)
{
    write(sndt, bufs, b, sender, sndsig);
}

