#include <windows.h>
#include "../../external/Minhook/include/MinHook.h"
#include "../../external/CppSDK/SDK.hpp"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "internal/ESP/ESP.h"
#include "internal/Config/Configs.h"
#include "internal/ESP/DrawESP.h"
#include "internal/Hooks/Hooks.h"

void MainThread(LPVOID lpParam) {
    MH_STATUS mhStatus = MH_Initialize();

    if (mhStatus != MH_OK && mhStatus != MH_ERROR_ALREADY_INITIALIZED) {
        return;
    }

	g_Hooks::FindWndProc();
    g_Hooks::FindPostRender();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        if (HANDLE h = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr)) CloseHandle(h);
        break;

    case DLL_PROCESS_DETACH:
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    }
    return TRUE;
}