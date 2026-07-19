#pragma once
#include "../../external/CppSDK/SDK.hpp"
#include "../Config/Configs.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>

namespace g_Util {
    inline SDK::APlayerController* GetLocalPC() {
        SDK::UWorld* World = SDK::UWorld::GetWorld();

        if (!World) {
            return nullptr;
        }

        if (!World->OwningGameInstance) {
            return nullptr;
        }

        if (World->OwningGameInstance->LocalPlayers.Num() == 0) {
            return nullptr;
        }

        SDK::ULocalPlayer* LocalPlayer = World->OwningGameInstance->LocalPlayers[0];
        if (!LocalPlayer) {
            return nullptr;
        }

        SDK::APlayerController* PC = LocalPlayer->PlayerController;
        if (!PC) {
            return nullptr;
        }

        return PC;
    }
}