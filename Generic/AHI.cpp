#include "LibrarySpool.h"
#include "AHI.h"
#include "Thread.h"
#include <exec/io.h>
#include <libclass/exec.h>
#include <exec/ports.h>
#include <devices/ahi.h>
#include <dos/dos.h>
#include <LibC/LibC.h>
#include "MsgCmd.h"

namespace GenNS
{
    enum AHICommand
    {
	cmd_NoOp       =  1000, // for initialization :]
	cmd_SetAudio,           // AudioID*
	cmd_FreeAudio,
	cmd_CreateStaticSample,
	cmd_CreateDynamicSample,
	cmd_FreeSample,
	cmd_PlaySample,
	cmd_SoundLoop,          // SoundInfo
	cmd_SoundStop,          // SoundInfo
	cmd_RequestAudio
    };

    struct ahi_CreateDynamicSample
    {
	AHI::AHISampleType	type;
	uint32		freq;
	const Hook*		func;
	uint32		buflen;
	uint16		result;
    };

    struct ahi_CreateStaticSample
    {
	AHI::AHISampleType	type;
	int			freq;
	void*		buffer;
	int			buflen;
	uint16		result;
    };

    struct ahi_RequestAudio
    {
	const char*		title;
	const TagItem*		tags;
	const AHI::AudioID*	result;
    };

    struct ahi_Sample
    {
	uint16	sample;
    };

    struct ahi_SetAudio
    {
	const AHI::AudioID*	audio;
	bool			result;
    };

    struct ahi_Channel
    {
	uint16	channel;
    };

    /*
    ** you don't want to use it...
    */
    class AHIMsg : public GenNS::Msg
    {
    public:
	AHICommand	cmd;
	union
	{
	    ahi_CreateDynamicSample	doCreateD;
	    ahi_CreateStaticSample	doCreateS;
	    ahi_Sample			doSample;
	    ahi_RequestAudio		doRequest;
	    ahi_SetAudio		doSetAudio;
	    ahi_Channel			doChannel;
	};

    public:
	AHIMsg(Port* p, AHICommand c) :
	    Msg(p),
	    cmd(c)
	{
	}
    };

};

using namespace GenNS;

AHI::AHI(short channels) :
    process(this, &AHI::subProcess),
    handler(this, &AHI::procCommand),
    music(this, &AHI::procMusicMessage),
    thread("AHI Process", handler, process)
{
    /*
    ** we will wait for reply on this port
    */
    Port p;
    AHIMsg m(&p, cmd_NoOp);

    pCurrentMode = 0;
    pAudio.Assign(0);

    numChannels = channels;
    if (numChannels > 32)
	numChannels = 32;

    for (int i=0; i<256; i++)
    {
	smpSample[i]   = 0;
    }

    for (int i=0; i<32; i++)
    {
	smpChan[i]  = 0;
	smpWaits[i] = 0;
    }

    hHkSound.Initialize(this, &AHI::procSoundMessage);

    // ensure everything is up and running
    thread.Start();
    thread.Send(&m);
    p.WaitReady();
    p.Recv();
}

AHI::~AHI()
{
    thread.Terminate();
    thread.WaitTerminated();
    hModes.ForEach(&freeAudioID);
}

bool AHI::freeAudioID(AudioID* const &id)
{
    delete id;
    return true;
}

iptr AHI::GetModeCount()
{
    return hModes.Count();
}

const AHI::AudioID *AHI::GetMode(iptr id)
{
    return hModes[id];
}

bool AHI::SetAudioMode(const AHI::AudioID *id)
{
    Port p;
    AHIMsg m(&p, cmd_SetAudio);
    m.doSetAudio.audio = id;
    thread.Send(&m);
    p.WaitReady();
    p.Recv();
    return m.doSetAudio.result;
}

const AHI::AudioID *AHI::GetAudioMode()
{
    return pCurrentMode;
}

void AHI::FreeAudio()
{
    /*
    ** no need for synchronous here.
    */
    AHIMsg *m = new AHIMsg(0, cmd_FreeAudio);
    thread.Send(m);
}

