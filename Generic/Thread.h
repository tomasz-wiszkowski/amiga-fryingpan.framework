#ifndef _THREAD_H_
#define _THREAD_H_

#include "Port.h"
#include "HookAttrT.h"
#include "HookT.h"
#include "Types.h"
#include "String.h"
#include "CallT.h"

struct Process;



namespace GenNS
{
   class Thread 
   {
      struct Process*		pProcess;
      SignalSemaphore		semaphore;
      String			hName;
      class Timer*		pTimer;
      Port*			pPort;
      iptr			lKillSignal;
      bool			wasTimer;

      /*
      ** task entry & message handler
      */
      const Call1T<void, Thread, Thread*>	dflt_entry;
      const ICall1T<void, Thread*>&		task_entry;
      const ICall2T<void, const Port*, Msg*>&	handler;

   protected:
      static void             __proc_init_code();
      void           fDefaultHandler(struct Thread *pThis);
   public:

      void	Start();
      bool      HandleSignals(int32 ATimeout);
      bool      HandleSignals(int32 ATimeout, uint32 ASignals);
      void      Terminate();
      bool      IsTerminated();
      bool      IsTimerSignal();
      void      WaitTerminated();
      iptr      GetSignals();
      void	Send(Msg* m); 
      void      ProcMessages();
      Thread(const char* AName, const ICall2T<void, const Port*, Msg*> &handler);
      Thread(const char* AName, const ICall2T<void, const Port*, Msg*> &handler, const ICall1T<void, Thread*> &entry);
      ~Thread();
   };
};

#endif //_THREAD_H_
