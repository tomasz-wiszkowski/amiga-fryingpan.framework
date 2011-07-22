#ifndef _MSGCMD_H_
#define _MSGCMD_H_

#include "Msg.h"

/*
** feel free to extend this class to suit your demand
*/
namespace GenNS
{
    class MsgCmd : public Msg 
    {
	uint32	command;
	void*	data;
    public:
	MsgCmd(uint32 cmd, void* dat, const Port* p = 0) :
	    Msg(p)
	{
	    command = cmd;
	    data = dat;
	}

	virtual uint32 GetCmd() const
	{
	    return command;
	}

	virtual void* GetData() const
	{
	    return data;
	}

    };
};

#endif 
