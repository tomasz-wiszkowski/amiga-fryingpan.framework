#include "DynList.h"
#include <libclass/exec.h>
#include "String.h"
#include "LibrarySpool.h"
#include <LibC/LibC.h>

using namespace GenNS;

void EList::InitMinList(struct MinList* mn)
{
   mn->mlh_Head       = (MinNode*)&mn->mlh_Tail;
   mn->mlh_TailPred   = (MinNode*)&mn->mlh_Head;
   mn->mlh_Tail       = 0;   
}

void EList::InitList(struct List* ln)
{
    InitMinList((struct MinList*)ln);
    ln->lh_Type = 0;
}

void EList::InitMinNode(struct MinNode* mn)
{
    mn->mln_Succ = 0;
    mn->mln_Pred = 0;
}

void EList::InitNode(struct Node* ln)
{
    InitMinNode((struct MinNode*)ln);
    ln->ln_Type = 0;
    ln->ln_Name = 0;
    ln->ln_Pri = 0;
}

// regular functions

EList::EList(MinList* pList)
{
   if (pList != 0)
   {
       PrvList	     = pList;
   }
   else
   {
       PrvList	     = new MinList;
       InitMinList(PrvList);
   }
}
 
EList::~EList()
{
}
   
void EList::Rem(struct MinNode* n)
{
    Exec->Remove((Node*)n);
}
   
iptr EList::GetCount()
{
   MinNode *pNode = PrvList->mlh_Head;
   iptr  lItems = 0;
   
   while (pNode->mln_Succ)
   {
      pNode = pNode->mln_Succ;
      ++lItems;
   }
   return lItems;
}

MinList *EList::GetList()
{
   return PrvList;
}

void EList::AddTail(MinNode* n)
{
    Exec->AddTail((List*)PrvList, (Node*)n);
}

void EList::AddHead(MinNode* n)
{
    Exec->AddHead((List*)PrvList, (Node*)n);
}

void EList::Enqueue(struct Node* n)
{
   Exec->Enqueue((List*)PrvList, n);  
} 
  
void EList::Insert(MinNode* n, MinNode* b)
{
    Exec->Insert((List*)PrvList, (Node*)n, (Node*)b);
}

