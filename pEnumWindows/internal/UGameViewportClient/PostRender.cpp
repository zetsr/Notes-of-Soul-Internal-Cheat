#pragma once
#include "../../external/CppSDK/SDK.hpp"
#include "../ESP/ESP.h"
#include "../Config/Configs.h"
#include "../ESP/DrawESP.h"
#include "../Hack/Hack.h"
#include "PostRender.h"
#include "../../external/Shadow-Gui/include/Shadow.h"

namespace g_UGameViewportClient {
    void PostRender(SDK::UGameViewportClient* _this, SDK::UCanvas* Canvas) {
        Shadow::NewFrame(Canvas);
        Shadow::UpdateAllHotkeyStates();
        Shadow::StyleColorsAmethyst();

        if (Shadow::Begin("Main##main_window")) {

        }
        Shadow::End();

        g_DrawESP::DrawESP(Canvas);
    }
}