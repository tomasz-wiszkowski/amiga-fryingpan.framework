#ifndef _OPTICAL_OPTICAL_H
#define _OPTICAL_OPTICAL_H

#include <Generic/Types.h>
#include <PlugLib/IPluginT.h>
#ifdef __amigaos4__
#include <exec/interfaces.h>
#endif
#include <libclass/exec.h>

#undef MOS_DIRECT_OS
#include <GenericLib/Calls.h>

enum 
{
    /* unsigned long x[] = {COMMAND, PARAMS...};
     * result = OptDoMethodA(&x);
     *
     * COMMAND           CODE           // [FLGS][RESULT ]=[PARAMS,...] 
     */


    DRV_NoOP       =  0x000000,      // [....][       ]=[drive]
    DRV_ScanDevice,                  // [....][ScanData*]=[char*] call this to get a list of scsi devices found on a given device
    DRV_FreeScanResults,             // [....][error  ]=[ScanData* ] call this to release scan results.

    // now the dedicated commands
    DRV_NewDrive   =  0x100000,      // [....][drive  ]=[name, unit]
    DRV_CloneDrive,                  // [....][drive  ]=[drive     ]
    DRV_EndDrive,                    // [....][       ]=[drive]
    DRV_Format,                      // [....][error  ]=[drive, type]
    DRV_Blank,                       // [....][error  ]=[drive, type]
    DRV_ControlUnit,                 // [....][error  ]=[drive, type]
    DRV_PrepareWrite,                // [....][error  ]=[drive, type]
    DRV_AbortOperation,              // [....][-------]=[obsolete call]
    DRV_AddTrack,                    // [....][-------]=[obsolete call]
    DRV_CloseDisc,                   // [....][error  ]=[drive, type, track]
    DRV_GetAttrs,                    // [....][error  ]=[drive, tagarray]
    DRV_SetAttrs,                    // [....][error  ]=[drive, tagarray]
    DRV_SCSICommand,                 // [....][-------]=[obsolete call]
    DRV_PlayAudio,                   // [....][-------]=[obsolete call]
    DRV_PlayControl,                 // [....][-------]=[obsolete call]
    DRV_GetAttr,                     // [....][value  ]=[drive, attr]
    DRV_SetAttr,                     // [....][error  ]=[drive, attr, value]
    DRV_RepairDisc,                  // [....][error  ]=[drive, type], don't use it
    DRV_LayoutTracks,                // [....][error  ]=[drive, DiscItem*]
    DRV_UploadLayout,                // [....][error  ]=[drive, DiscItem*]
    DRV_StructureDisc,               // [....][error  ]=[drive]
    DRV_ReadTrackAbsolute,           // [....][error  ]=[drive, DiscItem*, startsec, count, buffer] always from track start
    DRV_ReadTrackRelative,           // [....][error  ]=[drive, DiscItem*, startsec, count, buffer] always relative to 0
    DRV_WriteTrackAbsolute,          // [....][error  ]=[drive, DiscItem*, startsec, count, buffer] always from track start
    DRV_WriteTrackRelative,          // [....][error  ]=[drive, DiscItem*, startsec, count, buffer] always relative to 0
    DRV_WriteSequential,             // [....][error  ]=[drive, data, numsectors]
    DRV_WaitForDisc,                 // [....][error  ]=[drive, timeout [s]]
    DRV_LockDrive,                   // [....][error  ]=[drive, DRT_LockDrive_*]
    DRV_ForceUpdate,                 // [....][error  ]=[drive]
};

enum 
{
    DRA_AddTagNotify = 0x80DA0000,         // [.S.] Hook*        // awaiting reimplementation [TBD]
    DRA_RemTagNotify,                      // [.S.] Hook*        // TBD
    DRA_DeviceName,                        // [..G] char*        // i.e. scsi.device
    DRA_UnitNumber,                        // [..G] ULONG        // i.e. 1
    DRA_IsOpened,                          // [..G] BOOL         // 1 if drive opened
    DRA_EnableDiag,                        // [.SG] BOOL         // allow unit self diag and detection [obsolete]
    DRA_Attention,                         // [.SG] BOOL         // disable all background ops [obsolete]
    DRA_LastError,                         // [NSG] EOpticalError

