#include "../../external/CppSDK/SDK.hpp"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "../Config/Configs.h"
#include "../Util/Util.h"
#include "Menu.h"

namespace g_Menu {
    void DrawMenu(SDK::UCanvas* Canvas) {
        g_Util::UpdateCursorVisibility();
        Shadow::NewFrame(Canvas);
        Shadow::UpdateAllHotkeyStates();
        Shadow::StyleColorsAmethyst();

        if (!g_Config::bShowMenu) return;

        if (Shadow::Begin("Main##main_window")) {
            Shadow::HotKey("Toggle Menu##menu_key", &g_Config::kMenu);
        }
        Shadow::End();
    }
}