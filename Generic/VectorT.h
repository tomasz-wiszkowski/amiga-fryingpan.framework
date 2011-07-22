#ifndef _VECTORT_H_
#define _VECTORT_H_

#include "Types.h"
#include <LibC/LibC.h>

namespace GenNS
{
    template <typename T, long minSize=16>
        class VectorT 
        {
            T*          pElements;
            uint32      lCurrentSize;
            uint32      lTotalSize;
            bool        bSorted;

        protected:
            void        qsort(T* arr, int size, int(func)(const T&, const T&))
            {
                if (size > 1)
                {
                    int s1  = 0;            // best result will be given
                    int s2  = size;         // ....
                    T*  ar2 = new T[size];  // when we use pointers here :]
                    T   el  = arr[size/2];  // middle element

                    for (int i=0; i<size; i++)       // so, for almost every element
                    {
                        if (i == size/2)              // almost - except for the middle one
                            continue;

                        if (func(arr[i], el) < 0)     // fill table from both ways
                            ar2[s1++] = arr[i];        // if smaller - from one end
                        else                          //
                            ar2[--s2] = arr[i];        // if greater - from another
                    }

                    ar2[s1] = el;                    // put last element :)

                    for (int i=0; i<size; i++)       // clone table
                        arr[i] = ar2[i];              //

                    delete [] ar2;                   // kill the temp

                    qsort(arr, s1, func);
                    qsort(&arr[s2], size-s2, func);
                }
            };

            void        expand()
            {
                ASSERT(lCurrentSize <= lTotalSize);
                if (lCurrentSize == lTotalSize)
                {
                    lTotalSize   <<= 1; // twice that much
                    T* pNewElem    = new T[lTotalSize];
                    T* pOldElem    = pElements;
                    for (uint32 i=0; i<lCurrentSize; i++)
                        pNewElem[i] = pElements[i];
                    pElements      = pNewElem;
                    delete [] pOldElem;
                }
            };

            static int  compare(const T &a, const T &b)
            {
                return (a < b) ? -1 :
                    (a == b) ? 0  : 1;
            }

        public:

            VectorT()
            {
                if (minSize < 4)
                    lTotalSize  = 4;
                else
                    lTotalSize  = minSize;

                lCurrentSize   = 0;
                pElements      = new T[lTotalSize];
                bSorted        = true;
            }

            VectorT(const VectorT<T, minSize> &src)
            {
                *this = src;
            }

            virtual ~VectorT()
            {
                delete [] pElements;
            }

            T& operator [] (int32 lElement) const
            {
                FAIL((lElement < (int32)-lCurrentSize) || (lElement >= (int32)lCurrentSize), 
                        "Array index out of bounds (%ld vs %ld)", lElement, lCurrentSize)
                {
                    /*
                    ** this is totally wrong.
                    */
                    return pElements[0];
                }

                if (lElement >= 0)
                    return pElements[lElement];
                else
                    return pElements[lCurrentSize + lElement];
            }

            T& Get(int lElement) const
            {
                return (*this)[lElement];
            }

            VectorT<T, minSize>& operator << (T pNewElement)
            {
                expand();

                pElements[lCurrentSize] = pNewElement;
                ++lCurrentSize;

                bSorted = false;

                return *this;
            }

            uint32 Count() const
            {
                return lCurrentSize;
            }

            uint32 Max() const
            {
                return lTotalSize;
            }

            void Empty()
            {
                bSorted = true;
                lCurrentSize = 0;
                lTotalSize = minSize;
                delete [] pElements;
                pElements = new T[minSize];
            }

            bool IsSorted() const
            {
                return bSorted;
            }

            VectorT<T, minSize>& operator >> (T pOldElement)
            {
                uint32 i;
                for (i=0; i<lCurrentSize; i++)
                {
                    if (pElements[i] == pOldElement)
                    {
                        break;
                    }
                }

                if (i == lCurrentSize)  // not found.
                    return *this;

                --lCurrentSize;

                for (; i<lCurrentSize; i++)
                {
                    pElements[i] = pElements[i+1];
                }

                // removing item does not change order;

                return *this;
            }

