/*
 * Amiga Generic Set - set of libraries and includes to ease sw development for all Amiga platforms
 * Copyright (C) 2004-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com.
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

#ifndef __linux__

#include "Types.h"
#include <intuition/classes.h>

namespace GenNS
{
   class GenericOOP 
   {
   public:
                              GenericOOP();
      virtual                ~GenericOOP();  
      virtual iptr            DoMtd(iptr *pObject, iptr *pMsg);
      virtual iptr            DoSuperMtd(class IClass *pClass, iptr *pObject, iptr *pMsg);
      virtual iptr           *NewObj(char* Name, iptr FirstTag, ...);
      virtual iptr           *NewObj(Class* cls, iptr FirstTag, ...);
      virtual void            DisposeObj(iptr *obj);
      virtual void            AddChildObj(iptr *parent, iptr *child);
      virtual void            RemChildObj(iptr *parent, iptr *child);
   };
};
#endif /* __linux__ */
#endif

