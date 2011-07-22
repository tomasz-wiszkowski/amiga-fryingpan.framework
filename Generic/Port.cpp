#include "Port.h"
#include "Msg.h"
#include "DynList.h"
#include <exec/ports.h>
#include <utility/hooks.h>
#include <libclass/exec.h>
#include "LibrarySpool.h"
#include <LibC/LibC.h>

using namespace GenNS;

#ifndef PF_SIGALLOC
#define PF_SIGALLOC (1<<7) /* aros needs this. */
#endif

Port::Port(const ICall2T<void, const Port*, Msg*> *cb, int8 sig) :
    call(cb)
{
    EList::InitNode(&mp_Node);
    EList::InitList(&mp_MsgList);
    mp_Flags = PA_SIGNAL;

    if (sig == -1)
    {
	mp_Flags |= PF_SIGALLOC;
	mp_SigBit = Exec->AllocSignal(-1);
    }
    else
    {
	mp_SigBit = sig;
    }
    mp_SigTask = Exec->FindTask(0);
}

Port::~Port()
{
    Message *pMsg;
    while ((pMsg = Exec->GetMsg(this)) != 0)
    {
	static_cast<Msg*>(pMsg)->Reply();
    }

    if (mp_Flags & PF_SIGALLOC)
    {
	Exec->FreeSignal(mp_SigBit);
    }
}

uint32 Port::GetSignals() const
{
    return (1<<mp_SigBit);
}

void Port::Send(Msg* m) const
{
    Exec->PutMsg(const_cast<MsgPort*>(static_cast<const MsgPort*>(this)), static_cast<Message*>(m));
}

GenNS::Msg* Port::Recv() const
{
    Message* m = Exec->GetMsg(const_cast<MsgPort*>(static_cast<const MsgPort*>(this)));
    if (m == 0)
	return 0;

    return static_cast<Msg*>(m);
}

void Port::HandleMsgs() const
{
    GenNS::Msg *pMessage;

    while(true)
    {
	pMessage = Recv();
	if (pMessage == 0) 
	    break;

	if (call != 0)
	    (*call)(this, pMessage);
    }  
}
      
void Port::WaitReady() const
{
    Exec->WaitPort(const_cast<MsgPort*>(static_cast<const MsgPort*>(this)));
}

