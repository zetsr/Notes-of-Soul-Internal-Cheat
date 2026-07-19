#pragma once
#include <windows.h>
#include "../../external/CppSDK/SDK.hpp"

namespace g_Config {
    inline bool bShowMenu = false;
    inline int kMenu = VK_F1;
    inline int kMagicAttack = VK_XBUTTON2;

    // 敌人
    inline bool bESP_Enemy = true;
    inline bool bESP_Enemy_Name = true;
    inline bool bESP_Enemy_Health = true;

    // 队友
    inline bool bESP_Teammate = true;
    inline bool bESP_Teammate_Name = true;
    inline bool bESP_Teammate_Health = true;
}