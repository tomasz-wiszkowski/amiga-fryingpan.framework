#ifndef _MSG_H_
#define _MSG_H_

#include "Types.h"
#include <exec/ports.h>

/*
** feel free to extend this class to suit your demand
*/

namespace GenNS
{
    class Port;

    class Msg : public Message
    {
    protected:
	bool		    isCustom();
	struct PrivateData* getPrivateData();
    public:
	Msg(const GenNS::Port* replyport = 0);
	virtual		~Msg();
	static Msg*	From(Message*);
	void		Reply();
	bool		WaitFor();
	void		Dispose();
	bool		Cancel();

	bool		IsPending();
	bool		IsReplied();
	bool		IsReady();
	bool		IsCancelled();
    };
};

#endif //_MSG_H_
