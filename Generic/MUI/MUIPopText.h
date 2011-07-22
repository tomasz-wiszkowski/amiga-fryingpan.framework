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

#ifndef _GENERIC_MUI_MUIPOPTEXT_H_
#define _GENERIC_MUI_MUIPOPTEXT_H_


#include "MUIPopString.h"
#include "MUI.h"

namespace GenNS
{
    class MUIPopText : public MUIPopString
    {
    protected:
	virtual iptr *getPopDisplay();
    public:
	MUIPopText(const char* name, iptr cols=1, const char** names=0);
	virtual ~MUIPopText();
    };
};

#endif

