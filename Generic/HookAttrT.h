#ifndef _GENERIC_HOOKATTRT_H
#define _GENERIC_HOOKATTRT_H

#include "Generic.h"
#include <utility/hooks.h>
#include <libclass/utility.h>
#include "Types.h"
#include <LibC/LibC.h>

/**
 * \file HookAttrT.h
 * \brief Specialized class for hook calling - frees you from casting, makes sure you give the right params.
 */

namespace GenNS
{
   /** 
    * \class HookAttrT<T1, T2>
    * \brief Specialized class for hook calling
    * \details Types \a T1 and \a T2 represent the parameter types your hook accepts.
    * An example of HookAttrT use is:
    * \code
    *    HookAttrT<Object*, iptr> myHook;
    *    ...
    *    myHook(myObject, ARRAY(OM_METHOD1, val, 0));
    * \endcode
    * To initialize the encapsulated class use either way presented below
    * \code
    * class myClass
    * {
    *    HookAttrT<void*, void*> h;
    * public:
    *    myClass(const Hook *hook) :
    *       h(hook)
    *    {
    *       // ...
    *    }
    *    // ...
    * }
    * \endcode
    *
    * \code
    * class myClass
    * {
    *    HookAttrT<void*, void*> h;
    * public:
    *    // ...
    *    void setHook(const Hook *hook)
    *    {
    *       h = hook;
    *    }
    *    // ...
    * }
    * \endcode
    *
    */
   template <typename T1, typename T2>
      class HookAttrT 
      {
         const struct Hook   *FHook;
      public:

         /**
          * \brief Default constructor. Verifies the validity of types \a T1 and \a T2, sets hook to 0.
          */
         HookAttrT()
         {
            ASSERT(sizeof(T1) == sizeof(iptr));
            ASSERT(sizeof(T2) == sizeof(iptr));
            FHook = 0;
         }

         /**
          * \brief Parametrized constructor. Verifies the validity of types \a T1 and \a T2, sets internal hook to the provided pointer.
          * \sa operator = (const struct Hook*)
          */
         HookAttrT(const Hook* h)
         {
            ASSERT(sizeof(T1) == sizeof(iptr));
            ASSERT(sizeof(T2) == sizeof(iptr));
            FHook = h;
         }

         virtual       ~HookAttrT()
         {
         }

         /**
          * \brief Sets the internal hook pointer to new value. 
          * \sa HookAttrT(const Hook*)
          */
         HookAttrT      &operator = (const struct Hook* AHook)
         {
            FHook = AHook;
            return *this;
         }

         /**
          * \brief Use this method to call hook with adequate parameters.
          * \sa operator ()(T1, T2)
          */
         iptr           Call(T1 object, T2 message) const
         {
            ASSERT(Utility != 0);
            if (FHook == 0) 
               return 0;
            return Utility->CallHookPkt(const_cast<Hook*>(FHook), (void*)(object), (void*)(message));
         }

         /**
          * \brief Verifies whether hook is initialized. 
          * \details You don't need to check this before calling the hook. The checks are done automatically.
          */
         bool           IsValid() const
         {
            return (FHook != 0);
         }

         /**
          * \brief Static method to call specific hook.
          * \details Useful if you need to call your hook in only one place - saves memory, increases speed.
          * Example usage:
          * \code
          *    // ...
          *    HookAttrT<void*, void*>::Call(hook, mem1, mem2);
          *    // ...
          * \endcode
          */
         static iptr   Call(const Hook* pHook, T1 object, T2 message)
         {
            ASSERT(Utility != 0);
            if (pHook == 0) 
               return 0;
            return Utility->CallHookPkt(const_cast<Hook*>(pHook), (void*)(object), (void*)(message));
         }

         /**
          * \brief Use this method to call hook with adequate parameters.
          * \sa Call(T1, T2)
          */
         iptr operator () (T1 object, T2 message) const
         {
            ASSERT(Utility != 0);
            if (FHook == 0) 
               return 0;
            return Utility->CallHookPkt(const_cast<Hook*>(FHook), (void*)(object), (void*)(message));
         }
      };
};
#endif //_GENERIC_HOOKATTRT_H
