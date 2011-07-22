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

#ifndef _DEVWRAPPER_H_
#define _DEVWRAPPER_H_

#include <exec/libraries.h>
#include <Generic/Types.h>

struct DevInitStruct
{
   unsigned long  DevSize;
   void          *FuncTable;
   void          *DataTable;
   void           (*InitFunc)(void);
};


#ifdef __cplusplus
extern "C"
{
#endif 
   struct Device                *DEV_Open(struct Device *pOurBase, struct IORequest* r, uint unit, uint flags, struct ExecBase *pSysBase);
   void                          DEV_Close(struct Device *pOurBase, struct IORequest* r);
   uint                          DEV_Expunge();
   uint                          DEV_Reserved();
   sint                          DEV_BeginIO(struct Device *pOurBase, struct IORequest* r);
   sint                          DEV_AbortIO(struct Device *pOurBase, struct IORequest* r);
   extern struct DevInitStruct   DEV_InitStruct;
#ifdef __cplusplus
};
#endif

#define DEVICE(name, id, version)               \
   struct Resident DEV_Resident =               \
   {                                            \
      RTC_MATCHWORD,                            \
      &DEV_Resident,                            \
      &DEV_Resident+1,                          \
      RTF_AUTOINIT,                             \
      version,                                  \
      NT_DEVICE,                                \
      0,                                        \
      name,                                     \
      id,                                       \
      &DEV_InitStruct,                          \
   };      

#define DEV_FT_Begin                            \
   uint DEV_FuncTable[] =                       \
   {                                            \
      (uint) &DEV_Open,                         \
      (uint) &DEV_Close,                        \
      (uint) &DEV_Expunge,                      \
      (uint) &DEV_Reserved,                     \
      (uint) &DEV_BeginIO,                      \
      (uint) &DEV_AbortIO,

// }

#define DEV_FT_Function(f) (uint) &f,

// { DEV_FT_End
#define DEV_FT_End                              \
      ~0,                                       \
   };

#define GATE0(type, name)                       \
   type name();

#define GATE1(type, name, type1, reg1)          \
   type name(type1);

#define GATE2(type, name, type1, reg1, type2, reg2) \
   type name(type1, type2);

#define GATE3(type, name, type1, reg1, type2, reg2, type3, reg3) \
   type name(type1, type2, type3);

#define GATE4(type, name, type1, reg1, type2, reg2, type3, reg3, type4, reg4) \
   type name(type1, type2, type3, type4);

#define GATE5(type, name, type1, reg1, type2, reg2, type3, reg3, type4, reg4, type5, reg5) \
   type name(type1, type2, type3, type4, type5);




#endif //_LIBWRAPPER_H_
