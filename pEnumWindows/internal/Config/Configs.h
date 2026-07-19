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

    // 敌人
    inline bool bESP_Enemy = true;
    inline bool bESP_Enemy_Name = true;
    inline bool bESP_Enemy_Health = true;

    // 队友
    inline bool bESP_Teammate = true;
    inline bool bESP_Teammate_Name = true;
    inline bool bESP_Teammate_Health = true;
}