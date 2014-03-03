#include "cscript.h"
#include "cregster.h"
#include "dversion.h"
#include "xclassver.h"

BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(CRegister, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(CRegister, IMoaRegister, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
	BEGIN_XTRA_DEFINES_CLASS(CScript, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(CScript, IMoaMmXScript, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
END_XTRA