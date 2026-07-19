#pragma once
#include "../../external/CppSDK/SDK.hpp"
#include "../Config/Configs.h"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "../Hooks/Hooks.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>

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

    inline void UpdateMenuAlpha() {
        static auto lastTime = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        float dt = deltaTime.count();

        if (g_Config::bShowMenu) {
            g_Hooks::currentAlpha += g_Hooks::fadeSpeed * dt;
            if (g_Hooks::currentAlpha > 1.0f) g_Hooks::currentAlpha = 1.0f;
        }
        else {
            g_Hooks::currentAlpha -= g_Hooks::fadeSpeed * dt;
            if (g_Hooks::currentAlpha < 0.0f) g_Hooks::currentAlpha = 0.0f;
        }
    }

    // 血量颜色渐变：满血绿 (0,1,0) -> 空血红 (1,0,0)
    inline static SDK::FLinearColor GetHealthColor(float current, float max) {
        if (max <= 0.0f) return { 0.0f, 1.0f, 0.0f, 1.0f };
        float ratio = std::clamp(current / max, 0.0f, 1.0f);
        return { 1.0f - ratio, ratio, 0.0f, 1.0f };
    }

}