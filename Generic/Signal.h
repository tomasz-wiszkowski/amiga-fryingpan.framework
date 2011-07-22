#ifndef _GENERIC_SIGNAL_H_
#define _GENERIC_SIGNAL_H_

#include "Types.h"
#include <exec/tasks.h>

namespace GenNS
{
    class Signal
    {
	Task*	task;
	int8	signal;
	bool	allocated;

    public:

	/*
	** if bitnum is -1, we will allocate signal bit
	** otherwise, this number will be triggered
	*/
	Signal(int32 bitpos = -1);
	~Signal();
	bool CheckSignal() const;
	void WaitSignal() const;
	void SendSignal() const;
    };
};

#endif
