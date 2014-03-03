#ifndef _H_cscript
#define _H_cscript

#include "moaxtra.h"
#include "mmixscrp.h"
#include "moafile2.h"
#include "moastr2.h"
#include "driservc.h"

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
	#include <Carbon.h>
#endif


// {25962D41-F339-11d1-9880-00A0242DCC6D}
DEFINE_GUID(CLSID(CScript), 0x25962d41, 0xf339, 0x11d1, 0x98, 0x80, 0x0, 0xa0, 0x24, 0x2d, 0xcc, 0x6d);

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CScript)
	PIMoaMmValue				pMmValue;
	PIMoaMmList					pMmList;
	PIMoaDrPlayer				pDrPlayer;
	PIMoaDrMovie 				pIMoaDrMovie;
	PIMoaDrMovieContext pIDrMovieContext;
	EventHandlerRef		  mRefLocalMouseHandler;
	int									mLastError;
	int									mLastValue;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaMmXScript)
	EXTERN_DEFINE_METHOD(MoaError, Call, (PMoaMmCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, startWheelMouse, (PMoaMmCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, closeWheelMouse, (PMoaMmCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, mouseWheelGetValue, (PMoaMmCallInfo))
EXTERN_END_DEFINE_CLASS_INTERFACE

enum 
{
m_new = 0,						/* standard */

m_startWheelMouse,
m_closeWheelMouse,
m_mouseWheelGetValue,

m_XXXX
};


#endif /* _H_cscript */