    DRA_Drive_ReadSpeeds =  0x81DA0000,    // [..G] UWORD*       // read speeds in kBps
    DRA_Drive_WriteSpeeds,                 // [..G] UWORD*       // write speeds in kBps
    DRA_Drive_CurrentOperation,            // [N.G] ULONG        // OP_*
    DRA_Drive_OperationProgress,           // [..G] UWORD        // progress in 0 .. 65535
    DRA_Drive_SenseData,                   // [..G] DriveSense*  // Drive sense data
    DRA_Drive_Status,                      // [N.G] ULONG        // DRT_DriveStatus_*
    DRA_Drive_AbsoluteCDAddress,           // [..G] LBA          // for CDAudio - absolute time
    DRA_Drive_RelativeCDAddress,           // [..G] LBA          // for CDAudio - relative time
    DRA_Drive_TestMode,                    // [.SG] BOOL         // writing in test mode
    DRA_Drive_Vendor,                      // [..G] char*/NULL   // drive vendor, i.e. TEAC
    DRA_Drive_Product,                     // [..G] char*/NULL   // drive product, i.e. CDW540E
    DRA_Drive_Version,                     // [..G] char*/NULL   // drive version, i.e. 1.0K
    DRA_Drive_Firmware,                    // [..G] char*/NULL   // drive firmware date
    DRA_Drive_ReadsMedia,                  // [..G] ULONG        // DRT_Media_*
    DRA_Drive_WritesMedia,                 // [..G] ULONG        // DRT_Media_*
    DRA_Drive_MechanismType,               // [..G] ULONG        // DRT_Mechanism_*
    DRA_Drive_Capabilities,                // [..G] ULONG        // DRT_Data_*
    DRA_Drive_BufferSize,                  // [..G] UWORD        // in kB
    DRA_Drive_BufferUsed,                  // [..G] UWORD        // amount [in kB] of buffer in use
    DRA_Drive_VolumeLevels,                // [..G] UWORD        // volume levels
    DRA_Drive_CurrentAudioTrackIndex,      // [..G] UBYTE:UBYTE  // audio track / index
    DRA_Drive_CurrentProfile,              // [..G] ULONG        // DRT_Profile_*
    DRA_Drive_CurrentReadSpeed,            // [.SG] UWORD        // this is what user wants
    DRA_Drive_CurrentWriteSpeed,           // [.SG] UWORD        // in kBps please!!!
    DRA_Drive_IsRegistered,                // [..G] BOOL         // 1 if regged, 0 if not.
    DRA_Drive_WriteParams,                 // [.SG] ULONG        // DRT_Write_*
    DRA_Drive_IsDiscInserted,              // [..G] BOOL
    DRA_Drive_LockStatus,		   // [N.G] DRT_LockDrive_*