            T* GetArray() const
            {
                return pElements;
            }

            VectorT<T, minSize>& Sort(int(func)(const T&, const T&))
            {
                if (func == 0)
                    func = &compare;

                qsort(pElements, lCurrentSize, func);
                bSorted = true;
                return *this;
            }

            VectorT<T, minSize>& ForEach(bool(func)(const T&))
            {
                for (uint32 i=0; i<lCurrentSize; i++)
                {
                    if (!func(pElements[i]))
                        break;
                }

                return *this;
            }

            VectorT<T, minSize>& InsertElem(T elem, int32 pos)
            {
                ASSERT((pos >= 0) && (pos <= (int32)lCurrentSize));

                if (pos >= (int32)lCurrentSize)
                {
                    *this << elem;
                    return *this;
                }
                else if (pos < 0)
                {
                    return *this;
                }

                expand();
                for (int i=lCurrentSize; i>pos; --i)
                    pElements[i] = pElements[i-1];
                pElements[pos] = elem;
                lCurrentSize++;
                return *this;
            };

            VectorT<T, minSize>& RemoveElem(int32 pos)
            {
                ASSERT((pos >= 0) && (pos < lCurrentSize));
                *this >> pElements[pos];
                return *this;
            }

            VectorT<T, minSize>& InsertSorted(T elem, int(func)(const T&, const T&))
            {
                register int pos  = (lCurrentSize + 1) / 2;
                register int step = pos;

                if (func == 0)
                    func = &compare;

                do
                {
                    // left-check
                    if (pos > 0)
                    {
                        if (0 > func(elem, pElements[pos-1]))
                        {
                            step = (step+1) / 2;
                            pos -= step;
                            if (pos < 0)
                                pos = 0;
                            continue;
                        }
                    }

                    if (pos < (int32)lCurrentSize)
                    {
                        if (0 < func(elem, pElements[pos]))
                        {
                            step = (step+1) / 2;
                            pos += step;
                            if (pos > (int32)lCurrentSize)
                                pos = lCurrentSize;
                            continue;
                        }
                    }

                    break;
                }
                while (true);
                return InsertElem(elem, pos);
            }

            /** Returns Index of an element in vector, or -1 if not found.
             */
            int32 IndexOf(T elem, int(func)(const T&, const T&)) const
            {
                register uint32 pos  = (lCurrentSize) / 2;
                register uint32 step = (lCurrentSize+1) / 2;
                int res;
                int chance = 0;

                if (lCurrentSize == 0)
                    return -1;

                if (func == 0)
                    func = &compare;

                if (IsSorted())
                {
                    do
                    {
                        if (0 == func(elem, pElements[pos]))
                            return pos;

                        /*
                        * check if we have a match
                        */
                        res = func(elem, pElements[pos]);
                        if (res == 0)
                            return pos;
                        if ((pos == (lCurrentSize-1)) && (res > 0))
                            return -1;
                        if ((pos == 0) && (res == 0))
                            return -1;
                        if (step == 1)
                            ++chance;
                        if (chance == 3)
                            return -1;

                        step = (step+1) / 2;

                        if (0 > res)
                        {
                            pos -= step;
                            if (pos < 0)
                                pos = 0;
                        }
                        else
                        {
                            pos += step;
                            if (pos >= lCurrentSize)
                                pos = lCurrentSize-1;
                        }
                    }
                    while (true);
                }
                else
                {
                    for (uint32 i=0; i<Count(); i++)
                    {
                        if (0 == func(elem, pElements[i]))
                            return i;
                    }
                    return -1;
                }
            }

            VectorT<T, minSize>& operator =(const VectorT<T, minSize> &src)
            {
                lCurrentSize = src.lCurrentSize;
                lTotalSize   = src.lTotalSize;
                pElements    = new T[lTotalSize];
                bSorted      = src.bSorted;

                for (uint32 i=0; i<lCurrentSize; i++)
                {
                    pElements[i] = src.pElements[i];
                }

                return *this;
            }
        };
};

#endif //_VECTORT_H_
// vim: ts=3 et
