#include "Msg.h"
#include <libclass/exec.h>
#include "Port.h"

using namespace GenNS;

GenNS::Msg::Msg(const GenNS::Port *reply)
{
    if (reply != 0)
	mn_ReplyPort = const_cast<MsgPort*>(static_cast<const MsgPort*>(reply));
    else
	mn_ReplyPort = 0;

    /*
    ** initially message has no state (not ready, not cancelled, not pending).
    */
    mn_Node.ln_Type = NT_UNKNOWN;
}

GenNS::Msg::~Msg()
{
}
	
void GenNS::Msg::Dispose()
{
    delete this;
}

void GenNS::Msg::Reply()
{
   if (mn_ReplyPort != 0)
   {
      Exec->ReplyMsg(this);
   }
   else
   {
      delete this;
   }
}

bool GenNS::Msg::WaitFor()
{
    if (mn_ReplyPort != 0)
    {
	if (IsPending())
	    Exec->WaitPort(mn_ReplyPort);
	return IsReady();
    }
    return false;
}

bool GenNS::Msg::Cancel()
{
    Exec->Forbid();
    if (IsPending())
    {
	Exec->Remove(&mn_Node);
	mn_Node.ln_Type = NT_FREEMSG;
    }
    Exec->Permit();
    return IsCancelled();
}

bool GenNS::Msg::IsPending()
{
    return (mn_Node.ln_Type == NT_MESSAGE);
}

bool GenNS::Msg::IsReplied()
{
    return (mn_Node.ln_Type == NT_REPLYMSG);
}

bool GenNS::Msg::IsReady()
{
    return (!IsPending());
}

bool GenNS::Msg::IsCancelled()
{
    return (mn_Node.ln_Type == NT_FREEMSG);
}


