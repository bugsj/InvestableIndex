// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

void XLLEntry(HMODULE hModule, DWORD ul_reason_for_call);
void DLLCEntry(HMODULE hModule, DWORD ul_reason_for_call);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

	DLLCEntry(hModule, ul_reason_for_call);
	XLLEntry(hModule, ul_reason_for_call);

    return TRUE;
}

