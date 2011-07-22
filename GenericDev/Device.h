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

#ifndef _DEVWRAPPER_COMMON_H_
#define _DEVWRAPPER_COMMON_H_

#define __NOLIBBASE__
#include <exec/resident.h>
#include <Generic/Types.h>

#ifdef __cplusplus
extern "C"
{
#endif

   extern struct Library*      SysBase;
   extern struct Device*       DevBase;
   
   extern uint32               DEV_FuncTable[];    // LIB_FT_Begin + LIB_FT_Function + LIB_FT_End

   extern bool                 Dev_SetUp();
   extern bool                 Dev_Acquire(struct IORequest*, uint, uint);   // user defined setup with weak local binding
   extern void                 Dev_Release(struct IORequest*);             // in these functions you should only perform your own
   extern void                 Dev_CleanUp();
   extern sint                 Dev_BeginIO(struct IORequest*);
   extern sint                 Dev_AbortIO(struct IORequest*);

#ifdef __cplusplus
};
#endif

#if defined(__AMIGAOS4__)
   #include "DevWrapper_os4.h"
#elif defined(__MORPHOS__)
   #include "DevWrapper_mos.h"
#elif defined(__AROS__)
   #include "DevWrapper_aros.h"
#else
   #include "DevWrapper_os3.h"
#endif

#endif //_LIBWRAPPER_COMMON_H_
