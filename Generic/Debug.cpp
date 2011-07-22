#ifndef __LINUX__
#include "Debug.h"
#include <LibC/LibC.h>
#include <libclass/utility.h>
#include <libclass/dos.h>
#include <dos/datetime.h>
#include "Msg.h"

using namespace GenNS;

DbgMaster *DbgMaster::DebugMaster = 0;
iptr       DbgMaster::lNumInstances = 0; 

class GenNS::DbgMsg : public GenNS::Msg
{
public:
    enum MsgType
    {
	Msg_Init,
	Msg_Dump,
	Msg_Exit
    };
protected:
    MsgType		cmd;
    const DbgHandler*	parent;
    DbgLevel		level;
    String*		text;
    void*		data;
    uint16		len;
public:

    DbgMsg( MsgType	c, 
	    const DbgHandler *pParent, 
	    DbgLevel	lvl, 
	    const char* sFmtStr, 
	    sized_iptr* vFmtArgs, 
	    void*	pData, 
	    int		lLen, 
	    const Port* p) :
	Msg(p)
    {
	parent   = pParent;
	cmd	 = c;
	level	 = lvl;
	text     = 0;
	data     = 0;
	len      = 0;

	if (0 != sFmtStr)
	{
	    text = new String();
	    text->FormatStr(sFmtStr, vFmtArgs);
	}

	if (0 != pData)
	{
	    len  = lLen < 1024 ? lLen : 1024;
	    data = new char[lLen];
	    Exec->CopyMem(pData, data, lLen);
	}
    };

    virtual       ~DbgMsg()
    {
	if (0 != data) 
	    delete [] (char*)data;

	if (0 != text) 
	    delete text;
    };

    MsgType Cmd() const
    {
	return cmd;
    }

    const DbgHandler* Handler() const
    {
	return parent;
    }

    char  *Text() const
    {
	if (text == 0)
	    return 0;
	return text->Data();
    }

    void  *Data() const
    {
	return data;
    }

    uint16 Length() const
    {
	return len;
    };

    void   Perform() const
    {
	Handler()->DumpMsg(this);
    }
};

DbgHandler::DbgHandler(int loud, char *file) :
    next(0),
    name(file),
    fh1(0),
    fh2(0),
    silent(!loud),
    nodate(0),
    stask(Exec->FindTask(0)),
    dest(DbgMaster::GetDebug()),
    flags(~0)
{
    DbgMsg* m = new DbgMsg(DbgMsg::Msg_Init, this, Lvl_Info, 0, 0, 0, 0, 0);
    dest.Send(m);
}

void DbgHandler::InitH()
{
    BPTR tmp = 0;
    char var[128];

    sync.Acquire();

    if (0 < DOS->GetVar("TBS_LOGEVENTS", var, 64, 0))
    {
	String s(var);
	flags = s.ToLong();
    }

    if (0 < DOS->GetVar("TBS_VERBOSELOG", var, 64, 0))
    {
	String s(var);
	silent = (s.ToLong() == 0 ? true : silent);
    }
    else
	silent = true;

    if (0 < DOS->GetVar("TBS_LOGDIR", var, 128, 0))
    {
	tmp = DOS->Lock(var, ACCESS_READ);
	if (tmp != 0)
	    DOS->UnLock(tmp);
	else
	    strcpy(var, "RAM:");
    }
    else
	strcpy(var, "RAM:");

    String *i = new String();
    String *j = new String();

    *i = var;
    i->AddPath("logs");

    if (BPTR lock = DOS->Lock(i->Data(), ACCESS_READ)) 
    {
	DOS->UnLock(lock);
    } 
    else 
    {
	DOS->UnLock(DOS->CreateDir(i->Data()));
    }

    j->FormatStr("%s_%05ld.txt", ARRAY((iptr)name.Data(), Utility->GetUniqueID()));
    i->AddPath(j->Data());
    DOS->DeleteFile(i->Data());
    fh1 = DOS->Open(i->Data(), MODE_READWRITE);
    i->FormatStr("con:0/10//80/Debug: %s/auto/close/wait", ARRAY((ULONG)name.Data()));
    if (!silent) fh2 = DOS->Open(i->Data(), MODE_NEWFILE);
    delete i;
    delete j;
};

DbgHandler::~DbgHandler()
{
    dest.Send(new DbgMsg(DbgMsg::Msg_Exit, this, Lvl_Info, 0, 0, 0, 0, 0));
    sync.Acquire();
    DbgMaster::FreeDebug();
};

void DbgHandler::ExitH()
{
    if (fh2 != 0) 
	DOS->Close(fh2);

    if (fh1 != 0)
	DOS->Close(fh1);

    sync.Release();
}

void DbgHandler::PutDate() const
{
    if ((!fh1) && (!fh2)) 
	return;

    unsigned char  DT[16];
    unsigned char  TM[16];
    unsigned char  DAY[16];
    DateTime       CDT;

    CDT.dat_StrDate = (UBYTE*)&DT;
    CDT.dat_StrTime = (UBYTE*)&TM;
    CDT.dat_StrDay  = (UBYTE*)&DAY;
    CDT.dat_Format  = FORMAT_CDN;
    CDT.dat_Flags   = 0;

    DOS->DateStamp(&CDT.dat_Stamp);
    DOS->DateToStr(&CDT);
    if (fh2 != 0) DOS->VFPrintf(fh2, "[%s] ", ARRAY((ULONG)&TM));
    if (fh1 != 0) DOS->VFPrintf(fh1, "[%s] ", ARRAY((ULONG)&TM));
}

