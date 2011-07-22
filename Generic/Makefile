
#------------------------------------------------------------
# Section 1: defining objects, binaries and compile-time settings
#------------------------------------------------------------

OBJS = \
	DynList.o			\
	GenericOOP.o			\
	GenericMUI.o			\
	MUICustomClass.o		\
	LibrarySpool.o			\
	String.o			\
	HookAttrT.o     		\
	Thread.o			\
	Timer.o				\
	Port.o				\
	Msg.o				\
	Synchronizer.o			\
	RWSync.o			\
	RWSyncT.o			\
	NetSocket.o			\
	Vector.o			\
	VectorT.o         		\
	IHook.o				\
	HookT.o				\
	Debug.o				\
	XMLDocument.o			\
	DistributorT.o			\
	AHI.o				\
	Configuration.o			\
	ConfigParser.o			\
	Set.o				\
	SumMD5.o			\
	SumCRC.o			\
	Locale.o			\
	TagProperty.o			\
	IOString.o			\
	CycleBuffer.o			\
	FileReq.o			\
	Module.o			\
	MapArray.o			\
	Signal.o			\
	StackTrace.o			\
	PortPool.o			\
	MUI/MUI.o			\
	MUI/MUIList.o			\
	MUI/MUIPictureClass.o		\
	MUI/MUIPopup.o			\
	MUI/MUIPopAsl.o			\
	MUI/MUIPopString.o		\
	MUI/MUIPopText.o		\
	MUI/MUIToolBar.o		\
	MUI/MUITree.o			\
	MUI/MUIWindowMenu.o		\
	MUI/MUINumericClass.o		\
	MUI/MUIDynamicCycle.o		\
	MUI/MUIDynamicObject.o
	
 
Program			= libGeneric.a

TARGETS			= aros os3 morphos os4
OBJDIRS			= MUI
MAKEDIRS			= 
INSTALLDIR		= ../lib

INCOPTS			= -I../@Own/
LIBOPTS			= -nostdlib

AROS_LIBOPTS	= 
M68K_LIBOPTS	= 
MOS_LIBOPTS		= 
OS4_LIBOPTS		= 

BUILD_LIBRARY	= 1

	include ../Makefile.mk