    DRA_Disc_NumTracks = 0x82da0000,       // [..G] ULONG        // num tracks on disc
    DRA_Disc_Contents,                     // [..G] DiscItem*    // array of items
    DRA_Disc_ID,                           // [..G] ULONG        // disc id
    DRA_Disc_Size,                         // [..G] ULONG        // disc size
    DRA_Disc_State,                        // [..G] ULONG        // DRT_State_*
    DRA_Disc_LastSess_State,               // [..G] ULONG        // DRT_State_*
    DRA_Disc_Format,                       // [..G] ULONG        // DRT_DFormat_*
    DRA_Disc_BarCode,                      // [..G] ULONG[2]     // array ptr
    DRA_Disc_CatalogNumber,                // [..G] ??
    DRA_Disc_RW_Format,
    DRA_Disc_IsWritable,                   // [..G] BOOL
    DRA_Disc_IsErasable,                   // [..G] BOOL
    DRA_Disc_IsFormatable,                 // [..G] BOOL
    DRA_Disc_SubType,                      // [..G] ULONG        // DRT_SubType
    DRA_Disc_WriteMethod,                  // [.SG] ULONG        // DRT_WriteMethod_*
    DRA_Disc_IsOverwritable,               // [..G] BOOL
    DRA_Disc_NextWritableTrack,            // [..G] DiscItem*    // get first with NULL, ends with NULL
    DRA_Disc_CurrentDiscSeqNum,            // [..G] ULONG        // current disc sequential number. increases each update.
    DRA_Disc_Vendor,                       // [..G] const char*
    DRA_Disc_WriteProtect,                 // [.SG] BOOL         // set or get write protection - DVDRAM only
    DRA_Disc_AllowsWriteProtect            // [..G] BOOL         // use to check if write protection is allowed
};

enum EItemType
{
    Item_Disc = 0,
    Item_Session,
    Item_Track,
    Item_Index
};

enum EDataType 
{ 
    Data_Unknown = 0,    // 0000 bytes per sector, writing not supported
    Data_Audio,          // 2352 bytes per sector
    Data_Mode1,          // 2048 bytes per sector
    Data_Mode2,          // 2336 bytes per sector
    Data_Mode2Form1,     // 2048 bytes per sector
    Data_Mode2Form2      // 2324 bytes per sector
};

struct DiscSpeed
{
    uint8    type;	// refernce speed type - check below
    uint8    pad0;	// leave for future
    uint16   pad1;	// leave for future;
    uint8    begin_i;   // integer,
    uint8    begin_f;   // fraction (eg. 1.4x, 2.5x etc)
    uint8    end_i;	// integer
    uint8    end_f;	// fraction
    uint32   begin_kbps;// reference value to be used in Set Read/Write Speed
    uint32   end_kbps;	// as above
};

enum DIF_SpeedType
{
    DIF_Speed_Unknown	    = 0,    // unknown speed type
    DIF_Speed_CLV,		    // constant linear velocity
    DIF_Speed_CAV,		    // constant angular velocity
    DIF_Speed_ZCLV,		    // zone constant linear velocity
    DIF_Speed_CAVCLV		    // mixed CAV+CLV
};

enum DIF_Disc
{
    DIF_Disc_CloseSession    = 1,     // close last session (use multisession)
    DIF_Disc_CloseDisc       = 2,     // close disc after write (no multisession)
    DIF_Disc_MasterizeCD     = 4,     // write cd in SAO / DAO mode (cd must be empty)
};

enum DIF_Session
{
    DIF_Session_CDXA         = 1,     // for cd: write cdxa form1/2
    DIF_Session_CDI          = 2,     // for cd: write interactive videocd and such
};

enum DIF_Common
{
    DIF_Common_RelSize      = 4096,     // if set, size is relative to parent; 0 = beginning of parent, -1 = total parent blocks
    DIF_Common_RawData      = 8192,     // 2352 bytes/sector
    DIF_Common_SubChannel   = 16384,    // subchannel data, 96 bytes / sector extra
    DIF_Common_Type         = (DIF_Common_RawData | DIF_Common_SubChannel)
};

struct ScanData
{
    struct ScanData  *sd_Next;
    char             *sd_DeviceName;
    char             *sd_Vendor;
    unsigned char     sd_Unit;
    unsigned char     sd_Type;
} PACKED;

enum ELockType
{
    DRT_LockDrive_Unlock  = 0,
    DRT_LockDrive_Read    = 1,
    DRT_LockDrive_Write   = 2,
};

enum DRT_Format
{
    DRT_Format_Complete     = 0,
    DRT_Format_Fast,
};

enum DRT_Blank
{
    DRT_Blank_Complete      = 0,
    DRT_Blank_Fast,
    DRT_Blank_Session,
};

