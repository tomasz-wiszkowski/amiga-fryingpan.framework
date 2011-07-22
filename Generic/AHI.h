#ifndef _AHI_H_
#define _AHI_H_

#include "HookT.h"
#include "HookAttrT.h"
#include <libclass/ahi.h>
#include <devices/ahi.h>
#include "VectorT.h"
#include "RWSyncT.h"
#include "Synchronizer.h"
#include "CallT.h"
#include "Port.h"
#include "Msg.h"
#include "Thread.h"


namespace GenNS
{
   class Thread;
   class AHIMsg;
   struct ahi_RequestAudio;
   struct ahi_SetAudio;
   struct ahi_CreateStaticSample;
   struct ahi_CreateDynamicSample;
   struct ahi_Sample;
   struct ahi_Channel;


   class AHI 
   {
   public:
      struct AudioID
      {
         uint32	    id;
         const char name[64];
      };

      enum AHISampleType
      {
         AHI_Mono8      =  AHIST_M8S,
         AHI_Stereo8    =  AHIST_S8S,
         AHI_Mono16     =  AHIST_M16S,
         AHI_Stereo16   =  AHIST_S16S,
         AHI_Mono32     =  AHIST_M32S,
         AHI_Stereo32   =  AHIST_S32S
      };


   private:
      struct ExtAHISampleInfo : public AHISampleInfo
      {
         uint8             id;
         uint8             bps;
         uint32            freq;
         uint32            maxlen;
         bool              dynamic; 
         ExtAHISampleInfo *fndsample;
         HookAttrT<void*, iptr> hData;
         Thread           *update;
      };

   protected:
      const Call1T<void, AHI, Thread*>			process;
      const Call2T<void, AHI, const Port*, GenNS::Msg*> handler;
      const Call2T<void, AHI, const Port*, GenNS::Msg*> music;

      HookT<AHI, AHIAudioCtrl*, AHISoundMessage*>  hHkSound;

      Thread                           thread;
      AHIIFace                        *pAHI;
      VectorT<AudioID*>                hModes;
      short                            numChannels;

      RWSyncT<AHIAudioCtrl*>           pAudio;

      Synchronizer                     smpSync;

      ExtAHISampleInfo                *smpSample[256];
      ExtAHISampleInfo                *smpChan[32];
      Task                            *smpWaits[32];     // we will send SIGB_ABORT when it is ready :)
      unsigned char                    smpSignals[32];


      const AudioID*			pCurrentMode;
   protected:
      static bool	freeAudioID(AudioID* const &id);
      virtual void	subProcess(Thread*);
      virtual void	procCommand(const Port*, Msg*);   
      virtual iptr	procSoundMessage(AHIAudioCtrl*, AHISoundMessage*);
      virtual void	procMusicMessage(const Port*, GenNS::Msg*);

      virtual bool	do_SetAudio(ahi_SetAudio&);
      virtual void	do_FreeAudio();
      virtual void  	do_CreateDynamicSample(ahi_CreateDynamicSample&);
      virtual void  	do_CreateStaticSample(ahi_CreateStaticSample&);
      virtual void      do_FreeSample(uint16);
      virtual void      do_PlaySample(ahi_Sample&);
      virtual void      do_SoundStop(ahi_Channel&);
      virtual void      do_SoundLoop(ahi_Channel&);
      virtual void	do_RequestAudioMode(ahi_RequestAudio&);
      ExtAHISampleInfo *do_AllocSample(bool dynamic, AHISampleType type, void* buffer, int buflen);
   public:
      AHI(short channels);
      virtual          ~AHI();

      virtual iptr      GetModeCount();

      virtual const AudioID *GetMode(iptr index);
      virtual const AudioID *FindMode(iptr id);
      virtual const AudioID *GetAudioMode();
      virtual const AudioID *RequestAudioMode(const char* title, struct TagItem* tags);

      virtual bool      SetAudioMode(const AudioID *id);
      virtual void      FreeAudio();
      virtual uint16	CreateDynamicSample(AHISampleType type, int freq, const Hook* func, int bufflen);
      virtual uint16	CreateStaticSample(AHISampleType type, int freq, void* buffer, int length);
      virtual void      FreeSample(uint16);
      virtual void      PlaySample(uint16);
      virtual void      WaitSample(uint16);
   };
};

#endif

