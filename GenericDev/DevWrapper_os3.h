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

#ifndef _DEVWRAPPER_H_
#define _DEVWRAPPER_H_

#include <exec/libraries.h>
#include <exec/resident.h>
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
   struct Device                *DEV_Open(void);
   void                          DEV_Close(void);
   uint                          DEV_Expunge();
   uint                          DEV_Reserved();
   sint                          DEV_BeginIO(void);
   sint                          DEV_AbortIO(void);
   extern struct DevInitStruct   DEV_InitStruct;

#ifdef __cplusplus
};
#endif

   /*
    * the following macros DEFINE library
    */

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

#define DEV_FT_Function(f) (uint) &__gateway__##f,

// { DEV_FT_End
#define DEV_FT_End                              \
      0xffffffff,                               \
   };

#define GATE0(type, name)                       \
   type name();                                 \
   type __gateway__##name()                     \
   {                                            \
      return name();                            \
   }

#define GATE1(type, name, type1, reg1)          \
   type name(type1);                            \
   type __gateway__##name()                     \
   {                                            \
      register type1 _reg1 asm(#reg1);          \
      return name(_reg1);                       \
   }

#define GATE2(type, name, type1, reg1, type2, reg2) \
   type name(type1, type2);                     \
   type __gateway__##name()                     \
   {                                            \
      register type1 _reg1 asm(#reg1);          \
      register type2 _reg2 asm(#reg2);          \
      return name(_reg1, _reg2);                \
   }
      
#define GATE3(type, name, type1, reg1, type2, reg2, type3, reg3) \
   type name(type1, type2, type3);              \
   type __gateway__##name()                     \
   {                                            \
      register type1 _reg1 asm(#reg1);          \
      register type2 _reg2 asm(#reg2);          \
      register type3 _reg3 asm(#reg3);          \
      return name(_reg1, _reg2, _reg3);         \
   }
      
#define GATE4(type, name, type1, reg1, type2, reg2, type3, reg3, type4, reg4) \
   type name(type1, type2, type3, type4);       \
   type __gateway__##name()                     \
   {                                            \
      register type1 _reg1 asm(#reg1);          \
      register type2 _reg2 asm(#reg2);          \
      register type3 _reg3 asm(#reg3);          \
      register type4 _reg4 asm(#reg4);          \
      return name(_reg1, _reg2, _reg3, _reg4);  \
   }
      
#define GATE5(type, name, type1, reg1, type2, reg2, type3, reg3, type4, reg4, type5, reg5) \
   type name(type1, type2, type3, type4, type5);\
   type __gateway__##name()                     \
   {                                            \
      register type1 _reg1 asm(#reg1);          \
      register type2 _reg2 asm(#reg2);          \
      register type3 _reg3 asm(#reg3);          \
      register type4 _reg4 asm(#reg4);          \
      register type5 _reg5 asm(#reg5);          \
      return name(_reg1, _reg2, _reg3, _reg4, _reg5);  \
   }
      
      
   /*
    * the following macros DECLARE library
    */


#endif //_LIBWRAPPER_H_
