/*
 * Amiga Generic Set - set of libraries and includes to ease sw development for all Amiga platforms
 * Copyright (C) 2004-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "MUI.h"
#include <Generic/LibrarySpool.h>
#include <libclass/intuition.h>
#include <libraries/mui.h>

using namespace GenNS;

MUI::MUI()
{
   cbButtonHook = 0;
   gateBtns.Initialize(this, &MUI::gateButtons);
}

MUI::~MUI()
{
   muiElements.ForEach(&MUI::freeItem);
}
   
bool MUI::freeItem(elemAssoc* const &item)
{
   delete item;
   return true;
}

iptr *MUI::muiLabel(const char* contents, char key, int32 id, int32 weight, Alignment align)
{
   iptr *all;
   
   all = TextObject,
      MUIA_Text_Contents,     contents,
      MUIA_Weight,            weight,
      MUIA_Text_HiChar,       key,
      MUIA_ShowSelState,      false,
      MUIA_Text_SetMin,	      0,
      MUIA_Text_PreParse,     align == Align_Left     ? "\033l" :
                              align == Align_Right    ? "\033r" :
                                                        "\033c",
   End;
   
   muiElements << new elemAssoc(T_Label, id, all);

   return all;
}

iptr *MUI::muiButton(const char* contents, char key, int32 id)
{
   iptr *btn;
   
   btn = TextObject,
      ButtonFrame,
      MUIA_Text_Contents,     contents,
      MUIA_Text_HiChar,       key,
      MUIA_ControlChar,       key,
      MUIA_Text_PreParse,     "\033c",
      MUIA_Font,              MUIV_Font_Button,
      MUIA_Background,        MUII_ButtonBack,
      MUIA_InputMode,         MUIV_InputMode_RelVerify,
   End;

   DoMtd(btn, ARRAY(MUIM_Notify, MUIA_Pressed, false, (iptr)btn, 4, MUIM_CallHook, (iptr)gateBtns.GetHook(), id, 0));

   muiElements << new elemAssoc(T_Button, id, btn);

   return btn;
}
	
iptr *MUI::muiImageButton(iptr image, char key, int32 id)
{
    iptr *btn;

    btn = ImageObject,
	ButtonFrame,
	MUIA_Image_Spec,	image,
	MUIA_ControlChar,       key,
	MUIA_Background,        MUII_ButtonBack,
	MUIA_InputMode,         MUIV_InputMode_RelVerify,
    End;

    DoMtd(btn, ARRAY(MUIM_Notify, MUIA_Pressed, false, (iptr)btn, 4, MUIM_CallHook, (iptr)gateBtns.GetHook(), id, 0));

    muiElements << new elemAssoc(T_Button, id, btn);

    return btn;
}

iptr *MUI::muiCycle(const char** contents, char key, int32 id, int32 active)
{
   iptr *btn;
   
   btn = CycleObject,
      MUIA_Cycle_Entries,     contents,
      MUIA_ControlChar,       key,
   End;

   DoMtd(btn, ARRAY(MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, (iptr)btn, 4, MUIM_CallHook, (iptr)gateBtns.GetHook(), id, MUIV_TriggerValue));

   Intuition->SetAttrsA(btn, (TagItem*)ARRAY(
      MUIA_Cycle_Active,      active,
      TAG_DONE,               0
   ));

   muiElements << new elemAssoc(T_Cycle, id, btn);
   return btn;
}

iptr MUI::gateButtons(iptr btn, iptr* idptr)
{
   return cbButtonHook(idptr[0], idptr[1]);
}

void MUI::setButtonCallBack(const Hook *hook)
{
   cbButtonHook = hook;
}

iptr *MUI::muiCheckBox(const char* name, char key, int32 id, Alignment align, bool state)
{
   iptr *all;
   iptr *checkbox;
   iptr *label = 0;

   if (0 != name)
   {
      label    = muiLabel(name, key, ID_Default, 100, align);
   } 
   else
   {
      label    = muiLabel("", 0, ID_Default, 100, align);
   }

   checkbox = ImageObject,
      ImageButtonFrame,
      MUIA_Image_Spec,        MUII_CheckMark,
      MUIA_Background,        MUII_ButtonBack,
      MUIA_Image_FreeVert,    true,
      MUIA_ShowSelState,      false,
      MUIA_Weight,            0,
   End;

   all = HGroup,
      MUIA_InputMode,         MUIV_InputMode_Toggle,
      MUIA_ShowSelState,      false,
      Child,                  (align == Align_Left) ? checkbox : label,
      Child,                  (align == Align_Left) ? label : checkbox,
      MUIA_ControlChar,       key,
   End;

   DoMtd(all, ARRAY(MUIM_Notify, MUIA_Selected, MUIV_EveryTime, (iptr)all, 4, MUIM_CallHook, (iptr)gateBtns.GetHook(), id, MUIV_TriggerValue));

   muiElements << new elemAssoc(T_CheckBox, id, all);

   muiSetSelected(id, state);

   return all;
}

iptr *MUI::muiSlider(int32 min, int32 max, int32 level, char key, int32 id)
{
   iptr *slider;

   slider = SliderObject,
      MUIA_Numeric_Min,       min,
      MUIA_Numeric_Max,       max,
      MUIA_Numeric_Value,     level,
      MUIA_ControlChar,       key,
   End;

   DoMtd(slider, ARRAY(MUIM_Notify, MUIA_Numeric_Value, MUIV_EveryTime, (iptr)slider, 4, MUIM_CallHook, (iptr)gateBtns.GetHook(), id, MUIV_TriggerValue));

   muiElements << new elemAssoc(T_Slider, id, slider);
   return slider;
}

iptr *MUI::muiBar(const char *name, bool vertical)
{
   return RectangleObject,
      MUIA_Rectangle_HBar,       !vertical,
      MUIA_Rectangle_VBar,       vertical,
      MUIA_Rectangle_BarTitle,   name,
      vertical ? 
         MUIA_FixWidth : MUIA_FixHeight,  10,
   End;
}

void MUI::muiSetEnabled(int32 id, bool enabled)
{
   for (uint32 i=0; i<muiElements.Count(); i++)
   {
      if (muiElements[i]->id == id)
      {
         Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
            MUIA_Disabled,    !enabled,
            TAG_DONE,         0
         ));
      }
   }
}

void MUI::muiSetSelected(int32 id, int32 selected)
{
   for (uint32 i=0; i<muiElements.Count(); i++)
   {
      if (muiElements[i]->id == id)
      {
         switch (muiElements[i]->type)
         {
            case T_CheckBox:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_Selected,    selected,
                  TAG_DONE,         0
               ));
               break;
         
            case T_Cycle:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_Cycle_Active,selected,
                  TAG_DONE,         0
               ));
               break;

            default:
               break;
         }
      }
   }
}

void MUI::muiSetVisible(int32 id, bool visible)
{
   for (uint32 i=0; i<muiElements.Count(); i++)
   {
      if (muiElements[i]->id == id)
      {
         Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
            MUIA_ShowMe,      visible,
            TAG_DONE,         0
         ));
      }
   }
}

void MUI::muiSetValue(int32 id, int32 value)
{
   for (uint32 i=0; i<muiElements.Count(); i++)
   {
      if (muiElements[i]->id == id)
      {
         switch (muiElements[i]->type)
         {
            case T_Gauge:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_Gauge_Current,     (iptr)value,
                  TAG_DONE,               0
               ));
               break;

            case T_Slider:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_Numeric_Value,     (iptr)value,
                  TAG_DONE,               0
               ));
               break;

            default:
               break;
         }
      }
   }
}

void MUI::muiSetText(int32 id, const char *text)
{
   for (uint32 i=0; i<muiElements.Count(); i++)
   {
      if (muiElements[i]->id == id)
      {
         switch (muiElements[i]->type)
         {
            case T_Label:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_Text_Contents,     (iptr)text,
                  TAG_DONE,               0
               ));
               break;
         
            case T_String:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_String_Contents,   (iptr)text,
                  TAG_DONE,               0
               ));
               break;

            case T_Gauge:
               Intuition->SetAttrsA(muiElements[i]->elem, (TagItem*)ARRAY(
                  MUIA_Gauge_InfoText,    (iptr)text,
                  TAG_DONE,               0
               ));
               break;

            default:
               break;
         }
      }
   }
}

iptr *MUI::muiString(const char* contents, char key, int32 id, Alignment align)
{
   iptr *all;

   all = StringObject,
      StringFrame,
      MUIA_String_AdvanceOnCR,   true,
      MUIA_ControlChar,          key,
      MUIA_String_Format,        align == Align_Left  ?  MUIV_String_Format_Left:
                                 align == Align_Right ?  MUIV_String_Format_Right:
                                                         MUIV_String_Format_Center,
   End;

   muiElements << new elemAssoc(T_String, id, all);

   DoMtd(all, ARRAY(MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, (iptr)all, 4, MUIM_CallHook, (iptr)gateBtns.GetHook(), id, MUIV_TriggerValue));
   muiSetText(id, contents);
   return all;
}

iptr *MUI::muiGauge(const char* contents, int32 id)
{
   iptr *all;

   all = GaugeObject,
      GaugeFrame,
      MUIA_Gauge_Horiz,          true,
      MUIA_Gauge_InfoText,       contents,
      MUIA_Gauge_Max,            65535,
      MUIA_Gauge_Current,        0,
   End;

   muiElements << new elemAssoc(T_Gauge, id, all);

   return all;
}

iptr *MUI::muiSpace(int32 weight)
{
   return RectangleObject,
      MUIA_Weight,      weight,
   End;
}

