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


#ifndef _GENERIC_MUI_MUIDYNAMICOBJECT_H
#define _GENERIC_MUI_MUIDYNAMICOBJECT_H

#include <Generic/HookT.h>
#include <Generic/HookAttrT.h>
#include <Generic/VectorT.h>
#include <Generic/MUI/MUI.h>

namespace GenNS 
{
    class MUIDynamicObject : public MUI
    {
    protected:
	iptr*		all;
	iptr*		currentMember;
	iptr*		defaultMember;
	iptr*		updatable;
	bool		hsizable;
	bool		vsizable;

    public:
	MUIDynamicObject(bool hsiz, bool vsiz);
	virtual         ~MUIDynamicObject();
	virtual iptr*   getObject();
	virtual void	setChild(iptr* object);
	virtual void	resetChild();
    };
};

#endif

