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

#ifndef SYNCT_H_
#define SYNCT_H_

#include "Synchronizer.h"

namespace GenNS
{
    template <typename T>
	class SyncT 
	{
	    T                 resource;
	    Synchronizer      pSync;
	public:

	    SyncT()
	    {
	    }

	    virtual          ~SyncT()
	    {
	    }

	    virtual T        &Attempt(bool *bResult)
	    {
		if ((pSync.Attempt()) && (0 != bResult))
		    *bResult = true;
		else if (0 != bResult)      
		    *bResult = false;
		return resource;
	    }

	    virtual T        &Acquire()
	    {
		pSync.Acquire();
		return resource;
	    }

	    virtual void      Release()
	    {
		pSync.Release();
	    }

	    virtual T         Assign(T newValue)
	    {
		T oldValue;
		oldValue = Acquire();
		resource = newValue;
		Release();    
		return oldValue;  
	    }

	    virtual T         AttemptAssign(T newValue, bool *bResult)
	    {
		T oldValue;
		bool bRes;
		oldValue = Attempt(&bRes);
		if (bRes)
		{
		    resource = newValue;
		    Release();
		    if (bResult)
			*bResult = true;
		    return oldValue;
		}
		if (bResult)
		    *bResult = false;
		return resource;
	    }

	    SyncT<T>       &operator = (SyncT<T> &hOther)
	    {
		/*
		 ** a major flaw here is the fact that
		 ** two different semaphores guard same resource
		 */
		pSync.Acquire();		// make sure nobody accesses us atm

		resource = hOther.resource;    // copy elements, dont bother how.

		pSync.Release();               // this is the obtained one :)
		return *this;
	    }


	};
};

#endif /*SYNCT_H_*/
