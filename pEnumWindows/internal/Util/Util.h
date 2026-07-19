#pragma once
#include "../../external/CppSDK/SDK.hpp"
#include "../Config/Configs.h"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "../Hooks/Hooks.h"
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

    inline bool GetMouseCursorVisible() {
        if (auto world = SDK::UWorld::GetWorld()) {
            if (world->OwningGameInstance && world->OwningGameInstance->LocalPlayers.IsValidIndex(0)) {
                if (auto pc = world->OwningGameInstance->LocalPlayers[0]->PlayerController) {
                    return pc->bShowMouseCursor;
                }
            }
        }
        return false;
    }

    inline void SetMouseCursorVisible(bool visible) {
        if (auto world = SDK::UWorld::GetWorld()) {
            if (world->OwningGameInstance && world->OwningGameInstance->LocalPlayers.IsValidIndex(0)) {
                if (auto pc = world->OwningGameInstance->LocalPlayers[0]->PlayerController) {
                    pc->bShowMouseCursor = visible;
                }
            }
        }
    }

    inline void UpdateCursorVisibility() {
        static bool bWasAnimating = false;
        static bool gameOriginalCursorState = false;

        bool isAnimating = (g_Config::bShowMenu);

        if (isAnimating) {
            if (!bWasAnimating) {
                gameOriginalCursorState = GetMouseCursorVisible();
            }

            SetMouseCursorVisible(true);
            bWasAnimating = true;
        }
        else if (bWasAnimating) {
            SetMouseCursorVisible(gameOriginalCursorState);
            bWasAnimating = false;
        }
	}
}