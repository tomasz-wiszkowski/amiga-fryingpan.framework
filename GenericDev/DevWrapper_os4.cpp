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
#include "DevWrapper_os4.h"
#include <exec/resident.h>
#include <proto/exec.h>


   /*
    * 1: Function table for library manager
    */

static uint MainFuncTable[] =
{
      (uint) &DEV_Acquire,
      (uint) &DEV_Release,
      (uint) 0,
      (uint) 0,
      (uint) &DEV_Open,    
      (uint) &DEV_Close,
      (uint) &DEV_Expunge,
      (uint) 0,
      (uint) &DEV_BeginIO,
      (uint) &DEV_AbortIO,
      (uint) 0xffffffff,
};

static struct TagItem MainManagerTags[] =
{
   { MIT_Name,          (uint)"__device"     },
   { MIT_VectorTable,   (uint)MainFuncTable  },
   { MIT_Version,       1                    },
   { TAG_DONE,          0                    }
};

static struct TagItem DevManagerTags[] =
{
   { MIT_Name,          (uint)"main"         },
   { MIT_VectorTable,   (uint)DEV_FuncTable  },
   { MIT_Version,       1                    },
   { TAG_DONE,          0                    }
};



   /*
    * 3: All interfaces together
    */

static const uint DevInterfaces[] =
{
   (uint)   MainManagerTags,
   (uint)   DevManagerTags,
   (uint)   0
};



   /*
    * 4: Library creation tags
    */
    
struct TagItem DEV_CreateTags[] =
{
   { CLT_DataSize,      (uint)sizeof(struct Device)   },
   { CLT_InitFunc,      (uint)&DEV_Init               },
   { CLT_Interfaces,    (uint)&DevInterfaces          },
   { TAG_DONE,          (uint)0                       }
};

   struct Device       *DevBase  = 0;
   void                *SegList  = 0;

struct Device *DEV_Init(Device *pOurBase, void *pSegList, ExecIFace *pSysBase) 
{
   SysBase = pSysBase->Data.LibBase;
   IExec = pSysBase;
   DevBase = pOurBase;
   SegList = pSegList;

   if (true == Dev_SetUp())
      return DevBase;
   
   DeleteLibrary(&DevBase->dd_Library);
   return 0;
}

uint DEV_Expunge(struct LibraryManagerInterface *pSelf)
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

   DeleteLibrary(pSelf->Data.LibBase);

   return (ULONG)SegList;
}

struct Device *DEV_Open(struct LibraryManagerInterface *pSelf, struct IORequest *ior, uint unit, uint flags)
{
   if (false == Dev_Acquire(ior, unit, flags))
      return 0;
   DevBase->dd_Library.lib_Flags &= ~LIBF_DELEXP;
   DevBase->dd_Library.lib_OpenCnt++;
   return DevBase;
}

uint DEV_Close(struct LibraryManagerInterface *pSelf, struct IORequest *ior)
{
   Dev_Release(ior);

   if (DevBase->dd_Library.lib_OpenCnt == 0)
      return 0;

   if ((--DevBase->dd_Library.lib_OpenCnt) == 0)
   {
      if (DevBase->dd_Library.lib_Flags & LIBF_DELEXP)
         return (DEV_Expunge(pSelf));
   }
   return 0;
}

uint DEV_Acquire(struct LibraryManagerInterface *pSelf)
{
   return (pSelf->Data.RefCount++);
}

uint DEV_Release(struct LibraryManagerInterface *pSelf)
{
   return (pSelf->Data.RefCount--);
}

sint DEV_BeginIO(struct LibraryManagerInterface *pSelf, struct IORequest* r)
{
   return (r->io_Error = Dev_BeginIO(r));
}

sint DEV_AbortIO(struct LibraryManagerInterface* pSelf, struct IORequest* r)
{
   return (r->io_Error = Dev_AbortIO(r));
}

