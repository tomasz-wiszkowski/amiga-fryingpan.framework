#if !defined (__DEBUG_H) 
#define __DEBUG_H

#include "LibrarySpool.h"
#include "String.h"
#include "Thread.h"
#include "HookT.h"
#include <LibC/LibC.h>
#include "Synchronizer.h"

namespace GenNS
{
    class DbgMsg;
    class DbgHandler;
    class DbgMaster;

    enum DbgLevel
    {
	Lvl_Info    =  1,
	Lvl_Dump    =  2,
	Lvl_Debug   =  4,
	Lvl_Warning =  8,
	Lvl_Error   = 16,
	Lvl_Failure = 32
    };

    class DbgHandler
    {
	DbgHandler     *next;
	String          name;
	BPTR            fh1, fh2;
	bool		silent;
	bool		nodate;
	Task           *stask;
	Thread         &dest;
	uint16          flags;
	Synchronizer    sync;

    protected:
	void           PutDate() const;

    public:   
	DbgHandler(int, char*);
	~DbgHandler();
	void           DumpMsg(const class DbgMsg* const) const;
	void           Send(DbgLevel lvl, const char* sFmtString, sized_iptr *vFmtArgs, void* bMemBlock, int lMemLen) const;

	void           InitH();
	void           ExitH();
    };

    class DbgMaster 
    {
    private:
	Call2T<void, DbgMaster, const Port*, GenNS::Msg*> handler;

	Thread            thread;
	SignalSemaphore   ssem;
	Process          *proc;
	MsgPort          *MP;
	DbgHandler       *list;
	static DbgMaster *DebugMaster;
	static iptr       lNumInstances;


    protected:
	DbgMaster(void);
	virtual          ~DbgMaster(void);
	void		ProcMsgs(const Port* p, GenNS::Msg* m);

    public:
	static Thread    &GetDebug();
	static void       FreeDebug();
	static bool       CleanUp(void);
    };
};

#if defined( DEBUG ) 
#define _D(l, a, b...)        __dbg->Send(l, a, SIZEARRAY(b), 0, 0)
#define _DX(l, a, b...)       __dbg->Send(l, "%s: " a, SIZEARRAY((iptr)__PRETTY_FUNCTION__, b), 0, 0);
#define _DD(a,b)              __dbg->Send(Lvl_Dump, 0, 0, a, b)
#define _DDT(a, b, c)         __dbg->Send(Lvl_Dump, a, 0, b, c)
#define _ND(a)                __dbg = new DbgHandler(1, a);
#define _NDS(a)               __dbg = new DbgHandler(0, a);
#define USING_DEBUG           class GenNS::DbgHandler *__dbg
#define DECLARE_DEBUG         extern class GenNS::DbgHandler *__dbg;
#define DEFINE_DEBUG          class GenNS::DbgHandler *__dbg
#define DEBUG_ENGINE          __dbg
#define SET_DEBUG_ENGINE(v)   __dbg = (v)
#define _ED()                 delete __dbg

#define _createDebug(loud, name) setDebug(new DbgHandler(loud, name))
#define _destroyDebug()          delete getDebug();
#define _d(l, a, b...)           getDebug()->Send(l, a, SIZEARRAY(b), 0, 0);
#define _dx(l, a, b...)          getDebug()->Send(l, "%s: " a, SIZEARRAY((iptr)__PRETTY_FUNCTION__, b), 0, 0);
#define _dd(a, b)                getDebug()->Send(Lvl_Dump, 0, 0, a, b);
#define _ddt(a, b, c)            getDebug()->Send(Lvl_Dump, a, 0, b, c);
#else
#define _D(a...)  //
#define _ND(a)    //
#define _NDS(a)   //
#define _ED()     //
#define _DD(a, b) //
#define _DX(a...) //
#define USING_DEBUG // 
#define DEFINE_DEBUG //
#define DECLARE_DEBUG //
#define DEBUG_ENGINE ((DbgHandler*)0)
#define SET_DEBUG_ENGINE(v)  
#define _DDT(a, b, c)

#define _createDebug(loud, name)
#define _destroyDebug()
#define _d(a, b...)
#define _dx(l, a, b...)
#define _dd(a, b)
#define _ddt(a, b, c)
#endif

#endif


