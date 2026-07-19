#pragma once
#include <windows.h>
#include "../../external/CppSDK/SDK.hpp"
#include "../../external/Shadow-Gui/include/Shadow.h"

namespace g_Config {
    inline bool bShowMenu = false;
    inline int kMenu = VK_F1;

    inline bool bEnableMagicAttack = true;
    inline int kMagicAttack = VK_XBUTTON2;
    inline Shadow::HotkeyMode eMagicAttackMode = Shadow::HotkeyMode::HoldOn;
    inline int nMagicAttackMode = 0;
    inline float fMagicAttackRange = 5.0f;
    inline float fMagicAttackFov = 30.0f;

    inline bool bEnableTeleport = false;
    inline int kTeleport = VK_XBUTTON1;
    inline Shadow::HotkeyMode eTeleportMode = Shadow::HotkeyMode::HoldOn;
    inline int nTeleportTargetMode = 0;
    inline float fTeleportRange = 5.0f;
    inline float fTeleportFov = 30.0f;

    inline bool bESP_Enemy = true;
    inline bool bESP_Enemy_Name = true;
    inline float enemyNameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool bESP_Enemy_Distance = true;
    inline float enemyDistanceColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool bESP_Enemy_Health = true;
    inline bool bESP_Enemy_Invincible = true;
    inline float enemyInvincibleColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    inline bool bESP_Enemy_ProtectSoul = true;
    inline float enemyProtectSoulColor[4] = { 1.0f, 0.5f, 0.0f, 1.0f };
    inline bool bESP_Enemy_PlayDead = true;
    inline float enemyPlayDeadColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    inline bool bESP_Enemy_Controlled = true;
    inline float enemyControlledColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline bool bESP_Enemy_HoldingItem = true;
    inline float enemyHoldingItemColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

    inline bool bESP_Teammate = true;
    inline bool bESP_Teammate_Name = true;
    inline float teammateNameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool bESP_Teammate_Distance = true;
    inline float teammateDistanceColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool bESP_Teammate_Health = true;
    inline bool bESP_Teammate_Invincible = true;
    inline float teammateInvincibleColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    inline bool bESP_Teammate_ProtectSoul = true;
    inline float teammateProtectSoulColor[4] = { 1.0f, 0.5f, 0.0f, 1.0f };
    inline bool bESP_Teammate_PlayDead = true;
    inline float teammatePlayDeadColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    inline bool bESP_Teammate_Controlled = true;
    inline float teammateControlledColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline bool bESP_Teammate_HoldingItem = true;
    inline float teammateHoldingItemColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
}