#include "StackTrace.h"
#include <libclass/exec.h>
#include <exec/tasks.h>

using namespace GenNS;

iptr* StackTrace::GetBackTraces()
{
    iptr* traces = new iptr[256];
    int i = 0;
    Task* thistask = Exec->FindTask(0);

#if defined(__mc68000)
    register iptr* rega5 asm("a5");
    iptr* trace = rega5;
//    asm volatile("mov.l a5,%0\n\t" : "=r"(trace) : :);

    while ((trace <= thistask->tc_SPUpper) &&
	   (trace >  thistask->tc_SPLower))
    {
	traces[i++] = trace[1];
	trace = (iptr*)trace[0];
    }

#elif defined(__MORPHOS__)
    struct ETask *etask = thistask->tc_ETask;
    register iptr* regr1 asm("r1");
    iptr* trace = regr1;

    trace = (iptr*)trace[0];

    while ((trace <= etask->PPCSPUpper) &&
	   (trace >  etask->PPCSPLower))
    {
	traces[i++] = trace[1];
	trace = (iptr*)trace[0];
    }
#endif
    traces[i++] = 0;
    return traces;
}