void DbgHandler::Send(DbgLevel l, const char* sFmtString, sized_iptr *vFmtArgs, void* bMemBlock, int lMemLen) const
{
    if (0 == this)
	return;

    if (!(flags & l))
	return;

    dest.Send(new DbgMsg(DbgMsg::Msg_Dump, this, l, sFmtString, vFmtArgs, bMemBlock, lMemLen, 0));
}

void DbgHandler::DumpMsg(const DbgMsg* const m) const
{
    if ((0 == fh1) &&
	    (0 == fh2))
    {
	return;
    }

    String *pS      = new String();

    if (0 != m->Data())
    {
	unsigned char *bMemBuf = (unsigned char*)m->Data();
	iptr           lCurr   = 0;
	iptr           lMax    = m->Length();

	*pS = "[**DATA**]  ===";
	if (0 != m->Text())
	{
	    *pS += " ";
	    *pS += m->Text();
	    *pS += " ===";
	}
	while (pS->Length() < 90)
	{
	    *pS += "=";
	}

	if (fh2 != 0) DOS->FPuts(fh2, pS->Data());
	if (fh1 != 0) DOS->FPuts(fh1, pS->Data());

	while (lCurr < lMax)
	{
	    int lLen;
	    int i;

	    lLen = lMax - lCurr;                   // calculate size of
	    if (lLen > 16) lLen = 16;              // data to be printed in this line

	    if (0 == (lCurr % 16))                 // place enter
	    {                                      // as we are
		if (fh2 != 0) DOS->FPuts(fh2, "\n");           // :)
		if (fh1 != 0) DOS->FPuts(fh1, "\n");              //
	    }                                      //

	    if (fh2 != 0) DOS->FPuts(fh2, "[**DATA**]  ");
	    if (fh1 != 0) DOS->FPuts(fh1, "[**DATA**]  ");

	    for (i=0; i<lLen; i++)
	    {
		if (fh2 != 0) DOS->VFPrintf(fh2, "%02lx ", ARRAY(bMemBuf[lCurr+i]));
		if (fh1 != 0) DOS->VFPrintf(fh1, "%02lx ", ARRAY(bMemBuf[lCurr+i]));
	    }

	    for (;i<17; i++)
	    {
		if (fh2 != 0) DOS->FPuts(fh2, "   ");
		if (fh1 != 0) DOS->FPuts(fh1, "   ");
	    }

	    for (i=0; i<lLen; i++)
	    {
		unsigned char c = bMemBuf[lCurr+i];
		if (((c>0x1f) && (c<0x80)) ||
			(c>0x9f))
		{
		    if (fh2 != 0) DOS->FPutC(fh2, c);
		    if (fh1 != 0) DOS->FPutC(fh1, c);
		}
		else
		{
		    if (fh2 != 0) DOS->FPutC(fh2, '.');
		    if (fh1 != 0) DOS->FPutC(fh1, '.');
		}
	    }

	    lCurr += lLen;
	}

	if (fh2 != 0) DOS->FPuts(fh2, "\n");                // :)
	if (fh1 != 0) DOS->FPuts(fh1, "\n");                   //
	if (fh2 != 0) DOS->FPuts(fh2, pS->Data());
	if (fh1 != 0) DOS->FPuts(fh1, pS->Data());

    }
    else
    {
	if (0 != m->Text())
	{
	    PutDate();
	    if (fh2 != 0) DOS->FPuts(fh2, m->Text());                 // no end-of-line here!
	    if (fh1 != 0) DOS->FPuts(fh1, m->Text());                    // no end-of-line here!
	}   
    }

    if (fh2 != 0) DOS->FPuts(fh2, "\n");
    if (fh1 != 0) DOS->FPuts(fh1, "\n");
    if (fh2 != 0) DOS->Flush(fh2);
    if (fh1 != 0) DOS->Flush(fh1);
    delete pS; 
}


DbgMaster::DbgMaster() :
    handler(this, &DbgMaster::ProcMsgs),
    thread("TBS Debug", handler)
{
    thread.Start();
}

DbgMaster::~DbgMaster(void)
{
    thread.Terminate();
    thread.WaitTerminated();
}

bool DbgMaster::CleanUp(void)
{
    if (0 == DbgMaster::DebugMaster)
	return true;

    if (0 != DbgMaster::lNumInstances)
	return false;

    delete DbgMaster::DebugMaster;
    DbgMaster::DebugMaster = 0;
    return true;
}

void DbgMaster::ProcMsgs(const Port* p, GenNS::Msg* msg)
{
    DbgMsg *m = static_cast<DbgMsg*>(msg);

    switch (m->Cmd())
    {
	case DbgMsg::Msg_Dump:
	    m->Perform();
	    break;

	case DbgMsg::Msg_Init:
	    const_cast<DbgHandler*>(m->Handler())->InitH();
	    break;

	case DbgMsg::Msg_Exit:
	    const_cast<DbgHandler*>(m->Handler())->ExitH();
	    break;
    }
    m->Reply();
    return;
}


Thread &DbgMaster::GetDebug()
{
    if (DbgMaster::DebugMaster == 0)
    {
	DbgMaster::DebugMaster = new DbgMaster();
    }
    ++DbgMaster::lNumInstances;
    return DbgMaster::DebugMaster->thread;
}

void DbgMaster::FreeDebug()
{
    --DbgMaster::lNumInstances;
}

#endif
