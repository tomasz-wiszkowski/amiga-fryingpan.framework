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


#ifndef _GENERIC_MUI_MUINUMERICCLASS_H_
#define _GENERIC_MUI_MUINUMERICCLASS_H_ 

#include <Generic/GenericOOP.h>
#include <Generic/String.h>
#include <Generic/MUICustomClassT.h>
#include <Generic/HookAttrT.h>


namespace GenNS
{
    class MUINumericClass : public GenericOOP
    {
    public:
	enum
	{
	    MUIA_Numeric_StrHook    = 0x120000,
	};

    private:
	static MUICustomClassT<MUINumericClass> *MUINumericClass::muiclass;

    protected:
	IClass*				    parent;
	HookAttrT<MUINumericClass*, iptr>   stringify;
	String				    value;

    public:
	MUINumericClass(IClass* parent);
	virtual        ~MUINumericClass();                  
	virtual iptr	DoMtd(iptr *obj, iptr *msg);

	static MUICustomClass* CreateClass();
	static void	DestroyClass();
    };
};

#endif

