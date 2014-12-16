// Patch.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Patch.h"

PATCH_API void* init()
{
	return new int(0);
}

//data is the value returned by init
//return a value of loop_status to say what to do
PATCH_API int loop(void* dataptr)
{
	int& data = *reinterpret_cast<int*>(dataptr);
	std::cout << data << '\n';
	++data;
	std::getchar();
	if (data > 5)
	{
		delete dataptr;
		return PATCH_EXIT;
	}
	else
		return PATCH_CONTINUE;
}

struct Foo
{
	Foo()
	{
		std::cout << "static ctor!\n";
	}
};

Foo foo;

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
