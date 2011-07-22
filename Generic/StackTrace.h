#ifndef _GENERIC_STACKTRACE_H
#define _GENERIC_STACKTRACE_H

#include <Generic/VectorT.h>

namespace GenNS
{
    class StackTrace
    {
    public:
	static iptr* GetBackTraces();
    };
};
#endif
