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

#include "Device.h"
#include "DevWrapper_aros.h"
#define __NOLIBBASE__
#include <exec/resident.h>
#include <proto/exec.h>

struct Device *DEV_Init(Device *pOurBase, void *pSegList, ExecBase *pSysBase);

struct DevInitStruct DEV_InitStruct =
{
   (unsigned long)   sizeof(struct Device),
   (void*)           DEV_FuncTable,
   (void*)           NULL,
   (void (*)(void)) &DEV_Init
};

   struct Device *DevBase  = 0;
   static void   *SegList  = 0;

struct Device *DEV_Init(Device *pOurBase, void *pSegList, ExecBase *pSysBase)
{
   SysBase = (Library*)pSysBase;
   DevBase = pOurBase;
   SegList = pSegList;

   if (true == Dev_SetUp())
      return DevBase;
   
   FreeMem((void*)((ULONG)(DevBase) - DevBase->dd_Library.lib_NegSize), DevBase->dd_Library.lib_NegSize + DevBase->dd_Library.lib_PosSize);
   return 0;
}

struct Device *DEV_Open(struct Device *pOurBase, struct IORequest* r, uint unit, uint flags, struct ExecBase *pSysBase)
{
   if (false == Dev_Acquire(r, unit, flags))
      return 0;

   DevBase->dd_Library.lib_Flags &= ~LIBF_DELEXP;
   DevBase->dd_Library.lib_OpenCnt++;
   return DevBase;
}

uint DEV_Expunge()
{
   if (DevBase->dd_Library.lib_OpenCnt)
   {
      DevBase->dd_Library.lib_Flags |= LIBF_DELEXP;
      return 0;
   }

   Dev_CleanUp();

   Forbid();
   Remove(&DevBase->dd_Library.lib_Node);
   Permit();

   FreeMem((void*)((ULONG)(DevBase) - DevBase->dd_Library.lib_NegSize), DevBase->dd_Library.lib_NegSize + DevBase->dd_Library.lib_PosSize);
   return (ULONG)SegList;
}

void DEV_Close(struct Device*, struct IORequest* r)
{
   Dev_Release(r);

   if (DevBase->dd_Library.lib_OpenCnt == 0)
      return;

   if ((--DevBase->dd_Library.lib_OpenCnt) == 0)
   {
      if (DevBase->dd_Library.lib_Flags & LIBF_DELEXP)
         DEV_Expunge();
   }
   return;
}

uint DEV_Reserved(void)
{
   return 0;
}

sint DEV_BeginIO(struct Device *pOurBase, struct IORequest* r)
{
   return (r->io_Error = Dev_BeginIO(r));
}

sint DEV_AbortIO(struct Device *pOurBase, struct IORequest* r)
{
   return (r->io_Error = Dev_AbortIO(r));
}

