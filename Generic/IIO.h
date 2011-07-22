#ifndef _GENERIC_IIO_H
#define _GENERIC_IIO_H

#include "Types.h"

namespace GenNS
{
    class IIO
    {
    public:
	virtual iptr	GetDataAvail() = 0;
	virtual iptr	ReadData(void*, iptr) = 0;
	virtual iptr	WriteData(void*, iptr) = 0;
	virtual bool	IsOpened() = 0;
    };
};

#endif
