// Patch.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Patch.h"
#include "SomeClasses.h"
#include "ClassCheck.h"

Foo foo;

struct UsrData
{
	ClassChecker check;
	int counter;
};

PATCH_API void* init()
{
	UsrData* data = new UsrData;
	data->counter = 0;
	return data;
}

//data is the value returned by init
//return a value of loop_status to say what to do
PATCH_API int loop(void* dataptr, int reloaded)
{
	UsrData& data = *reinterpret_cast<UsrData*>(dataptr);
	if (reloaded && !data.check.CheckClassSizes())
	{
		delete dataptr;
		return PATCH_RESTART;
	}

	std::cout << data.counter << ' ' << reloaded << '\n';
	++data.counter;
	std::getchar();
	if (data.counter > 5)
	{
		delete dataptr;
		return PATCH_EXIT;
	}
	else
		return PATCH_CONTINUE;
}

/*
// This is an example of an exported variable
PATCH_API int nPatch=0;

// This is an example of an exported function.
PATCH_API int fnPatch(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Patch.h for the class definition
CPatch::CPatch()
{
	return;
}*/