enum DRT_Control
{
    DRT_Unit_Stop           =  0,
    DRT_Unit_Start,
    DRT_Unit_Eject,
    DRT_Unit_Load,
    DRT_Unit_Idle,
    DRT_Unit_Standby,
    DRT_Unit_Sleep,
};

enum DRT_Mechanism
{
    DRT_Mechanism_Unknown   =  0,
    DRT_Mechanism_Caddy,
    DRT_Mechanism_Tray,
    DRT_Mechanism_Popup,
    DRT_Mechanism_Changer,
    DRT_Mechanism_Magazine
};

enum DRT_DriveStatus
{
    DRT_DriveStatus_Unknown = -1,
    DRT_DriveStatus_NotOpened,       /* drive not opened */
    DRT_DriveStatus_Ready,           /* device ready, idling */
    DRT_DriveStatus_NoDisc,          /* device not ready to perform media action */
    DRT_DriveStatus_Busy,            /* device busy performing uncategorized operation */
};

enum DRT_Operation
{
    /// drive main operations ///////
    DRT_Operation_Unknown = -1,      /* unknown operation */
    DRT_Operation_NoOperation = 0,   /* device busy performing uncategorized operation */
    DRT_Operation_Play,              /* playing audio disc */
    DRT_Operation_Pause,             /* play suspended */
    DRT_Operation_Read,              /* reading data */
    /// other operations ////////////

    ///***************************************** drive control
    DRT_Operation_Control_General = 0x1000,   /* generic control unit */
    DRT_Operation_Control_Eject,              /* ejecting tray */
    DRT_Operation_Control_Load,               /* loading tray */
    DRT_Operation_Control_SpinUp,             /* spinning up disc */
    DRT_Operation_Control_SpinDown,           /* stop spinning disc */
    DRT_Operation_Control_Standby,            /* put drive to standby mode */
    ///***************************************** other types may follow.

    ///***************************************** drive & disc analysis
    DRT_Operation_Analyse_General = 0x2000,   /* general analyze */
    DRT_Operation_Analyze_Drive,              /* analyze drive state and capabilities */
    DRT_Operation_Analyse_Tracks,             /* analyze track layout */
    DRT_Operation_Analyse_Indices,            /* analyze indices location */
    DRT_Operation_Analyse_DataType,           /* analyze data types */
    DRT_Operation_Analyse_CDText,             /* analyze cd text */
    DRT_Operation_Analyse_FreeDB,             /* get freedb details */
    DRT_Operation_Analyse_DiscID,             /* analyze disc id */
    DRT_Operation_Analyse_ControlBlocks,      /* analyze atip/dcb details */
    DRT_Operation_Analyse_Speeds,             /* analyze read/write speeds */
    DRT_Operation_Analyse_Layout,             /* layout tracks */
    ///***************************************** other types may follow.

    ///***************************************** different types of write
    DRT_Operation_Write_General = 0x3000,     /* write in general */
    DRT_Operation_Write_Calibrate,            /* laser calibration */
    DRT_Operation_Write_Allocate,             /* leadin */
    DRT_Operation_Write_CDText,               /* write cdtext */
    DRT_Operation_Write_Data,                 /* write track */
    DRT_Operation_Write_Synchronize,          /* sync cache */
    DRT_Operation_Write_CloseTrack,           /* close track */
    DRT_Operation_Write_CloseSession,         /* close session */
    DRT_Operation_Write_CloseDisc,            /* close disc */
    DRT_Operation_Write_Repair,		      /* repair disc */
    ///***************************************** other types may follow.

    ///***************************************** different types of erase
    DRT_Operation_Erase_General = 0x4000,     /* general erase */
    DRT_Operation_Erase_BlankComplete,        /* complete blanking */
    DRT_Operation_Erase_BlankFast,            /* quick blanking */
    DRT_Operation_Erase_BlankSession,         /* blank last session */
    DRT_Operation_Erase_FormatComplete,       /* complete formatting */
    DRT_Operation_Erase_FormatFast,           /* quick formatting */
    DRT_Operation_Erase_FormatSession,        /* format *new* session */

};

