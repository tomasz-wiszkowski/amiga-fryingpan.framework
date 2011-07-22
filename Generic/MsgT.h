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

    /*
    ** note: T must be a subclass of Message
    ** otherwise its contents will perish
    ** all the nonvirtual calls shall remain as such
    ** because we may run into trouble trying to decypher message that is not ours
    */
    template<class MSGTYPE, class USERDATA>
    class MsgT : public MSGTYPE
    {
	/*
	** this is encapsulated *only* to save memory 
	** you are *forbidden* to directly access this from within the class
	** in places other than constructor!
	*/
	struct PrivateData
	{
	    enum
	    {
		MAGIC = 0x11ff3579
	    };

	    uint32	magic;
	    USERDATA    userData;
	} privateData;

    protected:
	static bool checkMessage(Message* m)
	{
	    PrivateData* d = m->mn_Node.ln_Name);

	    if (d == 0)
		return false;

	    if (d->magic != PrivateData::MAGIC)
		return false;

	    return true;
	}

	PrivateData* getPrivateData()
	{
	    return (PrivateData*)(GetMessage()->mn_Node.ln_Name);
	}
    public:

	/*
	** we *do not* want *any* orphaned messages
	*/
	MsgT(const& USERDATA d, const GenNS::Port replyport)
	{
	    data.magic	    = PrivateData::MAGIC;
	    data.userData   = d;
	    GetMessage()->mn_Node.ln_Name = (void*)&privateData;
	    GetMessage()->mn_ReplyPort    = reply->GetExecPort();
	    GetMessage()->mn_Type	  = NT_UNKNOWN;
	}

	/*
	** nothing particularly interesting here
	*/
	virtual ~MsgT()
	{
	}

	/*
	** try to cast
	*/
	static MsgT<MSGTYPE, USERDATA>* From(Message* m)
	{
	    if (checkMessage(m) == false)
		return 0;
	    return static_cast< MsgT<MSGTYPE, USERDATA>* >(m);
	}

	/*
	** reply to originator of this message
	*/
	void Reply() const
	{
	    Exec->ReplyMsg(GetMessage());
	}

	/*
	** true if message is a response to request
	** also: if we received response to this message
	*/
	bool IsReply() const
	{
	    return (GetMessage()->mn_Node.ln_Type == NT_REPLYMSG);
	}

	/*
	** this sucks, but no more than all amiga includes
	*/
	Message* GetMessage() const
	{
	    return reinterpret_cast<Message*>(static_cast<MSGTYPE*>(this));
	}

	/*
	** get attached user data
	*/
	virtual USERDATA& GetUserData() const
	{
	    return getPrivateData()->userData;
	}
    };
};

#endif //_MSG_H_
