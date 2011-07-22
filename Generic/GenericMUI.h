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

#ifndef _GENERICMUI_H_
#define _GENERICMUI_H_

#ifndef __linux__

#include "GenericOOP.h"
#include "LibrarySpool.h"
#include <intuition/classes.h>
#include <libclass/muimaster.h>

#undef MUI_MakeObject
#define MUI_MakeObject(a, b...) MUIMaster->MUI_MakeObjectA(a, ARRAY(b))
#undef MUI_NewObject

#define ButtonObject TextObject, \
                        ButtonFrame, \
                        MUIA_Font,          MUIV_Font_Button,\
                        MUIA_Text_PreParse, "\033c",\
                        MUIA_Background,    MUII_ButtonBack,\
                        MUIA_InputMode,     MUIV_InputMode_RelVerify

#define CheckmarkObject(state, title) \
   GroupObject,                                    \
      MUIA_Group_Horiz,    true,                   \
      MUIA_InputMode,      MUIV_InputMode_Toggle,  \
      MUIA_ShowSelState,   false,                  \
      Child,               ImageObject,            \
         ImageButtonFrame,                         \
         MUIA_Image_Spec,     MUII_CheckMark,      \
         MUIA_Background,     MUII_ButtonBack,     \
         MUIA_Image_FreeVert, true,                \
         MUIA_ShowSelState,   false,               \
         MUIA_Weight,         0,                   \
      End,                                         \
      Child,               TextObject,             \
         MUIA_Text_Contents,  title,               \
         MUIA_ShowSelState,   false,               \
      End,                                         \
   End

extern MUIMasterIFace *MUIMaster;

namespace GenNS
{
   class GenericMUI : public GenericOOP
   {
   protected:

      iptr                   *MUI_MakeObjectX(iptr Name, ...);
      iptr                   *MUI_NewObject(char* Name, iptr FirstTag, ...);
   };
};

#endif /* __linux__ */
#endif //_GENERICMUI_H_
