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

#include "MUINumericClass.h"
#include <libclass/intuition.h>
#include <libclass/muimaster.h>
#include <libraries/mui.h>
#include <libclass/utility.h>

using namespace GenNS;

   MUICustomClassT<MUINumericClass> *MUINumericClass::muiclass = 0;

MUINumericClass::MUINumericClass(IClass *cls)
{
    parent   = cls;
}

MUINumericClass::~MUINumericClass()
{
}

iptr MUINumericClass::DoMtd(iptr *obj, iptr *msg)
{
    iptr k;
    switch (msg[0]) 
    {
	case OM_NEW:
	    if (!(obj = (Object*)DoSuperMtd(parent, obj, msg))) 
		return 0;

            k = (iptr)Utility->GetTagData(MUIA_Numeric_StrHook, 0, (TagItem*)msg[1]);
	    stringify = (const Hook*)k;
	    return (iptr)obj;
	    break;

	case OM_DISPOSE:
	case MUIM_Hide:
	    break;

	case OM_SET:
	    break;

	case MUIM_NoNotifySet:
	case MUIM_Set:
	    break;

	case MUIM_Numeric_Stringify:
	    if (stringify.IsValid())
	    {
		return stringify.Call(this, msg[1]);
	    }
	    break;
    }
    return DoSuperMtd(parent, obj, msg);
}

MUICustomClass* MUINumericClass::CreateClass()
{
    if (MUINumericClass::muiclass == 0)
    {
	MUINumericClass::muiclass = new MUICustomClassT<MUINumericClass>(MUIC_Slider);
    }
    return MUINumericClass::muiclass;
}

void MUINumericClass::DestroyClass()
{
    MUICustomClassT<MUINumericClass> *p = MUINumericClass::muiclass;
    MUINumericClass::muiclass = 0;
    delete p;
}

