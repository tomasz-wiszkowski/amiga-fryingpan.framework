#ifndef _GENERIC_AUTOPTR_H
#define _GENERIC_AUTOPTR_H

namespace GenNS
{
    template<class T>
    class AutoPtrT
    {
    protected:
	T*  ptr;

    public:
	AutoPtrT()
	{
	    ptr = 0;
	}

	AutoPtrT(T* x)
	{
	    ptr = x;
	}

	~AutoPtrT()
	{
	    Free();
	}

	T* operator->()
	{
	    return ptr;
	} 

	void operator=(T* x)
	{
	    Free();
	    ptr = x;
	}

	void Free()
	{
	    if (0 != ptr)
		delete ptr;
	    ptr = 0;
	}
	
	bool IsSet()
	{
	    return (0 != ptr) ? true : false;
	}
    };
};

#endif
