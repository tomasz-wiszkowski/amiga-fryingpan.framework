#ifndef _GENERIC_PORTPOOL_H
#define _GENERIC_PORTPOOL_H

#include "VectorT.h"
#include "Port.h"

namespace GenNS
{
    class PortPool
    {
	int32		signal;
	VectorT<Port*>	ports;
	Task*		owner;

    public:
	PortPool();
	~PortPool();

	const Port* CreatePort(const ICall2T<void, const Port*, Msg*> *cback);
	void	    DestroyPort(const Port* port);
	uint32	    GetSignals();
	void	    HandleMsgs();
	void	    WaitReady();
    };
};

#endif
