#include <Generic/MapArrayT.h>
#include <Generic/HookT.h>
#include <Generic/Types.h>
#include <libclass/exec.h>
#include <libclass/dos.h>
#include <libclass/utility.h>
#include <utility/tagitem.h>
#include <LibC/LibC.h>


ExecIFace *Exec;
DOSIFace *DOS;
UtilityIFace *Utility;

uint32 StartupFlags = 0;

using namespace GenNS;

class Main
{
    MapArrayT<int>*	mar;

public:
   Main()
   {
       int ix;

       mar = new MapArrayT<int>(32);

       request("info", "adding 30 items", "ok", 0);
       for (int i = 0; i<30; i++)
       {
	   ix = mar->PutItem(i+1000);
	   DOS->VPrintf("New index: %ld\n", ARRAY(ix));
       }

       request("info", "count: %ld", "ok", ARRAY(mar->Count()));

       request("info", "disposing index 4, 9, 12", "ok", 0);
       mar->FreeItem(4);
       mar->FreeItem(9);
       mar->FreeItem(12);
       request("info", "count: %ld", "ok", ARRAY(mar->Count()));

       request("info", "checking items...", "ok", 0);
       for (int i=0; i<32; i++)
       {
	   DOS->VPrintf("Allocation status for idx %ld = %ld\n", ARRAY(i, mar->IsAllocated(i)));
	   DOS->VPrintf("	value: %ld\n", ARRAY((iptr)mar->GetItem(i)));
       }

       request("info", "releasing all others", "ok", 0);
       for (int i = 0; i < 32; i++)
       {
	   ix = (int)mar->FreeItem(i);
	   DOS->VPrintf("Releasing item %ld [%ld]\n", ARRAY(i, ix));
       }

       request("info", "overloading array", "ok", 0);
       for (int i=0; i<35; i++)
       {
	   mar->PutItem(0);
       }
   }

   ~Main()
   {
      delete mar;
   }
};

int main()
{
    Exec = ExecIFace::GetInstance(SysBase);
    DOS = DOSIFace::GetInstance(0);
    Utility = UtilityIFace::GetInstance(0);

    delete new Main;

    Utility->FreeInstance();
    DOS->FreeInstance();
    Exec->FreeInstance();
    return 0;
}
