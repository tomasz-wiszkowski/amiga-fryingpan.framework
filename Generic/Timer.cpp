#include "Timer.h"
#include <libclass/exec.h>
#include <devices/timer.h>
#include <exec/ports.h>
#include "LibrarySpool.h"

using namespace GenNS;

Timer::Timer() 
{
   issued = 0;
   port = Exec->CreateMsgPort();
   treq = (timerequest*)Exec->CreateIORequest(port, sizeof(timerequest));
   Exec->OpenDevice("timer.device", UNIT_VBLANK, (IORequest*)treq, 0);
}

Timer::~Timer()
{
   AbortRequest();
   WaitRequest();
   Exec->CloseDevice((IORequest*)treq);
   Exec->DeleteIORequest((IORequest*)treq);
   Exec->DeleteMsgPort(port);
}

void Timer::WaitRequest()
{
   
   if (issued != 0) 
   {
      Exec->WaitPort(port);
      treq = (timerequest*)Exec->GetMsg(port);
      issued = 0;
   }
}

void Timer::AbortRequest()
{
   if (issued != 0)
   {
      Exec->AbortIO((IORequest*)treq);
      WaitRequest();
   }
}

void Timer::AddRequest(iptr duration)  // milliseconds ?
{
   /*
    * don't initiate another request unless this one is finished.
    */
   if (issued != 0)
   {
      struct timerequest *tr = (struct timerequest*)Exec->GetMsg(port);
      if (tr == 0)
         return;

      treq = tr;
      issued = 0;
   }
   treq->tr_node.io_Command = TR_ADDREQUEST;
   treq->tr_time.tv_secs  = (duration/1000);
   treq->tr_time.tv_micro = (duration % 1000) * 1000;
   Exec->SendIO((IORequest*)treq);
   issued = 1;
}

iptr Timer::GetSignals(void)
{
   return 1<<port->mp_SigBit;
}
