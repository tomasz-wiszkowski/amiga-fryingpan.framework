#include <exec/libraries.h>

#undef PROC0
#undef PROC1
#undef PROC2
#undef PROC3
#undef PROC4
#undef PROC5
#undef PROC6
#undef PROC7
#undef PROC8
#undef PROC9
#undef PROC10
#undef PROC11
#undef PROC12
#undef FUNC0
#undef FUNC1
#undef FUNC2
#undef FUNC3
#undef FUNC4
#undef FUNC5
#undef FUNC6
#undef FUNC7
#undef FUNC8
#undef FUNC9
#undef FUNC10
#undef FUNC11
#undef FUNC12
#undef BEGINDECL
#undef BEGINSHORTDECL
#undef ENDDECL

extern class ExecIFace* Exec;

#if defined(__AMIGAOS4__)
   #include "Calls_os4.h"
#elif defined(__AROS__)
   #include "Calls_aros.h"
#elif defined(__MORPHOS__)
   #include "Calls_mos.h"
#else
   #include "Calls_os3.h"
#endif

