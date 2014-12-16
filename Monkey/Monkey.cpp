// Monkey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../Patch/Patch.h"

struct LibraryRAII
{
	HMODULE h;
	LibraryRAII(HMODULE h_) : h(h_) {}
	LibraryRAII(const LibraryRAII&) = delete;
	~LibraryRAII() { if (h) FreeLibrary(h); }
	bool CheckedClose()
	{
		bool result = FreeLibrary(h) != 0;
		h = nullptr;
		return result;
	}
};

struct HandleRAII
{
	HANDLE h;
	HandleRAII(HANDLE h_) : h(h_) {}
	HandleRAII(const HandleRAII&) = delete;
	~HandleRAII() { if (h) CloseHandle(h); }
	bool CheckedClose()
	{
		bool result = CloseHandle(h) != 0;
		h = nullptr;
		return result;
	}
};

void ThrowErrno(const std::string& text)
{
	DWORD errcode = GetLastError();
	if (errcode != 0)
	{
		LPTSTR lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		std::string message = lpMsgBuf;
		LocalFree(lpMsgBuf);
		throw std::runtime_error(text + ": " + message);
	}
}

init_ty init;
loop_ty loop;

const char* dll_file = "Debug\\Patch.dll";
const char* dll_file_inuse = "Debug\\Patch_inuse.dll";

bool FileExists(const char* name)
{
	WIN32_FIND_DATA findFileData;
	return FindFirstFile(name, &findFileData) != INVALID_HANDLE_VALUE;
}

std::unique_ptr<LibraryRAII> ReloadPatch()
{
	std::cerr << "Reloading...\n";

	CopyFile(dll_file, dll_file_inuse, false);
	ThrowErrno("CopyFile");

	std::unique_ptr<LibraryRAII> hdl = 
		std::make_unique<LibraryRAII>(LoadLibrary(dll_file_inuse));
	ThrowErrno("LoadLibrary");
	init = reinterpret_cast<init_ty>(GetProcAddress(hdl->h, "init"));
	ThrowErrno("GetProcAddress(init)");
	loop = reinterpret_cast<loop_ty>(GetProcAddress(hdl->h, "loop"));
	ThrowErrno("GetProcAddress(loop)");
	return hdl;
}

void DllModTime(PFILETIME lpFileTime)
{
	HandleRAII file = CreateFile(dll_file, GENERIC_READ,
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr, OPEN_EXISTING, 0, nullptr);
	ThrowErrno("CreateFile");
	GetFileTime(file.h, nullptr, nullptr, lpFileTime);
	ThrowErrno("GetFileTime");
}

int main(int, char*[])
try
{
	if (!FileExists(dll_file))
		throw std::runtime_error("dll not built");

	std::unique_ptr<LibraryRAII> hdl = ReloadPatch();

	FILETIME mod_time;
	DllModTime(&mod_time);
	FILETIME next_mod_time;

	int status;
	do {
		std::cerr << "Starting...\n";
		void* data = init();
		if (data == nullptr)
			return 1;
		do {
			DllModTime(&next_mod_time);
			if (next_mod_time.dwLowDateTime != mod_time.dwLowDateTime)
			{
				mod_time = next_mod_time;
				hdl.reset(); //we'll be copying over the loaded dll
				hdl = ReloadPatch();
				status = loop(data, 1);
			}
			else
				status = loop(data, 0);
		} while (status == PATCH_CONTINUE);
	} while (status == PATCH_RESTART);

	return 0;
}
catch (std::runtime_error& err)
{
	std::cerr << err.what();
}
