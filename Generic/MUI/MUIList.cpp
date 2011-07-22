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

#include "MUIList.h"
#include <Generic/LibrarySpool.h>
#include <libclass/intuition.h>
#include <mui/NList_mcc.h>
#include <mui/NListview_mcc.h>

using namespace GenNS;

MUIList::MUIList(const char *format, bool bms)
{
   listview       = 0;
   list           = 0;
   dragSortable   = false;
   multiSelect    = bms;
   this->format   = format;

   hHkOnSelect.Initialize(this, &MUIList::onSelect);
   hHkOnWBDrop.Initialize(this, &MUIList::onWBDrop);
   hHkOnDragSort.Initialize(this, &MUIList::onDragSort);
   hHkOnDblClick.Initialize(this, &MUIList::onDblClick);
}

MUIList::~MUIList()
{
}

iptr *MUIList::getObject()
{
   if (0 != listview)
      return listview;

   list = NListObject,
      InputListFrame,
      MUIA_NList_Title,          (iptr)"Title",
      MUIA_NList_Format,         (iptr)format.Data(),
      MUIA_NList_MultiSelect,    multiSelect ? MUIV_NList_MultiSelect_Default : MUIV_NList_MultiSelect_None,
      MUIA_NList_DragSortable,   dragSortable,
   End;

   ASSERTS((list != 0), "This program requires NList to work");

   if (list != 0)
   {
       listview = NListviewObject,
		MUIA_NListview_NList,      (iptr)list,
		End;

       ASSERTS((listview != 0), "This program requires NListview to work");

       DoMtd(list, ARRAY(MUIM_Notify, MUIA_NList_Active,	    (iptr)MUIV_EveryTime, (iptr)list, 2, MUIM_CallHook, (iptr)hHkOnSelect.GetHook()));
       DoMtd(list, ARRAY(MUIM_Notify, MUIA_NList_DragSortInsert,    (iptr)MUIV_EveryTime, (iptr)list, 2, MUIM_CallHook, (iptr)hHkOnDragSort.GetHook()));
       DoMtd(list, ARRAY(MUIM_Notify, MUIA_NList_DoubleClick,	    (iptr)MUIV_EveryTime, (iptr)list, 3, MUIM_CallHook, (iptr)hHkOnDblClick.GetHook(), MUIV_TriggerValue));

       DoMtd(list, ARRAY(MUIM_Notify, MUIA_AppMessage,   (iptr)MUIV_EveryTime, (iptr)list, 3, MUIM_CallHook, (iptr)hHkOnWBDrop.GetHook(), MUIV_TriggerValue));
   }

   return listview;
}

void MUIList::setConstructHook(const Hook* hook)
{
    if (0 == listview)
	getObject();

    Intuition->SetAttrsA(list, TAGARRAY( MUIA_NList_ConstructHook, (iptr)hook ));
}

void MUIList::setDestructHook(const Hook* hook)
{
    if (0 == listview)
	getObject();

    Intuition->SetAttrsA(list, TAGARRAY( MUIA_NList_DestructHook, (iptr)hook ));
}

void MUIList::setDisplayHook(const Hook* hook)
{
    if (0 == listview)
	getObject();

    Intuition->SetAttrsA(list, TAGARRAY( MUIA_NList_DisplayHook, (iptr)hook ));
}

void MUIList::setSelectionHook(const Hook* hook)
{
    hOnSelect = hook;
}

iptr MUIList::onSelect(void*, void*)
{
    void* data;

    DoMtd(list, ARRAY(MUIM_NList_GetEntry, (iptr)MUIV_NList_GetEntry_Active, (iptr)&data));
    return hOnSelect.Call(data, data);
}

iptr MUIList::onDblClick(void*, iptr item)
{
    iptr data;

    DoMtd(list, ARRAY(MUIM_NList_GetEntry, (iptr)MUIV_NList_GetEntry_Active, (iptr)&data));
    return hOnDblClick.Call(this, data);
}

iptr MUIList::onWBDrop(void*, AppMessage**m)
{
    return hOnWBDrop(*m, *m);
}

iptr MUIList::onDragSort(void*, void*)
{
    VectorT<void*> vec;
    void* data;

    request("Info", "Drag Sort", "Ok", 0);

    for (int32 i=0; ; i++)
    {
	DoMtd(list, ARRAY(MUIM_NList_GetEntry, i, (int32)&data));
	if (data == 0)
	    break;
	vec << data;
    }  
    return hOnSort.Call(&vec, &vec); 
}

void MUIList::addItem(void* item)
{
    ASSERT(0 != list);
    if (NULL != list)
	DoMtd(list, ARRAY(MUIM_NList_InsertSingle, (iptr)item, (iptr)MUIV_NList_Insert_Bottom));
}

void MUIList::clear()
{
    ASSERT(0 != list);
    if (NULL != list)
	DoMtd(list, ARRAY(MUIM_NList_Clear));
}

VectorT<void*> &MUIList::getSelectedItems()
{
    iptr nextSel = (iptr)MUIV_NList_NextSelected_Start;
    void* data;

    ASSERT(0 != list);
    selected.Empty();

    while (true)
    {
	DoMtd(list, ARRAY(MUIM_NList_NextSelected, (iptr)&nextSel));
	if (nextSel == (iptr)MUIV_NList_NextSelected_End)
	    break;
	DoMtd(list, ARRAY(MUIM_NList_GetEntry, nextSel, (iptr)&data));
	selected << data;
    }

    return selected;
}

void MUIList::setEnabled(bool enabled)
{
    Intuition->SetAttrsA(listview, (TagItem*)ARRAY(MUIA_Disabled, !enabled, TAG_DONE, 0));
}

void MUIList::setWBDropHook(const Hook* hook)
{
    hOnWBDrop = hook;
}

void MUIList::setDragSortHook(const Hook* hook)
{
    hOnSort = hook;
}

void MUIList::setDragSortable(bool sortable)
{
    dragSortable = sortable;
    if (0 != list)
	Intuition->SetAttrsA(list, TAGARRAY( MUIA_NList_DragSortable,   sortable ));
}

void MUIList::setDblClickHook(const Hook* hook)
{
    hOnDblClick = hook;
}