enum DRT_Close
{
    DRT_Close_Track               = 0,
    DRT_Close_Session,
    DRT_Close_Finalize,
};

enum DiscWriteMethod
{
    DRT_WriteMethod_Default    =  0,       // a write method that matches best.
    DRT_WriteMethod_TAO,                   // CD only
    DRT_WriteMethod_SAO,                   // CD only
    DRT_WriteMethod_Packet                 // CD-MRW and DVD only
};

enum DRT_Profile
{
    DRT_Profile_Unknown                  = 0,
    DRT_Profile_NoDisc,
    DRT_Profile_CD_ROM,
    DRT_Profile_CD_R,
    DRT_Profile_CD_RW,
    DRT_Profile_CD_MRW,
    DRT_Profile_DVD_ROM,
    DRT_Profile_DVD_MinusR,
    DRT_Profile_DVD_PlusR,
    DRT_Profile_DVD_MinusRW_Sequential,
    DRT_Profile_DVD_MinusRW_Restricted,
    DRT_Profile_DVD_PlusRW,
    DRT_Profile_DVD_RAM,
    DRT_Profile_DDCD_ROM,
    DRT_Profile_DDCD_R,
    DRT_Profile_DDCD_RW,
    DRT_Profile_BD_ROM,
    DRT_Profile_BD_R_Sequential,
    DRT_Profile_BD_R_PseudoOverwrite,
    DRT_Profile_BD_R_RandomWrite,
    DRT_Profile_BD_RW,
};

enum DRT_SubType
{
    DRT_SubType_Unknown                 = 0,

    DRT_SubType_CD_RW_LowSpeed          = 1,
    DRT_SubType_CD_RW_HighSpeed,
    DRT_SubType_CD_RW_UltraSpeed,

    DRT_SubType_DVD_DualLayer           = 1,

    DRT_SubType_CD_R_Quality1           = 1,  // best
    DRT_SubType_CD_R_Quality2,
    DRT_SubType_CD_R_Quality3,
    DRT_SubType_CD_R_Quality4,
    DRT_SubType_CD_R_Quality5,
    DRT_SubType_CD_R_Quality6,
    DRT_SubType_CD_R_Quality7,
    DRT_SubType_CD_R_Quality8,
    DRT_SubType_CD_R_Quality9,                // worst
};

enum DRT_Media
{
    DRT_Media_CD_ROM                  = 1,      
    DRT_Media_CD_R                    = 2,
    DRT_Media_CD_RW                   = 4,
    DRT_Media_DVD_ROM                 = 8,
    DRT_Media_DVD_MinusR              = 16,
    DRT_Media_DVD_PlusR               = 32,
    DRT_Media_DVD_MinusRW             = 64,
    DRT_Media_DVD_PlusRW              = 128,
    DRT_Media_DVD_RAM                 = 256,
    DRT_Media_DVD_PlusR_DL            = 512,
    DRT_Media_BD_ROM                  = 1024,
    DRT_Media_BD_R                    = 2048,
    DRT_Media_BD_RW                   = 4096
};

