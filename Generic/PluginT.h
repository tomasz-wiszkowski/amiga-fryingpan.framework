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

#ifndef _GENERIC_MODULET_H
#define _GENERIC_MODULET_H

#include "Module.h"

namespace GenNS
{
    template<typename T>
    class PluginT : public Module
    {
    public:
	/*
	 * MOD_Header always has to be first
	 */
	enum Plugin_Magic
	{
	    Plug_Magic1  = 'GNRC',
	    Plug_Magic2  = 'PLG\0'
	};

	struct PluginHeader 
	{
	    Module::Header		Header;
	    const char*			ModuleName;
	    const char*			VersionString;
	    T*				(*Init)();
	    void			(*Exit)(T*);
	    const struct TagItem*	Tags;
	};

    protected:
	T*	module;

    public:
	PluginT(const char* fname, uint16 ver, uint16 rev) : 
	    Module(fname, Plug_Magic1, Plug_Magic2, ver, rev)
	{
	    module = 0;

	    if (IsValid())
		module = ((PluginHeader*)header)->Init();
		
	    /*
	     * if false, don't bother to compare
	     */
	    valid &= (0 != module);
	}

	~PluginT()
	{
	    if (0 != module)
	    {
		((PluginHeader*)header)->Exit(module);
		module = 0;
	    }
	}

	T* Class()
	{
	    return module;
	}

	const char* VerStr()
	{
	    if (!IsValid())
		return 0;

	    return ((PluginHeader*)header)->VersionString;
	}

	const char* Name()
	{
	    if (!IsValid())
		return 0;

	    return ((PluginHeader*)header)->ModuleName;
	}
    };
};

#endif
