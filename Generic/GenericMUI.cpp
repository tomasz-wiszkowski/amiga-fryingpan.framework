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
#include "GenericMUI.h"
#include "LibrarySpool.h"
#include <intuition/classes.h>
#include <stdarg.h>
#include <libclass/utility.h>
#include <libclass/intuition.h>
#include "Debug.h"

using namespace GenNS;

iptr *GenericMUI::MUI_MakeObjectX(iptr Name, ...)
{
   va_list ap;
   iptr *params = new iptr [128];
   int pos = 0;
   va_start(ap, Name);
      
   while ((params[pos++] = va_arg(ap, iptr)) != 0)
   {
      params[pos++] = va_arg(ap, iptr);
      ASSERT(pos < 128);
   }
      
   pos = (int)MUIMaster->MUI_MakeObjectA(Name, (iptr*)params);
   delete [] params;
   return (Object*)pos;
}
   
iptr *GenericMUI::MUI_NewObject(char* Name, iptr FirstTag, ...)          // me hates mos for that.
{
   va_list ap;
   iptr *params = new iptr [128];
   int pos = 0;
   va_start(ap, FirstTag);
      
   params[pos] = FirstTag;
   while (params[pos++] != 0)
   {
      params[pos++] = va_arg(ap, iptr);
      params[pos] = va_arg(ap, iptr);         
      ASSERT(pos < 128);
   }
      
   pos = (int)MUIMaster->MUI_NewObjectA(Name, (struct TagItem*)params);
   delete [] params;
   return (Object*)pos;
}
  
#endif /* __linux__ */   
   

