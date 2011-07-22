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

#include "MUITree.h"
#include <Generic/LibrarySpool.h>
#include <libclass/intuition.h>
#include <mui/NListtree_mcc.h>
#include <mui/NListview_mcc.h>
using namespace GenNS;

MUITree::MUITree(const char *format, bool multiselect)
{
   listview = 0;
   list     = 0;
   this->multiselect = multiselect;
   this->format      = format;

   hHkConstruct.Initialize(this, &MUITree::doConstruct);
   hHkDestruct.Initialize(this, &MUITree::doDestruct);
   hHkDisplay.Initialize(this, &MUITree::doDisplay);
   hHkSelect.Initialize(this, &MUITree::doSelect);
}

MUITree::~MUITree()
{
}

iptr *MUITree::getObject()
{
   if (0 != listview)
      return listview;

   list = NListtreeObject,
      InputListFrame,
      MUIA_NListtree_Format,        (iptr)format.Data(),    
      MUIA_NListtree_Title,         true,
      MUIA_NListtree_MultiSelect,   multiselect,
      MUIA_NListtree_ConstructHook, (iptr)hHkConstruct.GetHook(),
      MUIA_NListtree_DestructHook,  (iptr)hHkDestruct.GetHook(),
      MUIA_NListtree_DisplayHook,   (iptr)hHkDisplay.GetHook(),
      MUIA_NListtree_DragDropSort,  false,
   End;

   if (list != 0)
   {
      listview = NListviewObject,
         MUIA_NListview_NList,      (iptr)list,
      End;
   
      DoMtd(list, ARRAY(MUIM_Notify, MUIA_NListtree_Active, MUIV_EveryTime, (iptr)list, 2, MUIM_CallHook, (iptr)hHkSelect.GetHook()));
   }
   else
   {
      ASSERTS(false, "NListTree could not be found! Memory will leak a little..");
      return TextObject, 
         MUIA_Text_Contents,     "NListTree could not be found!",
      End;
   }


   return listview;
}

void MUITree::setConstructHook(const Hook* hook)
{
   hConstruct = hook;
}

void MUITree::setDestructHook(const Hook* hook)
{
   hDestruct = hook;
}

void MUITree::setDisplayHook(const Hook* hook)
{
   hDisplay = hook;
}

void MUITree::setSelectionHook(const Hook* hook)
{
   hSelect = hook;
}

void MUITree::setWeight(int weight)
{
   iptr *obj = getObject();

   if (0 == obj)
      return;

   Intuition->SetAttrsA(obj, (TagItem*)ARRAY(
      MUIA_Weight,                  weight,
      TAG_DONE,                     0
   ));

}

iptr MUITree::doConstruct(void*, MUIP_NListtree_ConstructMessage* m)
{
   if (hConstruct.IsValid())
   {
      return hConstruct.Call(this, m->UserData);
   }
   else
   {
      return (iptr)m->UserData;
   }
}

iptr MUITree::doDestruct(void*, MUIP_NListtree_DestructMessage* m)
{
   if (hDestruct.IsValid())
   {
      return hDestruct.Call(this, m->UserData);
   }
   else
   {
      return 0;
   }
}

iptr MUITree::doDisplay(void*, MUIP_NListtree_DisplayMessage* m)
{
   if (hDisplay.IsValid())
   {
      if (m->EntryPos >= 0)
         return hDisplay.Call(m->Array, m->TreeNode->tn_User);
      else
         return hDisplay.Call(m->Array, (void*)NULL);
   }
   else
   {
      /*
       * due to the fact that we put just dummy data as node name, 
       * we need to pass the UserData as the name here :)
       */
      if (m->EntryPos >= 0)
         m->Array[0] = (char*)m->TreeNode->tn_User;
      else
         m->Array[0] = "Tree";
      return 0;
   }
}

iptr MUITree::doSelect(void*, void*)
{
   void* data = 0;
   MUI_NListtree_TreeNode *tn = 0;
   tn = (MUI_NListtree_TreeNode*)DoMtd(list, ARRAY(MUIM_NListtree_GetEntry, 
                                                   (iptr)MUIV_NListtree_GetEntry_ListNode_Active,
                                                   (iptr)MUIV_NListtree_GetEntry_Position_Active,
                                                   0));
   if (tn != 0)
      data = tn->tn_User;

   return hSelect.Call(data, data);
}

iptr MUITree::addEntry(iptr parent, void* entry, bool active)
{
   if (0 == list)
      getObject();

   if (0 == list)
      return 0;

   if (0 == parent)
      parent = MUIV_NListtree_Insert_ListNode_Root;

   return DoMtd(list, ARRAY(MUIM_NListtree_Insert, (iptr)"*", (iptr)entry, parent, (unsigned)MUIV_NListtree_Insert_PrevNode_Tail, TNF_LIST | (active ? MUIV_NListtree_Insert_Flag_Active : 0)));
}

void MUITree::clear()
{
   hSelected.Empty();

   if (0 == list)
      getObject();

   if (0 == list)
      return;

   DoMtd(list, ARRAY(MUIM_NListtree_Clear, 0, 0));  
}

VectorT<void*> &MUITree::getSelectedObjects()
{
   MUI_NListtree_TreeNode *sel = (MUI_NListtree_TreeNode*)MUIV_NListtree_NextSelected_Start;

   hSelected.Empty();

   while (true)
   {
      DoMtd(list, ARRAY(MUIM_NListtree_NextSelected, (iptr)&sel));
      if (sel == (MUI_NListtree_TreeNode*)MUIV_NListtree_NextSelected_End)
         break;
      hSelected << sel->tn_User;
   }
   
   return hSelected;
}

void MUITree::showObject(void* data, bool expand)
{
   MUI_NListtree_TreeNode *tn;

   tn = (MUI_NListtree_TreeNode*)DoMtd(list, ARRAY(MUIM_NListtree_FindUserData, 
                                                   MUIV_NListtree_FindUserData_ListNode_Root, 
                                                   (iptr)data, 
                                                   MUIV_NListtree_FindUserData_Flag_Activate));
   if (expand)
   {
      DoMtd(list, ARRAY(MUIM_NListtree_Open, 
                        (iptr)MUIV_NListtree_Open_ListNode_Parent, 
                        (iptr)MUIV_NListtree_Open_TreeNode_Active));
   }
}

void MUITree::setEnabled(bool enabled)
{
   Intuition->SetAttrsA(listview, (TagItem*)ARRAY(MUIA_Disabled, !enabled, TAG_DONE, 0));
}


