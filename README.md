# vcruntime140 proxy

DLL hijacking with vcruntime140

## About

This code allows DLL hijacking in applications by placing the `vcruntime140_1.dll` library in the application folder, without modifying the executable files of the application.

## How it works

Many modern applications built with platform building toolset version 140 (and higher) with run-time llibrary in [multithread-DLL](https://docs.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-160) (/MD) mode put the `vcruntime140_1.dll` library in the import table or call it indirectly.

The original `vcruntime140_1.dll` library contains only a few [exception handling functions](https://devblogs.microsoft.com/cppblog/making-cpp-exception-handling-smaller-x64/) (like CxxFrameHandler4).

Proxy loads itself, then loads the original `vcruntime140_1.dll` library if the corresponding [Visual C++ Redistributable](https://support.microsoft.com/en-us/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0) is installed, if the runtimes in the application are local (portable) , then it is enough to rename the original library to `vcruntime140_2.dll`.

## Features

- Small size
- Easy to use
- Support for inject in to many modern applications, without modifying the application files.

## Sample

```c++
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
```

