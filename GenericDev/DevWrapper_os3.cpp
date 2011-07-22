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
#include "DevWrapper_os3.h"
#define __NOLIBBASE__
#include <exec/libraries.h>
#include <exec/resident.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <LibC/LibC.h>

struct Device *DEV_Init();

struct DevInitStruct DEV_InitStruct =
{
   (unsigned long)   sizeof(struct Device),
   (void*)           DEV_FuncTable,
   (void*)           NULL,
   (void (*)(void)) &DEV_Init
};

   struct Device *DevBase  = 0;
   static void   *SegList  = 0;

struct Device *DEV_Init()
{
   asm volatile ( "move.l a6,%0\n\t"
                  "move.l a0,%1\n\t"
                  "move.l d0,%2\n\t"
                  : "=omg"(SysBase), "=omg"(SegList), "=omg"(DevBase)
                  :
                  : "cc", "memory");
   
   if (true == Dev_SetUp())
   {
      //request("Info", "initializing", "ok", 0);
      return DevBase;
   }

   FreeMem((void*)((ULONG)(DevBase) - DevBase->dd_Library.lib_NegSize), DevBase->dd_Library.lib_NegSize + DevBase->dd_Library.lib_PosSize);
   return 0;
}

struct Device *DEV_Open(void)
{
   uint unit, flags;
   struct IORequest *ior;

   asm volatile ( "move.l a1,%0\n\t"
                  "move.l d0,%1\n\t"
                  "move.l d1,%2\n\t"
                  : "=omg"(ior), "=omg"(unit), "=omg"(flags)
                  :
                  : "cc", "memory");
   
   //request("Info", "opening", "ok", 0);
   if (false == Dev_Acquire(ior, unit, flags))
      return 0;

   DevBase->dd_Library.lib_Flags &= ~LIBF_DELEXP;
   DevBase->dd_Library.lib_OpenCnt++;
   return DevBase;
}

uint DEV_Expunge()
{
   //request("Info", "expunging", "ok", 0);
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

void DEV_Close()
{
   struct IORequest* ior;
   asm volatile ( "move.l a1,%0\n\t"
                  : "=omg"(ior)
                  :
                  : "cc", "memory");
   
   //request("Info", "closing", "ok", 0);
   Dev_Release(ior);

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
   //request("Info", "call to reserved", "ok", 0);
   return 0;
}

sint DEV_BeginIO()
{
   struct IORequest* ior;
   asm volatile ( "move.l a1,%0\n\t"
                  : "=omg"(ior)
                  :
                  : "cc", "memory");
   //request("Info", "call to beginio", "ok", 0);
   return (ior->io_Error = Dev_BeginIO(ior));
}

sint DEV_AbortIO()
{
   struct IORequest* ior;
   asm volatile ( "move.l a1,%0\n\t"
                  : "=omg"(ior)
                  :
                  : "cc", "memory");
   //request("Info", "call to abortio", "ok", 0);
   return (ior->io_Error = Dev_AbortIO(ior));
}

