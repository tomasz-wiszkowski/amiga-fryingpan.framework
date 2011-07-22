#ifndef _GENERIC_ICOMPARABLE_H
#define _GENERIC_ICOMPARABLE_H

namespace GenNS
{
    class IComparable
    {
    public:
	virtual bool operator < (const IComparable &oth) const = 0;
	virtual bool operator == (const IComparable &oth) const = 0;
	virtual bool operator > (const IComparable &oth) const
	{
	    return (!((*this < oth) || (*this == oth)));
	}

	virtual bool operator != (const IComparable &oth) const
	{
	    return (!(*this == oth));
	}
    };
};

#endif
