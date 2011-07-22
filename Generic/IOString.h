#ifndef _GENERIC_IOSTRING_H
#define _GENERIC_IOSTRING_H

#include "IIO.h"
#include "String.h"

namespace GenNS
{
    class IOString
    {
    protected:
	IIO	    *stream;
	String	    str;
	char	    *buff;
	int	    len;
	int	    pos;
	int	    actual;

    public:
	IOString(IIO* io);
	~IOString();
	bool ReadString();
	void WriteString(const String& str);	
	operator const String&();
	String& Str();
    };
};

#endif