enum DRT_Capabilities
{
    DRT_Can_Read_Mode2             = 0x00000001,
    DRT_Can_Read_CDAudio           = 0x00000002,
    DRT_Can_Read_CDAudioAccurate   = 0x00000004,
    DRT_Can_Read_Mode2Form1        = 0x00000008,
    DRT_Can_Read_Mode2Form2        = 0x00000010,
    DRT_Can_Read_ISRC              = 0x00000020,
    DRT_Can_Read_MCN               = 0x00000040,
    DRT_Can_Read_BarCode           = 0x00000080,
    DRT_Can_Read_CDText            = 0x00000100,
    DRT_Can_Read_RAW               = 0x00000200,    // silly
    DRT_Can_Read_MountRainier      = 0x00000400,    // not implemented!
    DRT_Can_Read_MultiSession      = 0x00000800,
    DRT_Can_Write_MountRainier     = 0x00001000,    // not implemented
    DRT_Can_Write_CDText           = 0x00002000,
    DRT_Can_Play_Audio             = 0x00004000,
    DRT_Can_Do_SeparateMute        = 0x00008000,
    DRT_Can_Do_VolumeChange        = 0x00010000,
    DRT_Can_Do_SeparateVolume      = 0x00020000,
    DRT_Can_Do_WriteProtect        = 0x00040000,
    DRT_Can_Do_BurnProof           = 0x40000000,
    DRT_Can_Do_TestMode            = 0x80000000
};

enum EOpticalError 
{
    ODE_OK                  =  0,          // clear :D
    ODE_NoMemory            =  -1,         // not enough memory
    ODE_NoHandler           =  -2,         // unable to spawn handler [process]
    ODE_NoDevice            =  -3,         // there is no such device or unit
    ODE_InitError           =  -4,         // drive doesn't seem to respond to HD_SCSICMD
    ODE_BadDriveType        =  -5,         // drive is not a CD/DVD drive
    ODE_MediumError         =  -6,         // problem accessing medium
    ODE_IllegalType         =  -7,         // illegal type of action [like FORMAT] requested
    ODE_NoDevOpened         =  -8,         // no device has been opened
    ODE_CommandError        =  -9,         // device failed to execute a command
    ODE_NoFormatDescriptors =  -10,        // device does not specify any format IDs
    ODE_NoModePage          =  -11,        // no mode page is present
    ODE_DeviceBusy          =  -12,        // device is performing some operation and shouldn't be disturbed
    ODE_IllegalCommand      =  -13,        // this command shouldn't be issued here
    ODE_TooManyTracks       =  -14,        // track limit exceeded for writing
    ODE_TooManySessions     =  -15,        // session limit exceeded for writing (only one allowed)
    ODE_IllegalParameter    =  -16,        // illegal command parameter
    ODE_NoDisc              =  -17,        // no disc inside
    ODE_NotEnoughSpace      =  -18,        // not enough disc space or disc not writable
    ODE_DiscNotEmpty        =  -19,        // disc is not empty
    ODE_BadSessionNumber    =  -20,        // session number does not match
    ODE_BadTrackNumber      =  -21,        // track number does not match
    ODE_BadIndexNumber      =  -22,        // index number is wrong
    ODE_BadLayout           =  -23,        // when, say, we get session after tracks
    ODE_DiscFull            =  -24,        // disc is finalised, no spare space.
    ODE_DiscReserved        =  -25,        // disc is reserved.
    ODE_DiscNotFormatted    =  -26,        // disc has not been formatted yet.
    ODE_DeviceNotReady      =  -27,        // device not ready for operations.
    ODE_BadTrackMode        =  -28,        // the specified track mode is invalid
    ODE_BadBlockNumber      =  -29,        // bad block number or number beyond boundary
    ODE_Exiting             =  -30,        // when application is exiting. don't expect that too often.
    ODE_DriveLockedForWrite =  -31,
    ODE_DriveLockedForRead  =  -32,
    ODE_BadTransferSize     =  -33,        // for packet/incremental discs you have to send packets of fixed size, i.e. 16 sectors.
    ODE_NoDataReturned      =  -34,        // no data has been returned during previous transfer
};

#define Optical_Name		"optical.plugin"
#define Optical_Version		3
#define Optical_Revision	0

class Optical
{
public:
    virtual iptr	    DoMethodA(iptr* parm) = 0;
    virtual class IOptItem* CreateDisc() = 0;
};

typedef IPluginT<Optical*>    OpticalPlugin;

#endif
