/*
	DLL hijacking with vcruntime140
*/

#include <Windows.h>
#include <tchar.h>

// include proxy
#include "vcruntime.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		proxy::init_runtime();

		MessageBox(NULL, _T("DLL Injected!"), _T("Hello!"), MB_ICONINFORMATION);
	}
	if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		proxy::free_runtime();
	}
	return TRUE;
}