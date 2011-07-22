/*
 * FryingPan - Amiga CD/DVD Recording Software (User Intnerface and supporting Libraries only)
 * Copyright (C) 2001-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com
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

#include "MUIDynamicCycle.h"
#include <libclass/intuition.h>
#include <libraries/mui.h>

using namespace GenNS;

MUIDynamicCycle::MUIDynamicCycle()
{
   hHkBtnHook.Initialize(this, &MUIDynamicCycle::btnHook);
   setButtonCallBack(hHkBtnHook.GetHook());
}

bool MUIDynamicCycle::vecDeleteName(const char* const& name)
{
    delete [] const_cast<char*>(name);
    return true;
}

iptr MUIDynamicCycle::btnHook(iptr id, void* data)
{
    switch (id)
    {
	case ID_Cycle:
	    hook(this->id, (iptr)data);
	    break;
    }
    return 0;
}
   
const char* MUIDynamicCycle::dupstr(const char* str)
{
   char* dst = new char[strlen(str)+1];
   strcpy(dst, str);
   return dst;
}

void MUIDynamicCycle::rebuildCycle(const VectorT<const char*> *nm)
{
   String s;

   DoMtd(cyclegroup, ARRAY(MUIM_Group_InitChange));
   DoMtd(cyclegroup, ARRAY(OM_REMMEMBER, (int)cycle));

   this->names.ForEach(&vecDeleteName).Empty();

   if (nm != 0)
       for (uint32 i=0; i<nm->Count(); i++)
	   names << dupstr((*nm)[i]);

   if (names.Count() == 0)
      names << dupstr("---");

   names << 0;
   
   cycle = muiCycle(const_cast<const char**>(names.GetArray()), 0, ID_Cycle, 0);
   DoMtd(cyclegroup, ARRAY(OM_ADDMEMBER, (int)cycle));
   DoMtd(cyclegroup, ARRAY(MUIM_Group_ExitChange));
}

MUIDynamicCycle::~MUIDynamicCycle()
{
   names.ForEach(&vecDeleteName).Empty();
}

iptr *MUIDynamicCycle::getObject()
{
    cycle = 0;
    cyclegroup = VGroup, End;

    rebuildCycle(0);

    return cyclegroup;
}

void MUIDynamicCycle::setElements(VectorT<const char*> *vec)
{
    rebuildCycle(vec);
}

void MUIDynamicCycle::setCallbackHook(const Hook *hk)
{
   hook = hk;
}

void MUIDynamicCycle::setID(int32 i)
{
   id = i;
}

