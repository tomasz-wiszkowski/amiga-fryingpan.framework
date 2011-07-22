#include "Signal.h"
#include <libclass/exec.h>
#include <dos/dos.h>
#include <LibC/LibC.h>

using namespace GenNS;

Signal::Signal(int32 bitpos)
{
    FAIL(bitpos > 31, "Signal number out of range [0..31]")
    {
	bitpos = -1;
    }

    task = Exec->FindTask(0);

    /*
    ** if bitpos is negative, we want to allocate a new signal.
    */
    if (bitpos < 0)
    {
	signal = Exec->AllocSignal(-1);
	allocated = true;
	
	/*
	** signal allocation may have been unsuccessful
	*/
	FAIL(signal < 0, "Failed to allocate signal, will try to use system one.\nProgram behaviour may be erratic!")
	{
	    signal = SIGBREAKB_CTRL_F;
	    allocated = false;
	}
    }
    else
    {
	signal = bitpos;
	allocated = false;
    }
}

Signal::~Signal()
{
    if (allocated)
    {
	Exec->FreeSignal(signal);

	signal = -1;
	allocated = false;
    }
}

bool Signal::CheckSignal() const
{
    return (Exec->SetSignal(0, 0) & (1 << signal)) != 0;
}

void Signal::WaitSignal() const
{
    Exec->Wait(1 << signal);
}
void Signal::SendSignal() const
{
    Exec->Signal(task, 1 << signal);
}


