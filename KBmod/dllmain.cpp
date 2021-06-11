#include <Windows.h>
#include <string>
#include "Logger.h"
#include <vector>

HMODULE hMod = 0;

int Loaded() {

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hMod = hModule;
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Loaded, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}