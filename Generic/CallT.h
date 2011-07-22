#ifndef _GENERIC_CALLT_H_
#define _GENERIC_CALLT_H_


#include "Types.h"

namespace GenNS
{
    /*
    ** interfaces
    */
    template <typename Result>
	class ICall0T
	{
	public:
	    virtual Result Call() const = 0;
	    virtual ~ICall0T() {};

	    Result operator()() const
	    {
		return Call();
	    }
	};

    template <typename Result, typename T1>
	class ICall1T
	{
	public:
	    virtual Result Call(T1 p1) const = 0;
	    virtual ~ICall1T() {};

	    Result operator()(T1 p1) const
	    {
		return Call(p1);
	    }
	};

    template <typename Result, typename T1, typename T2>
	class ICall2T
	{
	public:
	    virtual Result Call(T1 p1, T2 p2) const= 0;
	    virtual ~ICall2T() {};

	    Result operator()(T1 p1, T2 p2) const
	    {
		return Call(p1, p2);
	    }
	};

    template <typename Result, typename T1, typename T2, typename T3>
	class ICall3T
	{
	public:
	    virtual Result Call(T1 p1, T2 p2, T3 p3) const = 0;
	    virtual ~ICall3T() {};

	    Result operator()(T1 p1, T2 p2, T3 p3) const
	    {
		return Call(p1, p2, p3);
	    }
	};

    /*
    ** classes
    */
    template <typename Result, class Base>
	class Call0T : 
	    public ICall0T<Result>
	{
	    Base*   base;
	    Result  (Base::*pointer)();

	public:
	    Call0T(Base* b, Result (Base::*p)()) :
		base(b),
		pointer(p)	       
	    {
	    }

	    virtual Result Call() const
	    {
		return (base->*pointer)();
	    }
	};

    template <typename Result, class Base, typename par1>
	class Call1T : 
	    public ICall1T<Result, par1>
	{
	    Base*   base;
	    Result  (Base::*pointer)(par1);

	public:
	    Call1T(Base* b, Result (Base::*p)(par1)) :
		base(b),
		pointer(p)	       
	    {
	    }

	    virtual Result Call(par1 p1) const
	    {
		return (base->*pointer)(p1);
	    }
	};

    template <typename Result, class Base, typename par1, typename par2>
	class Call2T : 
	    public ICall2T<Result, par1, par2>
	{
	    Base*   base;
	    Result  (Base::*pointer)(par1, par2);

	public:
	    Call2T(Base* b, Result (Base::*p)(par1, par2)) :
		base(b),
		pointer(p)
	    {
	    }

	    virtual Result Call(par1 p1, par2 p2) const
	    {
		return (base->*pointer)(p1, p2);
	    }
	};

    template <typename Result, class Base, typename par1, typename par2, typename par3>
	class Call3T :
	    public ICall3T<Result, par1, par2, par3>
    {
	Base*	base;
	Result  (Base::*pointer)(par1, par2, par3);

    public:
	Call3T(Base* b, Result (Base::*p)(par1, par2, par3)) :
	    base(b),
	    pointer(p)	       
	{
	}

	virtual Result Call(par1 p1, par2 p2, par3 p3) const
	{
	    return (base->*pointer)(p1, p2, p3);
	}
    };

    /*
    ** classes that pass parameters to themselves
    */
    template <typename Result, class Base, typename par1>
	class Call1TP : 
	    public ICall0T<Result>, 
	    public ICall1T<Result, par1>
	{
	    Base*   base;
	    Result  (Base::*pointer)(par1);
	    par1    param1;

	public:
	    Call1TP(Base* b, Result (Base::*p)(par1)) :
		base(b),
		pointer(p)	       
	    {
	    }

	    Call1TP(Base* b, Result (Base::*p)(par1), par1 p1) :
		base(b),
		pointer(p),
		param1(p1)
	    {
	    }

	    virtual Result Call() const
	    {
		return (base->*pointer)(param1);
	    }

	    virtual Result Call(par1 p1) const
	    {
		return (base->*pointer)(p1);
	    }
	};

    template <typename Result, class Base, typename par1, typename par2>
	class Call2TP : 
	    public ICall0T<Result>, 
	    public ICall1T<Result, par1>, 
	    public ICall2T<Result, par1, par2>
	{
	    Base*   base;
	    Result  (Base::*pointer)(par1, par2);
	    par1    param1;
	    par2    param2;

	public:
	    Call2TP(Base* b, Result (Base::*p)(par1, par2)) :
		base(b),
		pointer(p)	       
	    {
	    }

	    Call2TP(Base* b, Result (Base::*p)(par1, par2), par1 p1) :
		base(b),
		pointer(p),
		param1(p1)
	    {
	    }

	    Call2TP(Base* b, Result (Base::*p)(par1, par2), par1 p1, par2 p2) :
		base(b),
		pointer(p),
		param1(p1),
		param2(p2)
	    {
	    }

	    virtual Result Call() const
	    {
		return (base->*pointer)(param1, param2);
	    }

	    virtual Result Call(par1 p1) const
	    {
		return (base->*pointer)(p1, param2);
	    }

	    virtual Result Call(par1 p1, par2 p2) const
	    {
		return (base->*pointer)(p1, p2);
	    }
	};

    template <typename Result, class Base, typename par1, typename par2, typename par3>
	class Call3TP :
	    public ICall0T<Result>, 
	    public ICall1T<Result, par1>, 
	    public ICall2T<Result, par1, par2>,
	    public ICall3T<Result, par1, par2, par3>
    {
	Base*	base;
	Result  (Base::*pointer)(par1, par2, par3);
	par1	param1;
	par2	param2;
	par3	param3;

    public:
	Call3TP(Base* b, Result (Base::*p)(par1, par2, par3)) :
	    base(b),
	    pointer(p)	       
	{
	}

	Call3TP(Base* b, Result (Base::*p)(par1, par2, par3), par1 p1) :
	    base(b),
	    pointer(p),
	    param1(p1)
	{
	}

	Call3TP(Base* b, Result (Base::*p)(par1, par2, par3), par1 p1, par2 p2) :
	    base(b),
	    pointer(p),
	    param1(p1),
	    param2(p2)
	{
	}

	Call3TP(Base* b, Result (Base::*p)(par1, par2, par3), par1 p1, par2 p2, par3 p3) :
	    base(b),
	    pointer(p),
	    param1(p1),
	    param2(p2),
	    param3(p3)
	{
	}

	virtual Result Call() const
	{
	    return (base->*pointer)(param1, param2, param3);
	}

	virtual Result Call(par1 p1) const
	{
	    return (base->*pointer)(p1, param2, param3);
	}

	virtual Result Call(par1 p1, par2 p2) const
	{
	    return (base->*pointer)(p1, p2, param3);
	}

	virtual Result Call(par1 p1, par2 p2, par3 p3) const
	{
	    return (base->*pointer)(p1, p2, p3);
	}
    };
};

#endif
