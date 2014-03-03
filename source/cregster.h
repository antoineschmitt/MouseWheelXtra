#ifndef _H_cregster
#define _H_cregster

#include "moaxtra.h"
#include "cscript.h"

// {25962D42-F339-11d1-9880-00A0242DCC6D}
DEFINE_GUID(CLSID(CRegister), 
0x25962d42, 0xf339, 0x11d1, 0x98, 0x80, 0x0, 0xa0, 0x24, 0x2d, 0xcc, 0x6d);

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CRegister) 
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CRegister, IMoaRegister) 
	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaDict))
EXTERN_END_DEFINE_CLASS_INTERFACE


#endif /* _H_cregster */

