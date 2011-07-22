#include "Module.h"
#include <libclass/dos.h>

#include <LibC/LibC.h>

using namespace GenNS;

Module::Module(const char* name, iptr magic1, iptr magic2, uint16 ver, uint16 rev)
{
    iptr* loc = 0;
    iptr  size;
    iptr  next;

    valid   = false;
    header  = 0;
    segment = (iptr)DOS->LoadSeg(name);
    next    = segment;

    while ((0 == header) && (0 != next))
    {
	loc = (iptr*)BADDR(next);
	next = loc[0];
	size = loc[-1];

	request("Info", "Seg pos: %08lx / %ld", "Ok", ARRAY((iptr)loc, size));

	size /= sizeof(iptr);
	while (size > 0)
	{
	    if (loc[0] == magic1)
	    {
		request("Info", "%08lx -> %08lx %08lx %08lx\n", "Ok", ARRAY((iptr)loc, loc[0], loc[1], loc[2]));
	    }
	    if ((loc[0] == magic1) &&
		(loc[1] == magic2) &&
		(loc[2] == (iptr)loc))
	    {
		header = (Header*)loc;
		break;
	    }
	    --size;
	    ++loc;
	}
    }

    /*
     * have we found header?
     */
    if (0 == header)
	return;

    /* 
     * check version
     */
    if ((header->Version < ver) ||
	((header->Version == ver) && (header->Revision < rev)))
	return;

    valid = true;
}

bool Module::IsValid()
{
    return valid;
}

Module::~Module()
{
    if (segment)
	DOS->UnLoadSeg((BPTR)segment);
}

uint16 Module::Version()
{
    return header->Version;
}

uint16 Module::Revision()
{
    return header->Revision;
}

