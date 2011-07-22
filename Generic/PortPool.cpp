#include "PortPool.h"
#include <libclass/exec.h>
#include <dos/dos.h>
#include <LibC/LibC.h>

using namespace GenNS;

PortPool::PortPool()
{
    signal = Exec->AllocSignal(-1);
    FAIL(signal == -1, "Could not allocate spare signal, this application will fail to work");
    owner = Exec->FindTask(0);
}

PortPool::~PortPool()
{
    FAIL(owner != Exec->FindTask(0), "Port Pool can ONLY be used by owner task!");

    Exec->FreeSignal(signal);
}

const Port* PortPool::CreatePort(const ICall2T<void, const Port*, Msg*> *cback)
{
    FAIL(owner != Exec->FindTask(0), "Port Pool can ONLY be used by owner task!");

    Port* p = new Port(cback, signal);
    ports << p;

    return p;
}

void PortPool::DestroyPort(const Port* p)
{
    FAIL(owner != Exec->FindTask(0), "Port Pool can ONLY be used by owner task!");

    ports >> const_cast<Port*>(p);
    delete p;
}

uint32 PortPool::GetSignals()
{
    FAIL(owner != Exec->FindTask(0), "Port Pool can ONLY be used by owner task!");
    return 1 << signal;
}

void PortPool::HandleMsgs()
{
    FAIL(owner != Exec->FindTask(0), "Port Pool can ONLY be used by owner task!");

    for (uint32 i=0; i<ports.Count(); i++)
    {
	ports[i]->HandleMsgs();
    }
}

void PortPool::WaitReady()
{
    Exec->Wait(1 << signal);
}