uint16 AHI::CreateDynamicSample(AHISampleType type, int freq, const Hook* func, int bufflen)
{
    Port p;
    AHIMsg m(&p, cmd_CreateDynamicSample);
    m.doCreateD.type = type;
    m.doCreateD.freq = freq;
    m.doCreateD.func = func;
    m.doCreateD.buflen = bufflen;
    thread.Send(&m);
    p.WaitReady();
    p.Recv();
    return m.doCreateD.result;
}

uint16 AHI::CreateStaticSample(AHISampleType type, int freq, void* buffer, int length)
{
    Port p;
    AHIMsg m(&p, cmd_CreateStaticSample);
    m.doCreateS.type = type;
    m.doCreateS.freq = freq;
    m.doCreateS.buffer = buffer;
    m.doCreateS.buflen = length;
    thread.Send(&m);
    p.WaitReady();
    p.Recv();
    return m.doCreateS.result;
}

void AHI::FreeSample(uint16 idx)
{
    /*
    ** no need for synchronous here.
    */
    AHIMsg *m = new AHIMsg(0, cmd_FreeSample);
    m->doSample.sample = idx;
    thread.Send(m);
}

void AHI::PlaySample(uint16 idx)
{
    /*
    ** no need for synchronous here.
    */
    AHIMsg *m = new AHIMsg(0, cmd_PlaySample);
    m->doSample.sample = idx;
    thread.Send(m);
}

const AHI::AudioID *AHI::RequestAudioMode(const char* title, struct TagItem* tags)
{
    Port p;
    AHIMsg m(&p, cmd_RequestAudio);
    m.doRequest.title = title;
    m.doRequest.tags  = tags;
    thread.Send(&m);
    p.WaitReady();
    p.Recv();
    return m.doRequest.result;
}

const AHI::AudioID *AHI::FindMode(iptr id)
{
    iptr idx;
    for (idx=0; idx<GetModeCount(); idx++)
    {
	if (GetMode(idx)->id == id)
	    return GetMode(idx);
    }
    return 0;
}

void AHI::subProcess(Thread *pThis)
{
    pAHI = AHIIFace::GetInstance();

    if (pAHI != 0)
    {
	for (iptr   i  =  pAHI->AHI_NextAudioID(AHI_INVALID_ID); 
	     i !=  AHI_INVALID_ID;
	     i  =  pAHI->AHI_NextAudioID(i))
	{
	    AudioID *pID = new AudioID;

	    pAHI->AHI_GetAudioAttrsA(i, 0, (TagItem*)ARRAY(
							   AHIDB_Name,       (int)&pID->name, 
							   AHIDB_BufferLen,  63,
							   0,                0));

	    pID->id     = i;
	    hModes << pID;
	}
    }


    pThis->HandleSignals(0xffffffff);

    do_FreeAudio();

    pAHI->FreeInstance();
    return;
}

void AHI::procCommand(const Port*, Msg* m)
{
    AHIMsg* c = static_cast<AHIMsg*>(m);

    switch (c->cmd)
    {
	case cmd_NoOp:
	    break;

	case cmd_SetAudio:
	    do_SetAudio(c->doSetAudio);
	    break;

	case cmd_FreeAudio:
	    do_FreeAudio();
	    break;

	case cmd_CreateDynamicSample:
	    do_CreateDynamicSample(c->doCreateD);
	    break;

	case cmd_CreateStaticSample:
	    do_CreateStaticSample(c->doCreateS);
	    break;

	case cmd_FreeSample:
	    do_FreeSample(c->doSample.sample);
	    break;

	case cmd_PlaySample:
	    do_PlaySample(c->doSample);
	    break;

	case cmd_SoundStop:
	    do_SoundStop(c->doChannel);
	    break;

	case cmd_SoundLoop:
	    do_SoundLoop(c->doChannel);
	    break;

	case cmd_RequestAudio:
	    do_RequestAudioMode(c->doRequest);
	    break;
    };

    c->Reply();
    return;
}

