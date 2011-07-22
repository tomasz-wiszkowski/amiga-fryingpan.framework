#ifndef _LIBCLASS_ASL_H_
#define _LIBCLASS_ASL_H_ 

extern class ASLIFace *ASL;
#include <libclass/exec.h>
#undef MOS_DIRECT_OS
#include <GenericLib/Calls.h>
#include <libraries/asl.h>

struct TagItem;

BEGINDECL(ASL, "asl.library")
protected:
    FUNC0(FileRequester*,   __AllocFileRequest,	    5 );
    PROC1(		    __FreeFileRequest,	    6, FileRequester*, req, a0);
    FUNC1(bool,		    __RequestFile,	    7, FileRequester*, req, a0);
    FUNC2(void*,	    __AllocAslRequest,	    8, uint32, type, d0, TagItem*, tags, a0);
    PROC1(		    __FreeAslRequest,	    9, void*, req, a0);
    FUNC2(bool,		    __AslRequest,	   10, void*, req, a0, TagItem*, tags, a1);
    PROC0(		    __private0__,	   11 );
    PROC0(		    __private1__,          12 );
    PROC1(		    __AbortAslRequest,     13, void*, req, a0);
    PROC1(		    __ActivateAslRequest,  14, void*, req, a0);
public:

    inline ScreenModeRequester* AllocScreenModeRequest(TagItem* t)
    {
	return (ScreenModeRequester*)__AllocAslRequest(ASL_ScreenModeRequest, t);
    }

    inline FileRequester* AllocFileRequest(TagItem* t)
    {
	return (FileRequester*)__AllocAslRequest(ASL_FileRequest, t);
    }

    inline FontRequester* AllocFontRequest(TagItem* t)
    {
	return (FontRequester*)__AllocAslRequest(ASL_FontRequest, t);
    }

    inline void FreeAslRequest(ScreenModeRequester* r)
    {
	__FreeAslRequest(r);
    }

    inline void FreeAslRequest(FontRequester* r)
    {
	__FreeAslRequest(r);
    }

    inline void FreeAslRequest(FileRequester* r)
    {
	__FreeAslRequest(r);
    }

    inline bool AslRequest(ScreenModeRequester* r, TagItem* t)
    {
	return __AslRequest(r, t);
    }

    inline bool AslRequest(FontRequester* r, TagItem* t)
    {
	return __AslRequest(r, t);
    }

    inline bool AslRequest(FileRequester* r, TagItem* t)
    {
	return __AslRequest(r, t);
    }

    inline void ActivateAslRequest(ScreenModeRequester* r)
    {
	__ActivateAslRequest(r);
    }

    inline void ActivateAslRequest(FontRequester* r)
    {
	__ActivateAslRequest(r);
    }

    inline void ActivateAslRequest(FileRequester* r)
    {
	__ActivateAslRequest(r);
    }

    inline void AbortAslRequest(ScreenModeRequester* r)
    {
	__AbortAslRequest(r);
    }

    inline void AbortAslRequest(FontRequester* r)
    {
	__AbortAslRequest(r);
    }

    inline void AbortAslRequest(FileRequester* r)
    {
	__AbortAslRequest(r);
    }

ENDDECL

#endif 
