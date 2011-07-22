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

#ifndef _DEVWRAPPER_OS4_H_
#define _DEVWRAPPER_OS4_H_

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
   struct Device          *DEV_Init(struct Device*, void*, struct ExecIFace*);
   struct Device          *DEV_Open(struct LibraryManagerInterface *pSelf, struct IORequest*, uint, uint);
   uint                    DEV_Close(struct LibraryManagerInterface *pSelf, struct IORequest*);
   uint                    DEV_Expunge(struct LibraryManagerInterface *pSelf);
   uint                    DEV_Acquire(struct LibraryManagerInterface *pSelf);
   uint                    DEV_Release(struct LibraryManagerInterface *pSelf);
   sint                    DEV_BeginIO(struct LibraryManagerInterface *pOurBase, struct IORequest* r);
   sint                    DEV_AbortIO(struct LibraryManagerInterface *pOurBase, struct IORequest* r);
   extern struct TagItem   DEV_CreateTags[];

#ifdef __cplusplus
};
#endif

#define DEVICE(name, id, version)               \
   struct Resident DEV_Resident =               \
   {                                            \
      RTC_MATCHWORD,                            \
      &DEV_Resident,                            \
      &DEV_Resident+1,                          \
      RTF_AUTOINIT | RTF_NATIVE,                \
      version,                                  \
      NT_DEVICE,                                \
      0,                                        \
      name,                                     \
      id,                                       \
      &DEV_CreateTags,                          \
   };      

#define DEV_FT_Begin                            \
   uint DEV_FuncTable[] =                       \
   {                                            \
      (uint) &DEV_Acquire,                      \
      (uint) &DEV_Release,                      \
      (uint) 0,                                 \
      (uint) 0,                                 \
      (uint) &DEV_BeginIO,                      \
      (uint) &DEV_AbortIO,                      \
// }      

#define DEV_FT_Function(f) (uint) &__gateway__##f,

// { DEV_FT_End
#define DEV_FT_End                              \
      0xffffffff,                               \
   };

#define GATE0(type, name)                       \
   type name();                                 \
   type __gateway__##name(Interface*)           \
   {                                            \
      return name();                            \
   }

#define GATE1(type, name, type1, reg1)          \
   type name(type1);                            \
   type __gateway__##name(Interface*,type1 p1)  \
   {                                            \
      return name(p1);                          \
   }

#define GATE2(type, name, type1, reg1, type2, reg2) \
   type name(type1, type2);                     \
   type __gateway__##name(Interface*, type1 p1, type2 p2)\
   {                                            \
      return name(p1, p2);                      \
   }
      
#define GATE3(type, name, type1, reg1, type2, reg2, type3, reg3) \
   type name(type1, type2, type3);              \
   type __gateway__##name(Interface*, type1 p1, type2 p2, type3 p3)\
   {                                            \
      return name(p1, p2, p3);                  \
   }
      
#define GATE4(type, name, type1, reg1, type2, reg2, type3, reg3, type4, reg4) \
   type name(type1, type2, type3, type4);       \
   type __gateway__##name(Interface*, type1 p1, type2 p2, type3 p3, type4 p4)\
   {                                            \
      return name(p1, p2, p3, p4);              \
   }
      
#define GATE5(type, name, type1, reg1, type2, reg2, type3, reg3, type4, reg4, type5, reg5) \
   type name(type1, type2, type3, type4, type5);\
   type __gateway__##name(Interface*, type1 p1, type2 p2, type3 p3, type4 p4, type5 p5)\
   {                                            \
      return name(p1, p2, p3, p4, p5);  \
   }
      



#endif //_LIBWRAPPER_OS4_H_