iptr AHI::procSoundMessage(AHIAudioCtrl* pCtl, AHISoundMessage*msg)
{
    ExtAHISampleInfo *pExt = smpChan[msg->ahism_Channel];

    // if this is not true we started nosound
    if (0 != pExt)
    {
	AHIMsg* m;
	if ((pExt->dynamic) && (pExt->fndsample->ahisi_Length))
	    m = new AHIMsg(0, cmd_SoundLoop);
	else
	    m = new AHIMsg(0, cmd_SoundStop);
	m->doChannel.channel = msg->ahism_Channel;
	thread.Send(m);
    }
    return 0;
}

void AHI::procMusicMessage(const Port*, GenNS::Msg* m)
{
    MsgCmd* mc = static_cast<MsgCmd*>(m);
    ExtAHISampleInfo* ext = (ExtAHISampleInfo*)mc->GetData();

    switch (mc->GetCmd())
    {
	case cmd_SoundLoop:
	    ext->ahisi_Length = ext->hData(ext->ahisi_Address, ext->maxlen);// / pExt->bps;
	    //Generic::MessageBox("Info", "Passing data\n%ld bytes requested,\n%ld bytes received.", "Ok", ARRAY(pExt->maxlen, pExt->ahisi_Length));
	    break;

    }
}

bool AHI::do_SetAudio(ahi_SetAudio &sa)
{
    AHIAudioCtrl *pCtl = 0;
    int           err;

    FAIL(0 == pAHI, "AHI could not be opened.")
	return false;

    if (0 != sa.audio)
    {
	if ((pCurrentMode !=0) && (pCurrentMode->id == sa.audio->id))
	    return true;
	do_FreeAudio();
    }
    else
    {
	do_FreeAudio();
	return true;
    }

    pCtl = pAHI->AHI_AllocAudioA((TagItem*)ARRAY(
						 AHIA_AudioID,        sa.audio->id,
						 AHIA_MixFreq,        44100,
						 AHIA_Channels,       numChannels,
						 AHIA_Sounds,         256,
						 AHIA_SoundFunc,      (int)hHkSound.GetHook(),
						 TAG_DONE,            0
						));

    if (0 != pCtl)
    {
	pAudio.Assign(pCtl);
	pCurrentMode = sa.audio;
	err = pAHI->AHI_ControlAudioA(pCtl, (TagItem*)ARRAY(AHIC_Play, true, 0, 0));
	if (0 != err)
	{
	    request("Error", "Unable to start audio device.\nError code %ld", "Proceed", ARRAY(err));
	}
	return true;
    }
    return false;
}

void AHI::do_FreeAudio()
{
    AHIAudioCtrl     *pCtl = 0;

    if (pCurrentMode == 0)
	return;
    if (0 == pAHI)
	return;

    pCtl = pAudio.Assign(0);
    if (0 != pCtl)
    {
	for (int i=0; i<256; i++)
	{
	    if (0 != smpSample[i])
	    {
		do_FreeSample(i);
	    }
	}
	pAHI->AHI_FreeAudio(pCtl);
    }
    pCurrentMode = 0;
}

void AHI::do_CreateDynamicSample(ahi_CreateDynamicSample &dat)
{
    ExtAHISampleInfo *pExt1 = do_AllocSample(true, dat.type, 0, dat.buflen);
    ExtAHISampleInfo *pExt2 = do_AllocSample(true, dat.type, 0, dat.buflen);

    if ((0 == pExt1) ||
	(0 == pExt2))
    {
	if (0 != pExt1)
	    do_FreeSample(pExt1->id);
	if (0 != pExt2)
	    do_FreeSample(pExt2->id);

	dat.result = AHI_NOSOUND;
	return;
    }

    pExt1->fndsample = pExt2;
    pExt2->fndsample = pExt1;

    pExt1->freq      = dat.freq;
    pExt2->freq      = dat.freq;

    pExt1->update    = new Thread("AHI Music", music);
    pExt2->update    = pExt1->update;
    pExt1->update->Start();

    pExt1->hData     = dat.func;
    pExt2->hData     = dat.func;

    dat.result = pExt1->id;
}

void AHI::do_CreateStaticSample(ahi_CreateStaticSample &dat)
{
    dat.result = AHI_NOSOUND;

    ExtAHISampleInfo *pExt = do_AllocSample(false, dat.type, dat.buffer, dat.buflen);

    if (pExt != NULL)
    {
	pExt->freq     = dat.freq;
	dat.result = pExt->id;
    }
}

