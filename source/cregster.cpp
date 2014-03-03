
#ifndef INITGUID        /* INITGUID causes the actual GUIDs to be declared, */ 
	#define INITGUID      /* not just references to them. Must be declared in */
#endif                  /* only one file. */          

#include "cregster.h"
#include "dversion.h"
#include "xclassver.h"

BEGIN_DEFINE_CLASS_INTERFACE(CRegister, IMoaRegister) 
END_DEFINE_CLASS_INTERFACE

/*******************************************************************************
 * SCRIPTING XTRA MESSAGE TABLE DESCRIPTION.
 *
 * The general format is:
 * xtra <nameOfXtra>
 * new object me [ args ... ]
 * <otherHandlerDefintions>
 * --
 * The first line must give the name of the Scripting xtra.
 * The remaining lines give the names of the handlers that this xtra implements
 * along with the required types of the arguments for each handler.
 * 
 * -- Pre-defined handler new 
 * The new handler will be called when a child object is created, 
 * the first argument is always the child object and you defined any remaining arguments.
 * The new handler is the place to initialize any memory required by the child object.
 * 
 * -- Simple Handler Definitions
 * Each handler definition line is format like this:
 * <handlerName> <argType1> <argName1>, <argType2> <argName2> ...
 * The first word is the handler name. Following this are types description for
 * the argument to the handler, each separated by a comma.
 * The argument name <argName>, may be omited.
 * Permited argument types are: 
 * 	integer 
 * 	float
 * 	string
 * 	symbol
 * 	object
 * 	any
 * 	*
 * For integer, float, string, symbol, and object, the type of the argument must 
 * match. The type any means allow any type. The asterisk (*) means any number and 
 * any type of arguments.
 * 
 * The first argument is the child object and is always declared to be of type object.
 * 
 * -- Global Handlers
 * An asterisk (*) preceeding the handler name signifies a global handler.
 * This handler is at the global scope level and can be called from any
 * movie.
 * 
 * -- Xtra level handlers
 * A plus (+) preceeding the handler name signifies an Xtra level handler. 
 * This kind of handler can be called directly from the Xtra reference,
 * without creating a child object.
 * 
 * The enumerated list in beeplngo.H must correspond directly with the msgTable 
 * (i.e. they must be in the same order).
 * 
 *******************************************************************************/ 
 
static char msgTable[] = { 
	"xtra MouseWheel\n" \
	"-- version " VER_VERSION_STRING "\n"
	"new object me, *\n"  /* standard first handler entry in all message tables */
 	"-- All functions return 1 on success or error description if error--\n"
 	"*startWheelMouse -- start\n"
 	"*closeWheelMouse -- stop\n"
 	"/*mouseWheelGetValue -- get\n"
};


/* ---------------------------------------------------- MoaCreate_CRegister */
STDMETHODIMP MoaCreate_CRegister(CRegister FAR * This)
{
  MoaError err = kMoaErr_NoErr;
  return(err);
}

/* --------------------------------------------------- MoaDestroy_CRegister */
STDMETHODIMP_(void) MoaDestroy_CRegister(CRegister FAR * This)
{
  return;
}

/* ---------------------------------- CRegister_IMoaRegister Create/Destroy */
CRegister_IMoaRegister::CRegister_IMoaRegister(MoaError FAR * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CRegister_IMoaRegister::~CRegister_IMoaRegister() {}


/* ------------------------------------------- CRegister_IMoaRegister::Register */
STDMETHODIMP_(MoaError) CRegister_IMoaRegister::Register(PIMoaCache pCache, 
	PIMoaDict pXtraDict)
{

	/* variable declaration */
	MoaError 	err = kMoaErr_NoErr;
	PIMoaDict 	pRegDict;

	/* Register the scripting xtra */
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CScript, &IID_IMoaMmXScript, &pRegDict);
			
	if ( err == kMoaErr_NoErr)
	{
		/* Register the method table */
		err = pRegDict->Put(kMoaMmDictType_MessageTable, msgTable, 0, kMoaMmDictKey_MessageTable);

		/* Mark this Xtra "Safe for Shockwave" */
		MoaBool bItsSafe = TRUE;
		err = pRegDict->Put(kMoaMmDictType_SafeForShockwave,
			&bItsSafe, 	sizeof( bItsSafe ), kMoaMmDictKey_SafeForShockwave);

	}

	return(err);
}
