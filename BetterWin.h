#pragma once


#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

//defining macros to make sure we dont include some unnecessary headers (this speeds up building the project)
//things that we DO in fact need are commented out
//for reference on this ctrl + click <Windows.h>


#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
//#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES
//#define NOWINSTYLES 
#define NOSYSMETRICS 
#define NOMENUS
#define NOICONS
//#define NOKEYSTATES 
#define NOSYSCOMMANDS 
#define NORASTEROPS
//#define NOSHOWWINDOW - SW_ *
#define OEMRESOURCE //- OEM Resource values
#define NOATOM
#define NOCLIPBOARD // Clipboard routines
#define NOCOLOR //- Screen colors
#define NOCTLMGR // Control and Dialog routines
#define NODRAWTEXT // DrawText() and DT_ *
#define NOKERNEL // All KERNEL defines and routines
//#define NOUSER // All USER defines and routines
#define NONLS // All NLS defines and routines
//#define NOMB // MB_ * and MessageBox()
#define NOMEMMGR //- GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE
#define NOMINMAX  //Macros min(a, b) and max(a, b)
//#define NOMSG // typedef MSG and associated routines
#define NOOPENFILE //- OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL //- SB_ * and scrolling routines
#define NOSERVICE //- All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND // Sound driver routines
#define NOTEXTMETRIC
#define NOWH 
//#define NOWINOFFSETS // GWL_*, GCL_*, associated routines
#define NOCOMM // COMM driver routines
#define NOKANJI // Kanji support stuff.
#define NOHELP // Help engine interface.
#define NOPROFILER // Profiler interface.
#define NODEFERWINDOWPOS // DeferWindowPos routines
#define NOMCX // Modem Configuration Extensions
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#define STRICT

#include <Windows.h>