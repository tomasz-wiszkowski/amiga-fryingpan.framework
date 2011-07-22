#ifndef _DYNLIST_H_
#define _DYNLIST_H_

#include "Types.h"
#include <exec/lists.h>
#include <exec/nodes.h>

namespace GenNS
{
   class EList 
   {
   protected:
       MinList          *PrvList;

   public:
       EList(MinList* pList=0);
       ~EList();

       void	    Rem(struct MinNode* n);
       iptr	    GetCount();
       MinList	   *GetList();
       void	    AddTail(MinNode* n);
       void	    AddHead(MinNode* n);
       void	    Insert(MinNode* n, MinNode* b);

       /* enqueue is the only one that cannot work on min nodes */
       void	    Enqueue(struct Node* n);
   public: /* static stuff */
       static void	InitList(struct List* ln);
       static void	InitMinList(struct MinList* ln);
       static void	InitNode(struct Node* ln);
       static void	InitMinNode(struct MinNode* ln);
   };
};

#endif //_DYNLIST_H_