AHI::ExtAHISampleInfo *AHI::do_AllocSample(bool dynamic, AHISampleType type, void* buffer, int buflen)
{
    if (NULL == pAHI)
	return 0;

    smpSync.Acquire();

    ExtAHISampleInfo *pInfo = 0;
    int               i;
    AHIAudioCtrl     *pCtl  = 0;

    for (i=0; i<256; i++)
    {
	if (smpSample[i] == 0)
	    break;
    }

    if (i < 256)
    {
	pInfo = new ExtAHISampleInfo;

	if (dynamic)
	{
	    buffer = new char[buflen];
	}

	switch (type)
	{
	    case AHI_Mono8:
		pInfo->bps  =  1;
		break;
	    case AHI_Mono16:
		pInfo->bps  =  2;
		break;
	    case AHI_Mono32:
		pInfo->bps  =  4;
		break;
	    case AHI_Stereo8:
		pInfo->bps  =  2;
		break;
	    case AHI_Stereo16:
		pInfo->bps  =  4;
		break;
	    case AHI_Stereo32:
		pInfo->bps  =  8;
		break;
	    default:
		pInfo->bps  =  1;
	}
	pInfo->ahisi_Address    = buffer;
	pInfo->ahisi_Length     = buflen / pInfo->bps;
	pInfo->ahisi_Type       = type;
	pInfo->id               = i;
	pInfo->freq             = 0;
	pInfo->dynamic          = dynamic;
	pInfo->fndsample        = 0;
	pInfo->update           = 0;
	pInfo->maxlen           = buflen;

	pCtl = pAudio.ObtainRead();
	if (NULL != pCtl)
	{
	    pAHI->AHI_LoadSound(i, (dynamic ? AHIST_DYNAMICSAMPLE : AHIST_SAMPLE), pInfo, pCtl);
	}
	pAudio.Release();

	smpSample[i] = pInfo;
    }

    smpSync.Release();

    return pInfo;
}

void AHI::do_FreeSample(uint16 idx)
{
    if (idx >= 256)
	return;

    if (0 == pAHI)
	return;

    smpSync.Acquire();
    if (0 != smpSample[idx])
    {
	ExtAHISampleInfo *pExt1 = smpSample[idx];
	ExtAHISampleInfo *pExt2 = pExt1->fndsample;

	// 1 CLEAR SAMPLE AND ITS FRIEND
	smpSample[idx] = 0;
	if (NULL != pExt2)
	    smpSample[pExt2->id] = 0;

	pExt1->update->Terminate();
	pExt1->update->WaitTerminated();
	delete pExt1->update;
	pExt1->update = 0;
	pExt2->update = 0;

	// 2 UNLOAD SAMPLE AND ITS FRIEND
	AHIAudioCtrl *pCtl = pAudio.ObtainRead();
	if (pCtl)
	{
	    pAHI->AHI_UnloadSound(idx, pCtl);
	    if (NULL != pExt2)
		pAHI->AHI_UnloadSound(pExt2->id, pCtl);
	}
	pAudio.Release();

	// 3 FREE MEMORY BUFFER FOR SAMPLE AND ITS FRIEND
	if (pExt1->dynamic == true)
	    delete [] ((char*)pExt1->ahisi_Address);
	if ((NULL != pExt2) &&
	    (pExt2->dynamic == true))
	    delete [] ((char*)pExt2->ahisi_Address);

	// 4 FREE SAMPLE AND ITS FRIEND
	delete pExt1;
	if (NULL != pExt2)
	    delete pExt2;
    }
    smpSync.Release();
}

