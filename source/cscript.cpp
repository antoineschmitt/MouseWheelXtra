#include "CarbonEvents.h"

#include "cscript.h"

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
	#include <stdio.h>
#endif

// Errors
typedef enum {
    ASMWERROR_ALREADYSTARTED = 1, 			// "MouseWheel Xtra already started"
    ASMWERROR_NOTSTARTED,								// "MouseWheel Xtra not started"
    ASMWERROR_INITERROR									// "MouseWheel Xtra initialisation error"
} asmwErrors;


// prototype
pascal OSStatus HandleMouseEvents(EventHandlerCallRef inCallRef, EventRef inEvent, void *userData );


/// ****************************************
// New Apple Event : kEventMouseScroll
// Should be implemented in carbon some time in the future...
// see : http://developer.apple.com/qa/qa2005/qa1453.html

enum
{
kEventParamMouseWheelSmoothVerticalDelta       = 'saxy', // typeSInt32
kEventParamMouseWheelSmoothHorizontalDelta     = 'saxx' // typeSInt32
};

/*
 *  kEventClassMouse / kEventMouseScroll
 *
 *  Summary:
 *    The user wants to scroll the object underneath the given mouse location
 *    (the window specified in the kEventParamWindowRef parameter)
 *    by the specified number of pixels.
 *
 *  Mac OS X threading:
 *    Not thread safe
 *
 *  Parameters:
 *
 *    --> kEventParamMouseLocation (in, typeHIPoint)
 *          The mouse location, in global coordinates.
 *
 *    --> kEventParamWindowRef (in, typeWindowRef)
 *          The window under the mouse.
 *
 *    --> kEventParamWindowMouseLocation (in, typeHIPoint)
 *          The window-relative position of the mouse in the window
 *          given in the kEventParamWindowRef parameter. 0,0 is at the
 *          top left of the structure of the window.
 *
 *    --> kEventParamWindowPartCode (in, typeWindowPartCode)
 *          The part code that the mouse location hit in the window.
 *          This parameter only exists if the WindowRef parameter
 *          exists. This saves you the trouble of calling FindWindow,
 *          which is expensive on Mac OS X as it needs to call the
 *          Window Server.
 *
 *    --> kEventParamKeyModifiers (in, typeUInt32)
 *          The keyboard modifiers that were pressed when the event was
 *          generated.
 *
 *    --> kEventParamMouseWheelSmoothVerticalDelta (in, typeSInt32)
 *          A typeSInt32 indicating how many pixels to scroll vertically.
 *          Do not multiply this by your line height the same way you would
 *          for the kEventParamMouseWheelDelta parameter of a
 *          kEventMouseWheelMoved event. Both this and the equivalent
 *          Horizontal parameter may be present in an event; if so, you
 *          should scroll both horizontally and vertically if you can.
 *
 *    --> kEventParamMouseWheelSmoothHorizontalDelta (in, typeSInt32)
 *          A typeSInt32 indicating how many pixels to scroll horizontally.
 *          Do not multiply this by your line height the same way you would
 *          for the kEventParamMouseWheelDelta parameter of a
 *          kEventMouseWheelMoved event. Both this and the equivalent
 *          Vertical parameter may be present in an event; if so, you
 *          should scroll both horizontally and vertically if you can.
 *
 *    --> kEventParamEventRef (in, typeEventRef)
 *          A typeEventRef of a compatibility kEventMouseWheelMoved event
 *          that corresponds to this event. This parameter may not always be
 *          present. This parameter is used by the event dispatching
 *          mechanism to make sure that objects which don't register for
 *          kEventMouseScroll events can see kEventMouseWheelMoved events
 *          instead. You may extract this event and examine its contents
 *          if your code requires a kEventMouseWheelMoved event
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventMouseScroll             = 11
};

/// ****************************************


/**************** The class ********************/
BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaMmXScript)
END_DEFINE_CLASS_INTERFACE

/* ----------------------------------- Interface Create/Destroy */
CScript_IMoaMmXScript::CScript_IMoaMmXScript(MoaError FAR * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CScript_IMoaMmXScript::~CScript_IMoaMmXScript() {}


//**********************************
//***************** CREATE/DESTROY
//**********************************

STDMETHODIMP MoaCreate_CScript(CScript FAR * pObj) {
	// Xtra creation
	
	// needed MOA interfaces
	pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&pObj->pMmValue);
	pObj->pCallback->QueryInterface(&IID_IMoaMmList, (PPMoaVoid)&pObj->pMmList);
	pObj->pCallback->QueryInterface(&IID_IMoaDrPlayer, (PPMoaVoid)&pObj->pDrPlayer);
	pObj->pIMoaDrMovie = NULL;
	pObj->pIDrMovieContext = NULL;
	pObj->pDrPlayer->GetActiveMovie(&pObj->pIMoaDrMovie);
	if (pObj->pIMoaDrMovie)
				pObj->pIMoaDrMovie->QueryInterface(&IID_IMoaDrMovieContext, (PPMoaVoid)&pObj->pIDrMovieContext);
	
	
	// my CarbonEvents callback
	pObj->mRefLocalMouseHandler = NULL;
	pObj->mLastValue = 0;
	
	return kMoaErr_NoErr;
}

STDMETHODIMP_(void) MoaDestroy_CScript(CScript FAR * pObj) {
	// cleanup
		
	if (pObj->mRefLocalMouseHandler)
		RemoveEventHandler(pObj->mRefLocalMouseHandler );
	pObj->mRefLocalMouseHandler = NULL;
	
	if (pObj->pIDrMovieContext)
		pObj->pIDrMovieContext->Release();
	pObj->pIDrMovieContext = NULL;
	
	if (pObj->pIMoaDrMovie)
			pObj->pIMoaDrMovie->Release();
	pObj->pIMoaDrMovie = NULL;
	
	if (pObj->pDrPlayer)
		pObj->pDrPlayer->Release();
	pObj->pDrPlayer = NULL;
	
	if (pObj->pMmValue)
		pObj->pMmValue->Release();
	pObj->pMmValue = NULL;
	
	if (pObj->pMmList)
		pObj->pMmList->Release();
	pObj->pMmList = NULL;
}

