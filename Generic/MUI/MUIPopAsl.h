/*
 * Amiga Generic Set - set of libraries and includes to ease sw development for all Amiga platforms
 * Copyright (C) 2004-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef _GENERIC_MUI_MUIPOPASL_H_
#define _GENERIC_MUI_MUIPOPASL_H_

#include "MUIPopup.h"
#include "MUI.h"
#include <Generic/HookT.h>
#include <Generic/HookAttrT.h>
#include <Generic/String.h>

namespace GenNS
{
    class MUIPopAsl : public MUIPopup
    {
    public:
	enum PopType
	{
	    Pop_File,
	    Pop_Dir
	};

    protected:
	enum
	{
	    ID_String      =  100
	};

    protected:
	char                             value[1024];
	HookAttrT<void*, void*>          hCallback;
	PopType                          type;

    protected:
	HookT<MUIPopAsl, iptr, void*>           hHkBtnHook;

    protected:
	virtual bool                     onOpen();
	virtual bool                     onClose();
	virtual iptr			*getPopDisplay();
	virtual iptr			*getPopButton();
	virtual iptr			*getPopObject();
	virtual iptr			 buttonHandler(iptr id, void* data);

    public:
	MUIPopAsl(const char* name, PopType type = Pop_File);
	virtual                         ~MUIPopAsl();                                    
	virtual void                     setValue(const void* string);
	virtual const void              *getValue();
    };
};

#endif

