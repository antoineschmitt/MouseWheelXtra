
#include <MacTypes.r>
#include "dversion.h"
#include "xclassver.h"

resource 'vers' (1)
{
	XTRA_CLASS_MAJOR_VERSION,
	XTRA_CLASS_MINOR_VERSION,
	finalStage, //DEV STAGE (alpha = 0x40, beta = 0x60,final = 0x80, release = 0x80;)
	XTRA_CLASS_RELEASE_TYPE, //NON-FINAL BUILD NUMBER
	verFrance, //REGION CODE
	VER_VERSION_STRING,
	VER_PRODUCTNAME
};

resource 'vers' (2)
{
	XTRA_CLASS_MAJOR_VERSION,
	XTRA_CLASS_MINOR_VERSION,
	finalStage,  //DEV STAGE (alpha = 0x40, beta = 0x60,final = 0x80, release = 0x80;)
	XTRA_CLASS_RELEASE_TYPE, //NON-FINAL BUILD NUMBER
	verFrance, //REGION CODE
	VER_VERSION_STRING,
	VER_VERSION_STRING //--------VISIBLE
};