STDMETHODIMP
CScript_IMoaMmXScript::startWheelMouse(PMoaDrCallInfo callPtr) {  
	OSStatus				status;
		
	if (pObj->mRefLocalMouseHandler) {
		pObj->pMmValue->IntegerToValue(-ASMWERROR_ALREADYSTARTED, &callPtr->resultValue);	
		return kMoaErr_NoErr;
	}

	// listen to mouseWheel
	EventTypeSpec	mouseEvents[2] = {
		{ kEventClassMouse, kEventMouseWheelMoved	},
		{ kEventClassMouse, kEventMouseScroll	}	
	};

	// attach to the EventDispatcherTarget, which is the highest priority
	EventTargetRef targ = GetEventDispatcherTarget();
	status = InstallEventHandler(targ,
												NewEventHandlerUPP( HandleMouseEvents ),
												sizeof(mouseEvents)/sizeof(EventTypeSpec),
												mouseEvents, pObj, &pObj->mRefLocalMouseHandler	);

	if (status != kMoaErr_NoErr) {
		pObj->mLastError = -12;
		pObj->pMmValue->IntegerToValue(-ASMWERROR_INITERROR, &callPtr->resultValue);	
	} else {
		pObj->pMmValue->IntegerToValue(1, &callPtr->resultValue);	
	}

	
	return kMoaErr_NoErr;
}

STDMETHODIMP
CScript_IMoaMmXScript::closeWheelMouse(PMoaDrCallInfo callPtr) {
	if (!pObj->mRefLocalMouseHandler) {
		pObj->pMmValue->IntegerToValue(-ASMWERROR_NOTSTARTED, &callPtr->resultValue);	
		return kMoaErr_NoErr;
	}

	RemoveEventHandler(pObj->mRefLocalMouseHandler );
	pObj->mRefLocalMouseHandler = NULL;

	// ok	
	pObj->pMmValue->IntegerToValue(1, &callPtr->resultValue);	
	return kMoaErr_NoErr;
}


pascal OSStatus HandleMouseEvents(EventHandlerCallRef inCallRef, EventRef inEvent, void *userData ) {
	CScript FAR * pObj = (CScript FAR *)userData;

	SInt32 deltaPixels = 0;
  OSStatus status;
	
	// return eventNotHandledErr;
	
	if (GetEventKind(inEvent)  == kEventMouseWheelMoved) {
	  EventMouseWheelAxis axis;
  
	  status = GetEventParameter( inEvent, kEventParamMouseWheelAxis, typeMouseWheelAxis, NULL, sizeof(axis), NULL, &axis );
  	if (status != kMoaErr_NoErr) return eventNotHandledErr;

	  if ( axis != kEventMouseWheelAxisY ) return eventNotHandledErr;
	  	
		int oneClick = 120; // the amount for one click of the wheel
  	SInt32 delta;
		
  	status = GetEventParameter( inEvent, kEventParamMouseWheelDelta, typeLongInteger, NULL, sizeof(delta), NULL, &delta );
	  if (status != kMoaErr_NoErr) return eventNotHandledErr;
    deltaPixels = oneClick * delta;
	  
	} else if (GetEventKind(inEvent)  == kEventMouseScroll) {
		
  	status = GetEventParameter( inEvent, kEventParamMouseWheelSmoothVerticalDelta, typeLongInteger, NULL, sizeof(deltaPixels), NULL, &deltaPixels );
	  if (status != kMoaErr_NoErr) return eventNotHandledErr;
		
	} else {
		return eventNotHandledErr;
	}
			
	pObj->mLastValue = deltaPixels;
	
	// callback to lingo
	/* Prepare the pushContext, in order to call safely into Director */
	DrContextState drContextState;
	if (!pObj->pIDrMovieContext) {
		pObj->mLastError = -101;
	} else if (pObj->pIDrMovieContext->PushXtraContext(&drContextState) == kMoaErr_NoErr) {
		// call into Director OK

		MoaMmSymbol		sym; 
		MoaMmValue		intValue;
		pObj->pMmValue->StringToSymbol("WheelMouseEvent", &sym);
		pObj->pMmValue->IntegerToValue(deltaPixels, &intValue);
		pObj->pDrPlayer->CallHandler(sym, 1, &intValue, NULL);
		pObj->pMmValue->ValueRelease(&intValue);
		
		// End call into director
		pObj->pIDrMovieContext->PopXtraContext(&drContextState);
	}


   return eventNotHandledErr;
}


STDMETHODIMP
CScript_IMoaMmXScript::mouseWheelGetValue(PMoaMmCallInfo callPtr) {

	MoaError err = kMoaErr_NoErr;
	
	pObj->pMmValue->FloatToValue(pObj->mLastValue, &callPtr->resultValue);

	return kMoaErr_NoErr;
}


/* -------------------------------------------- CScript_IMoaMmXScript::Call */
STDMETHODIMP
CScript_IMoaMmXScript::Call(PMoaMmCallInfo callPtr) {
	

	switch	( callPtr->methodSelector ) {
			
		case m_startWheelMouse:
			startWheelMouse(callPtr);
			break;
			
		case m_closeWheelMouse:
			closeWheelMouse(callPtr);
			break;
			
		case m_mouseWheelGetValue:
			mouseWheelGetValue(callPtr);
			break;
										
	}

	return kMoaErr_NoErr;
}
