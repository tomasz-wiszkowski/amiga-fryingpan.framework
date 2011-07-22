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


#ifndef _GENERIC_MUI_MUIWINDOWMENU_H_
#define _GENERIC_MUI_MUIWINDOWMENU_H_

#include <Generic/GenericMUI.h>
#include <Generic/HookT.h>
#include <Generic/HookAttrT.h>

namespace GenNS
{
    class MUIWindowMenu : public GenericMUI
    {
	Object*                                pMain;
	Object*                                pMenu;
	Object*                                pRadio;
	HookAttrT<iptr, MUIWindowMenu*>        hCallback;
	HookT<MUIWindowMenu, iptr*, iptr*>     hLocal;

    protected:
	virtual iptr   callBack(iptr* arg1, iptr* arg2);
    public:
	MUIWindowMenu();
	virtual                ~MUIWindowMenu();

	virtual Object         *getObject();
	virtual void            addMenu(const char* tTitle);
	virtual void            addSeparator();
	virtual void            addItem(const char* tTitle, iptr lParam, const char *tShort = 0);
	virtual void            addChoice(const char* tTitle, bool bSelected, iptr lParam, const char *tShort = 0);
	virtual void            addRadio(const char* tTitle);
	virtual void            addRadioOption(const char* tTitle, bool bSelected, iptr lParam, const char* tShort = 0);
	virtual void            setHook(const Hook* pNotify);
    };
};

#endif

