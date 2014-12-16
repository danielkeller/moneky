// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PATCH_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PATCH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PATCH_EXPORTS
#define PATCH_API extern "C" __declspec(dllexport)
#else
#define PATCH_API extern "C"  __declspec(dllimport)
#endif

//initialize the application, returning nullptr on failure
typedef void* (*init_ty)();

enum loop_status
{
	PATCH_CONTINUE = 0,
	PATCH_EXIT = 1,
	PATCH_RESTART = 2
};

//data is the value returned by init
//return a value of loop_status to say what to do
typedef int (*loop_ty)(void*);

/*
// This class is exported from the Patch.dll
class PATCH_API CPatch {
public:
	CPatch(void);
	// TODO: add your methods here.
};

extern PATCH_API int nPatch;

PATCH_API int fnPatch(void);
*/