#define U8(str) reinterpret_cast<const char*>(u8##str)
#include "../../external/CppSDK/SDK.hpp"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "../Config/Configs.h"
#include "../Util/Util.h"
#include "../Hooks/Hooks.h"
#include "Menu.h"

namespace g_Menu {
    void DrawMenu(SDK::UCanvas* Canvas) {
        g_Util::UpdateMenuAlpha();
        g_Util::UpdateCursorVisibility();
        Shadow::NewFrame(Canvas);
        Shadow::UpdateAllHotkeyStates();
        Shadow::StyleColorsAmethyst();

        for (Shadow::Color& color : Shadow::GetStyle().Colors) {
            color.a *= g_Hooks::currentAlpha;
        }

        if (g_Hooks::currentAlpha <= 0.001f) return;

        static std::vector<std::string> targetOptions = { U8("敌人"), U8("队友") };
        static int selectedTarget = 0;

        if (Shadow::Begin(U8("黑梦##main_window"), Shadow::ShadowWindowFlags_::ShadowWindowFlags_TextAlignCenter)) {
            if (Shadow::BeginTabBar("MainTabs##tabs")) {
                // 设置
                if (Shadow::BeginTabItem(U8("设置##tab0"))) {
                    Shadow::HotKey(U8("菜单开关##menu_key"), &g_Config::kMenu);
                }
                Shadow::EndTabItem();

                // 功能
                if (Shadow::BeginTabItem(U8("功能##tab1"))) {
                    Shadow::HotKey(U8("魔法攻击##magic_attack__key"), &g_Config::kMagicAttack, &g_Config::bEnableMagicAttack, &g_Config::eMagicAttackMode);
                }
                Shadow::EndTabItem();

                // 视觉
                if (Shadow::BeginTabItem(U8("视觉##tab2"))) {
                    Shadow::Combo(U8("阵营##target_combo"), &selectedTarget, targetOptions);

                    // 敌人设置
                    if (selectedTarget == 0) {
                        Shadow::Switch(U8("启用敌人ESP##esp_enemy"), &g_Config::bESP_Enemy);
                        Shadow::Switch(U8("显示名字##enemy_name"), &g_Config::bESP_Enemy_Name);
                        Shadow::Switch(U8("显示血量##enemy_health"), &g_Config::bESP_Enemy_Health);
                    }
                    // 队友设置
                    else if (selectedTarget == 1) {
                        Shadow::Switch(U8("启用队友ESP##esp_teammate"), &g_Config::bESP_Teammate);
                        Shadow::Switch(U8("显示名字##teammate_name"), &g_Config::bESP_Teammate_Name);
                        Shadow::Switch(U8("显示血量##teammate_health"), &g_Config::bESP_Teammate_Health);
                    }
                }
                Shadow::EndTabItem();
            }
            Shadow::EndTabBar();
        }
        Shadow::End();
    }
}