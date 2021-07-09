/*
	Runtime proxy loader
*/

#include "vcruntime.h"

#include <Windows.h>
#include <tchar.h>

typedef size_t (RUNTIME_CALL *_CxxFrameHandler4_t)(void*, size_t, void*, void*);

static HMODULE g_hRuntime = nullptr;
static _CxxFrameHandler4_t g_CxxFrameHandler4 = nullptr;

#pragma comment(linker,"/export:__CxxFrameHandler4=_CxxFrameHandler4")
RUNTIME_EXPORT size_t RUNTIME_CALL _CxxFrameHandler4(void* pExcept, size_t RN, void* pContext, void*pDC)
{
	if (g_CxxFrameHandler4) return g_CxxFrameHandler4(pExcept, RN, pContext, pDC);
	return 1;
}

#pragma comment(linker,"/export:__NLG_Dispatch2=_NLG_Dispatch2")
RUNTIME_EXPORT void RUNTIME_CALL _NLG_Dispatch2()
{
	return;
}

#pragma comment(linker,"/export:__NLG_Return2=_NLG_Return2")
RUNTIME_EXPORT void RUNTIME_CALL _NLG_Return2()
{
	return;
}

namespace proxy
{
	inline void crash()
	{
		uint8_t* memory = nullptr;
		*memory = 0;
	}

	HMODULE getLocalVCRuntime()
	{
		return LoadLibraryExW(_T(VC_RUNTIME_LOCAL_LIB), NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
	}

	HMODULE getSystemVCRuntime()
	{
		size_t len = 0;
		static wchar_t runtimePath[MAX_PATH];

		if (GetSystemDirectoryW(runtimePath, MAX_PATH - 1))
		{
			const wchar_t* runtime = _T(VC_RUNTIME_LIB);
			wchar_t* endPath = runtimePath;

			while (*endPath++) ++len;

			if (len + ARRAYSIZE(VC_RUNTIME_LIB) > MAX_PATH)
				return FALSE;

			*(--endPath)++ = '\\';
			while (*endPath++ = *runtime++);

			return LoadLibraryExW(runtimePath, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
		}
		return nullptr;
	}

	void init_runtime()
	{
		if (!g_hRuntime)
		{
			g_hRuntime = getLocalVCRuntime();
			if (!g_hRuntime) g_hRuntime = getSystemVCRuntime();

			if (g_hRuntime)
			{
				g_CxxFrameHandler4 = (_CxxFrameHandler4_t)GetProcAddress(g_hRuntime, "__CxxFrameHandler4");
			}
		}
		// handler required!
		if (!g_CxxFrameHandler4) crash();
	}
	void free_runtime()
	{
		if (g_hRuntime)
		{
			FreeLibrary(g_hRuntime);
			g_CxxFrameHandler4 = nullptr;
			g_hRuntime = nullptr;
		}
	}
}