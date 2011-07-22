#include "Thread.h"
#include <exec/ports.h>
#include <dos/dos.h>
#include <exec/semaphores.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <libclass/utility.h>
#include <libclass/exec.h>
#include <libclass/dos.h>
#include <dos/dosextens.h>
#include "HookAttrT.h"
#include "String.h"
#include "Timer.h"
#include "LibrarySpool.h"

#ifndef __MORPHOS__
#define NP_CodeType  TAG_IGNORE
#define CODETYPE_PPC 0
#endif

using namespace GenNS;

void Thread::Start()
{
    Message  *m = 0;
    MsgPort  *p = 0;

    ULONG tags[] = {
	NP_Entry,      (ULONG)&__proc_init_code,
	NP_StackSize,  65536,
	NP_Name,       (iptr)hName.Data(),
	NP_CodeType,   CODETYPE_PPC,
	TAG_DONE,      0
    };

    Exec->InitSemaphore(&semaphore);
    pProcess = DOS->CreateNewProc((TagItem*)tags);    
    p = Exec->CreateMsgPort();
    m = new Message; 
    m->mn_Length         = sizeof(Message);     
    m->mn_Node.ln_Name   = (char*)this;
    m->mn_ReplyPort      = p;
    Exec->PutMsg(&pProcess->pr_MsgPort, m);
    Exec->WaitPort(p);
    m = Exec->GetMsg(p);
    Exec->DeleteMsgPort (p); 
    delete m;
}

void Thread::Terminate()
{
    if ((Exec->FindTask(0) == &pProcess->pr_Task) || (!Exec->AttemptSemaphore(&semaphore)))
    {
	Exec->Signal(&pProcess->pr_Task, lKillSignal);
    }
}

bool Thread::IsTerminated()
{
    if (Exec->AttemptSemaphore(&semaphore))
    {
	Exec->ReleaseSemaphore(&semaphore);
	return true;
    }
    return false;
}

void Thread::WaitTerminated()
{
    Exec->ObtainSemaphore(&semaphore);  
    Exec->ReleaseSemaphore(&semaphore);
}

iptr Thread::GetSignals()
{
    return lKillSignal | pTimer->GetSignals() | pPort->GetSignals();
}

bool Thread::HandleSignals(int32 ATimeout)
{
    return HandleSignals(ATimeout, 0);
}

bool Thread::HandleSignals(int32 ATimeout, uint32 ASignals)
{
    iptr lSignals = ASignals;
    iptr lRes;

    wasTimer = false;

    if ((ATimeout != 0) && (ATimeout != -1))
    {
	pTimer->AddRequest(ATimeout);   
    }
    else if (ATimeout == 0)
    {
	pTimer->AbortRequest();
    }

    lSignals |= GetSignals();
    do
    {
	pPort->HandleMsgs();             // if there are any pending messages, handle them at once
	if (ATimeout)                       // if timeout has been set...
	    lRes = Exec->Wait(lSignals);     // wait for any signal or timeout
	else                                // otherwise
	    lRes = Exec->SetSignal(0, lSignals); // just check signals

	if (lRes & lKillSignal)             // if we got killed
	    return true;                     // exit immediately 
	if (lRes &pTimer->GetSignals())
	    wasTimer = true;
	if (lRes &pPort->GetSignals())      // if we received a new message
	    pPort->HandleMsgs();          // take care of all messages
	if (lRes &~pPort->GetSignals())     // otherwise, if it was a timer or any other signal
	    return false;                    // quit & report
    } while (ATimeout);                    // eventually loop.
    return false;
}

bool Thread::IsTimerSignal()
{
    return wasTimer;
}

void Thread::__proc_init_code()
{
    Process          *p;                         // pointer to this process
    Message          *m;                         // pointer to received message
    Thread           *d;                         // pointer to the class

    p = (Process*) Exec->FindTask(0);            // get process message
    Exec->WaitPort(&p->pr_MsgPort);              // we expect our class pointer
    m = Exec->GetMsg(&p->pr_MsgPort);            // handle it
    d = (Thread*)m->mn_Node.ln_Name;             // get class pointer
    d->lKillSignal = 1<<Exec->AllocSignal(-1);   // we dont have to free it.
    d->pPort       = new Port(&d->handler);
    d->pTimer      = new Timer();
    Exec->ObtainSemaphore(&d->semaphore);
    Exec->ReplyMsg(m);

    d->task_entry.Call(d);

    delete d->pPort;
    delete d->pTimer;
    d->pProcess = 0;
    Exec->ReleaseSemaphore(&d->semaphore);
};

Thread::Thread(const char* name, const ICall2T<void, const Port*, Msg*> &hnd, const ICall1T<void, Thread*> &entry) :
    hName(name), 
    dflt_entry(this, &Thread::fDefaultHandler),
    task_entry(entry),
    handler(hnd)
{
};

Thread::Thread(const char* name, const ICall2T<void, const Port*, Msg*> &hnd) :
    hName(name), 
    dflt_entry(this, &Thread::fDefaultHandler),
    task_entry(dflt_entry),
    handler(hnd)
{
};

Thread::~Thread()
{
}

void Thread::Send(Msg* m)
{
    pPort->Send(m);
}

void Thread::ProcMessages()
{
    pPort->HandleMsgs();
}

void Thread::fDefaultHandler(struct Thread *pThis)
{
    pThis->HandleSignals(0xffffffff);
    return;
}
