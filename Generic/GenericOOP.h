/*
 * Amiga Generic Set - set of libraries and includes to ease sw development for all Amiga platforms
 * Copyright (C) 2001-2011 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com.
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

#ifndef _GENERIC_OOP_H_
#define _GENERIC_OOP_H_

#include "Types.h"
#include <intuition/classes.h>
#include <intuition/classusr.h>

namespace GenNS
{
   class GenericOOP 
   {
   public:
                              GenericOOP();
      virtual                ~GenericOOP();  
      virtual uint32          DoMtd(uint32* pObject, uint32 *pMsg);
      virtual uint32          DoSuperMtd(IClass *pClass, uint32* pObject, uint32 *pMsg);
      virtual uint32         *NewObj(char* Name, uint32 FirstTag, ...);
      virtual uint32         *NewObj(Class* cls, uint32 FirstTag, ...);
      virtual void            DisposeObj(uint32 *obj);
      virtual void            AddChildObj(uint32 *parent, uint32 *child);
      virtual void            RemChildObj(uint32 *parent, uint32 *child);
   };
};
#endif

