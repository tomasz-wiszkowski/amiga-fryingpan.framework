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


#ifndef _GENERIC_MUI_MUIPOPUP_H_
#define _GENERIC_MUI_MUIPOPUP_H_

#include <Generic/HookAttrT.h>
#include "MUI.h"
#include <Generic/HookT.h>
#include <Generic/String.h>

namespace GenNS
{
    class MUIPopup : public MUI 
    {
    protected:
	iptr	    *all;
	iptr	    *display;
	iptr	    *button;
	iptr	    *object;
	iptr	    *label;
	iptr	    *popup;
	String       name;

    protected:
	HookT<MUIPopup, void*, void*> hHkOpen;
	HookT<MUIPopup, void*, void*> hHkClose;
	HookAttrT<iptr, const void*>  hNotify;
	iptr                          id;

    private:
	virtual iptr	doOpen(void*, void*);
	virtual iptr	doClose(void*, void*);

    protected:
	virtual bool         onOpen()                         =  0;
	virtual bool         onClose()                        =  0;
	virtual iptr	    *getPopDisplay()                  =  0;
	virtual iptr	    *getPopButton()                   =  0;
	virtual iptr	    *getPopObject()                   =  0;

	virtual void         update();
    public:
	virtual void         setValue(const void* string)     =  0;
	virtual const void  *getValue()                       =  0;

	MUIPopup(const char* name);
	virtual             ~MUIPopup();
	virtual void         setID(iptr id);
	virtual void         setCallbackHook(const Hook*);
	virtual iptr	    *getObject();
	virtual void         open();
	virtual void         close();
    };
};

#endif