void AHI::do_PlaySample(ahi_Sample &dat)
{
    int            idx = dat.sample;
    AHIAudioCtrl  *pCtl;
    bool           res = false;
    int            err;
    int            chan;

    if (idx > 255)
	return;
    if (idx < 0)
	return;

    if (0 == smpSample[idx])
	return;

    pCtl = pAudio.ObtainRead();
    if (pCtl != 0)
    {
	for (chan=0; chan<numChannels; chan++)
	{
	    if (smpChan[chan] == 0)
		break;
	}

	if (chan != numChannels)
	{
	    smpChan[chan] = smpSample[idx];

	    if (smpSample[idx]->dynamic != false)
	    {
		memset(smpSample[idx]->ahisi_Address, 0, smpSample[idx]->maxlen);
		memset(smpSample[idx]->fndsample->ahisi_Address, 0, smpSample[idx]->maxlen);
	    }

	    err = pAHI->AHI_PlayA(pCtl, (TagItem*)ARRAY(
							AHIP_BeginChannel,   chan,
							AHIP_Freq,           smpSample[idx]->freq,
							AHIP_Vol,            0x10000,
							AHIP_Pan,            0x8000,
							AHIP_Sound,          idx,
							AHIP_EndChannel,     0,
							TAG_DONE,            0));

	    if (err != 0)
	    {
		smpChan[chan] = 0;
		res = false;
	    }
	    else
	    {
		res = true;
	    }
	}
    }
    pAudio.Release();

    return;
}

void AHI::do_SoundStop(ahi_Channel& dat)
{
    int            chan = dat.channel;
    AHIAudioCtrl  *pCtl;

    if (chan > numChannels)
	return;
    if (chan < 0)
	return;

    smpSync.Acquire();
    pCtl = pAudio.ObtainRead();

    if (pCtl != NULL)
    { 
	smpChan[chan] = 0;
	pAHI->AHI_PlayA(pCtl, (TagItem*)ARRAY(
					      AHIP_BeginChannel,   chan,
					      AHIP_Sound,          AHI_NOSOUND,
					      AHIP_Vol,            65536,
					      AHIP_Pan,            32768,
					      AHIP_LoopSound,      0,
					      AHIP_EndChannel,     0,
					      TAG_DONE,            0));
	pAHI->AHI_SetSound(chan, AHI_NOSOUND, 0, 0, pCtl, 0);

	if (smpWaits[chan] != NULL)
	{
	    Exec->Signal(smpWaits[chan], 1 << (smpSignals[chan]));
	    smpWaits[chan] = 0;
	}
    }
    pAudio.Release();
    smpSync.Release();
}

void AHI::WaitSample(uint16 sample)
{
    bool bWait = false;
    int sig = Exec->AllocSignal(-1);
    Exec->SetSignal(0, 1 << sig);

    smpSync.Acquire();

    for (int i=0; i<numChannels; i++)
    {
	if ((smpChan[i] != 0) &&
	    (smpChan[i]->id == sample))
	{
	    smpWaits[i] = Exec->FindTask(0);
	    smpSignals[i] = sig;
	    bWait = true;
	}
    }

    smpSync.Release();

    if (true == bWait)
    {
	Exec->Wait(1 << sig);
    }
    Exec->FreeSignal(sig);
}

void AHI::do_SoundLoop(ahi_Channel& info)
{
    ExtAHISampleInfo *pExt = smpChan[info.channel];
    AHIAudioCtrl     *pCtl = pAudio.ObtainRead();
    MsgCmd*	     cmd = new MsgCmd(cmd_SoundLoop, pExt->fndsample);

    pAHI->AHI_SetSound(info.channel, pExt->fndsample->id, 0, 0, pCtl, 0);
    smpChan[info.channel] = pExt->fndsample;
    pExt->update->Send(cmd);
    pAudio.Release();
    return;
}

void AHI::do_RequestAudioMode(ahi_RequestAudio& am)
{
    struct TagItem *ti = TAGARRAY  
	(
	 AHIR_TitleText,		    (iptr)am.title,
	 am.tags ? TAG_MORE : TAG_DONE,  (iptr)am.tags
	);

    am.result = 0;

    struct AHIAudioModeRequester *aar = pAHI->AHI_AllocAudioRequestA(ti);

    if (aar)
    {
	if (pAHI->AHI_AudioRequestA(aar, 0))
	{
	    // this should go here!
	    am.result = FindMode(aar->ahiam_AudioID);
	}
	pAHI->AHI_FreeAudioRequest(aar);
    }
}

