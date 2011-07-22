#ifndef _PORT_H_
#define _PORT_H_

#include "Synchronizer.h"
#include "Types.h"
#include "CallT.h"

namespace GenNS
{
    class Msg;

    class Port : public MsgPort
    {
	const ICall2T<void, const Port*, Msg*>* call;
    public:

	Port(const ICall2T<void, const Port*, Msg*> *cback = 0, int8 sig=-1);
	~Port();
	uint32   GetSignals() const;
	void	 Send(Msg *m) const;
	Msg*	 Recv() const;
	void	 HandleMsgs() const;
	void	 WaitReady() const;
    };
};

#endif //_PORT_H_
