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

#ifndef __linux__
#include "GenericOOP.h"
#include "LibrarySpool.h"
#include <libclass/exec.h>
#include <libclass/utility.h>
#include <libclass/intuition.h>
#include <stdarg.h>
#include "Debug.h"

using namespace GenNS;

iptr GenericOOP::DoMtd(iptr *pObject, iptr *pMsg)
{
   return Utility->CallHookPkt((const Hook*)OCLASS(pObject), (void*)pObject, pMsg);
}

iptr GenericOOP::DoSuperMtd(IClass *pClass, iptr *pObject, iptr *pMsg)
{
   return Utility->CallHookPkt((Hook*)pClass->cl_Super, (void*)pObject, pMsg);
}

iptr *GenericOOP::NewObj(char* Name, iptr FirstTag, ...)          // me hates mos for that.
{
   va_list ap;
   iptr *params = new iptr [128];
   iptr pos = 0;
   va_start(ap, FirstTag);
      
   params[pos] = FirstTag;
   while (params[pos++] != 0)
   {
      params[pos++] = va_arg(ap, iptr);
      params[pos] = va_arg(ap, iptr);         
      ASSERT(pos < 128);
   }

   pos = (iptr)Intuition->NewObjectA(0, Name, (struct TagItem*)params);
   delete [] params;
   return (iptr*)pos;
}

iptr *GenericOOP::NewObj(Class* cls, iptr FirstTag, ...)          // me hates mos for that.
{
   va_list ap;
   iptr *params = new iptr [128];
   iptr pos = 0;
   va_start(ap, FirstTag);
      
   params[pos] = FirstTag;
   while (params[pos++] != 0)
   {
      params[pos++] = va_arg(ap, iptr);
      params[pos] = va_arg(ap, iptr);         
      ASSERT(pos < 128);
   }

   pos = (iptr)Intuition->NewObjectA((IClass*)cls, 0, (struct TagItem*)params);
   
   delete [] params;
   return (iptr*)pos;
}

void GenericOOP::DisposeObj(iptr *obj)
{
   Intuition->DisposeObject((void*)obj);
}

void GenericOOP::AddChildObj(iptr *parent, iptr *child)
{
   DoMtd(parent, ARRAY(OM_ADDMEMBER, (iptr)child));
}

void GenericOOP::RemChildObj(iptr *parent, iptr *child)
{
   DoMtd(parent, ARRAY(OM_REMMEMBER, (iptr)child));
}

GenericOOP::~GenericOOP()
{
}

GenericOOP::GenericOOP()
{
   ASSERTS(Utility != 0, "Please create library spool!");
}

#endif /* __linux__ */
