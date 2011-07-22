#ifndef LIB_C_H_
#define LIB_C_H_

#define __NOLIBBASE__
#define NO_INLINE_STDARG
#undef  __USE_INLINE__

#include <intuition/classusr.h>                      
#include <intuition/classes.h>
#include <exec/semaphores.h>
#include "types.h"

extern struct SignalSemaphore __InternalSemaphore;
extern void*                  __InternalMemPool;

extern struct Library              *SysBase;
extern struct Library              *__InternalDOSBase;
extern struct Library              *__InternalIntuitionBase;
extern struct Library              *__InternalUtilityBase;
extern struct WBStartup            *__WBStartup;
extern uint32                       StartupFlags;

enum StartupFlag
{
   SF_NoWBStartup = 1
};

#ifdef __AMIGAOS4__
   extern struct ExecIFace            *IExec;
   extern struct DOSIFace             *__InternalDOSIFace;
   extern struct IntuitionIFace       *__InternalIntuitionIFace;
   extern struct UtilityIFace         *__InternalUtilityIFace;
#endif

#ifdef __cplusplus
extern void*   operator new(size_t size);
extern void*   operator new(size_t size, const struct Allocator *a);
extern void    operator delete(void* mem);
extern void*   operator new[](size_t lSize);
extern void*   operator new[](size_t lSize, const struct Allocator *a);
extern void    operator delete[](void* mem);
extern const struct Allocator* set_default_allocator(const struct Allocator *a);
#endif

#undef DoMethod
#undef DoMethodA
#undef DoSuperMethod
#undef DoSuperMethodA

#ifdef __cplusplus
extern "C"
{
#endif
   /* Startup Code */
   extern int     _go();                           // here we do have the exec base.
   extern void    __main();                        // can be replaced i suppose
   extern int     __setup();                       // setup proc, call it from your lib init. REQUIRES VALID EXECBASE
   extern void    __cleanup();                     // cleanup proc, call it from your lib expunge. REQUIRES VALID EXECBASE
   extern int     main();

   extern void    _error(const char*, iptr*);

   /* regular c code */
   extern void*   malloc(size_t size);
   extern void*   malloc_pooled(void* pool, size_t size);
   extern void    free(void* mem);
   extern void*   realloc(void*, size_t);
   extern long    __initmem();
   extern void    __pure_virtual(void);
   extern void    __cxa_pure_virtual(void);   
   
   extern char*   strcpy(char* dst, const char* src);
   extern char*   strncpy(char* dst, const char* src, size_t len);
   extern char*   strcat(char* dst, const char* src);
   extern char*   strncat(char* dst, const char* src, size_t len);
   extern int     strcmp(const char* str1, const char* str2);
   extern int     stricmp(const char* str1, const char* str2);
   extern int     strncmp(const char* str1, const char* str2, size_t len);
   extern int     strnicmp(const char* str1, const char* str2, size_t len);
   extern char*   strchr(const char* str, int chr);
   extern size_t  strlen(const char* str);

   extern void    bzero(void* mem, size_t len);
   extern void*   memset(void* mem, int content, size_t len);
   extern void*   memcpy(void* dest, const void* src, size_t length);
   extern void*   memmove(void* dest, const void* src, size_t length);
   extern int     memcmp(const void* src, const void* dst, size_t length);
   extern void    bcopy(const void* src, void* dest, size_t length);
   extern void    abort() __attribute__((noreturn));
 
   extern uint64  udiv6432(uint64, uint32);
   extern int64   div6432(int64, int32);
   extern uint64  udiv64(uint64, uint64);
   extern int64   div64(int64, int64);
   extern uint64  umod64(uint64, uint64);
   extern int64   mod64(int64, int64);

   extern uint64   __ashldi3(uint64, int32);
   extern uint64   __lshrdi3(uint64, int32);

   extern int64   __divdi3(int64, int64);
   extern int64   __moddi3(int64, int64);

   iptr          DoMethodA(Object* obj, iptr *message);
   iptr          DoSuperMethodA(Class* cl, Object* obj, iptr *message);

   int32          request(const char* title, const char* body, const char* gads, const iptr* params);

   const struct Allocator* SLAB_Init(uint8 last_shift, const uint32 config_array[]);
   void  SLAB_Cleanup(const struct Allocator*);
   void* SLAB_Alloc(const struct Allocator *sa, size_t bytes);
   void  SLAB_Free(const struct Allocator *sa, void* mem);

   /*
    * allocator definition
    */

   struct Allocator
   {
      void* (*alloc)(const struct Allocator* alc, size_t len);
      void  (*free)(const struct Allocator* alc,void* mem);
   };

   extern const struct Allocator DEFAULT_ALLOCATOR;

#ifdef __cplusplus
};
#endif

#ifndef NOASSERTS
   #define ASSERT(expr)          \
      if (!(expr))               \
      {                          \
         if (0 == request("ASSERTION FAILED!", "Assertion failed. "     \
                                      "Please report.\nCondition:\n" #expr          \
                                      "\nOccurence:\n%s\n" __FILE__ ", line %ld\n",     \
                                      "Continue|Abort", ARRAY((iptr)(__FUNCTION__), __LINE__)))           \
            abort();      \
      }        

   #define ASSERTS(expr, msg)    \
      if (!(expr))               \
      {                          \
         if (0 == request("ASSERTION FAILED!", "Assertion failed. "                \
                                      "Please report.\nCondition:\n" #expr                            \
                                      "\nOccurence:\n%s\n" __FILE__ ", line %ld\n"                        \
                                      "Message:\n%s" , "Continue|Abort", ARRAY((iptr)__FUNCTION__, __LINE__, (iptr)msg)))  \
            abort();             \
      }        

    #define FAIL(expr, msg, more...) \
	if ((expr) && (0 == request("Unexpected condition", "%s, %ld\n%s\n\n" msg, "OK", ARRAY((iptr)__FILE__, __LINE__, (iptr)#expr, more))))

    #define WARN(expr, msg, more...) \
	if ((expr) && (0 == request("Unexpected condition", "%s, %ld\n%s\n\n" msg, "OK|Abort", ARRAY((iptr)__FILE__, __LINE__, (iptr)#expr, more))))

    #define CKPT(msg, more...) \
	request("Checkpoint", "%s, %ld\n\n" msg, "OK", ARRAY((iptr)__FILE__, __LINE__, more))

#else
   #define ASSERT(expr)
   #define ASSERTS(expr, msg)

    #define FAIL(expr, msg, more...) \
	if ((expr) && (0 == request("Unexpected condition", msg, "OK", ARRAY(more))))

    #define WARN(expr, msg, more...) \
	if ((expr) && (0 == request("Unexpected condition", msg, "OK|Abort", ARRAY(more))))

    #define CKPT(msg, more...) \
	request("Checkpoint", msg, "OK", ARRAY(more))

#endif

#endif /*FUNCTIONS_H_*/
