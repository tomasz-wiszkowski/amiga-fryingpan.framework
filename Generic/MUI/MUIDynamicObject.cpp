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

#include "MUIDynamicObject.h"
#include <libclass/intuition.h>
#include <libraries/mui.h>

using namespace GenNS;

MUIDynamicObject::MUIDynamicObject(bool hsiz, bool vsiz)
{
    this->all = 0;
    this->updatable = 0;
    this->currentMember = 0;
    this->defaultMember = 0;
    this->hsizable = hsiz;
    this->vsizable = vsiz;
}

MUIDynamicObject::~MUIDynamicObject()
{
    resetChild();
}

void MUIDynamicObject::setChild(iptr* obj)
{
    if (currentMember == obj)
	return;

    DoMtd(updatable, ARRAY(MUIM_Group_InitChange));
    DoMtd(updatable, ARRAY(OM_REMMEMBER, (iptr)currentMember));
    DoMtd(updatable, ARRAY(OM_ADDMEMBER, (iptr)obj));
    DoMtd(updatable, ARRAY(MUIM_Group_ExitChange));

    currentMember = obj;
}

iptr *MUIDynamicObject::getObject()
{
    if (all != 0)
	return all;

    updatable = VGroup, End;
    all = updatable;

    defaultMember = muiLabel("");
    currentMember = defaultMember;
    DoMtd(updatable, ARRAY(OM_ADDMEMBER, (iptr)defaultMember));

    if (hsizable)
    {
	all = HGroup,
	    Child,	muiSpace(1),
	    Child,	all,
	    Child,	muiSpace(1),
	End;
    }

    if (vsizable)
    {
	all = VGroup,
	    Child,	muiSpace(1),
	    Child,	all,
	    Child,	muiSpace(1),
	End;
     }

    return all;
}

void MUIDynamicObject::resetChild()
{
    setChild(defaultMember);
}
