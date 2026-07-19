#pragma once
#include "../../external/CppSDK/SDK.hpp"
#include "../ESP/ESP.h"
#include "../Config/Configs.h"
#include "../ESP/DrawESP.h"
#include "../Hack/Hack.h"
#include "../Menu/Menu.h"
#include "PostRender.h"

namespace g_UGameViewportClient {
    void PostRender(SDK::UGameViewportClient* _this, SDK::UCanvas* Canvas) {
        g_DrawESP::DrawESP(Canvas);
		g_Menu::DrawMenu(Canvas);
    }
}