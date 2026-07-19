#define U8(str) reinterpret_cast<const char*>(u8##str)
#include "../../external/CppSDK/SDK.hpp"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "../Config/Configs.h"
#include "../Util/Util.h"
#include "../Hooks/Hooks.h"
#include "Menu.h"
#include "../../XorStr.h"

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

        static int selectedTarget = 0;

        // 直接通过 .str() 组装临时的 vector 数组，绕过任何非法的静态引用数组定义
        std::vector<std::string> tempOptions{
            _XOR_(U8("敌人")).str(),
            _XOR_(U8("队友")).str()
        };

        // 每一个 UI 调用严格使用 .str()
        if (Shadow::Begin(_XOR_(U8("黑梦##main_window")).str(), Shadow::ShadowWindowFlags_::ShadowWindowFlags_TextAlignCenter)) {
            if (Shadow::BeginTabBar(_XOR_(U8("MainTabs##tabs")).str())) {

                // 设置
                if (Shadow::BeginTabItem(_XOR_(U8("设置##tab0")).str())) {
                    Shadow::HotKey(_XOR_(U8("菜单开关##menu_key")).str(), &g_Config::kMenu);
                }
                Shadow::EndTabItem();

                // 功能
                if (Shadow::BeginTabItem(_XOR_(U8("功能##tab1")).str())) {
                    Shadow::HotKey(_XOR_(U8("魔法攻击##magic_attack__key")).str(), &g_Config::kMagicAttack, &g_Config::bEnableMagicAttack, &g_Config::eMagicAttackMode);
                    Shadow::Slider(_XOR_(U8("魔法攻击范围##magic_attack__range")).str(), &g_Config::fMagicAttackRange, 0.1f, 100.0f, 0.1f);
                }
                Shadow::EndTabItem();

                // 视觉
                if (Shadow::BeginTabItem(_XOR_(U8("视觉##tab2")).str())) {
                    Shadow::Combo(_XOR_(U8("阵营##target_combo")).str(), &selectedTarget, tempOptions);

                    // 敌人设置
                    if (selectedTarget == 0) {
                        Shadow::Switch(_XOR_(U8("启用敌人ESP##esp_enemy")).str(), &g_Config::bESP_Enemy);
                        Shadow::Switch(_XOR_(U8("显示名字##enemy_name")).str(), &g_Config::bESP_Enemy_Name);
                        Shadow::Switch(_XOR_(U8("显示血量##enemy_health")).str(), &g_Config::bESP_Enemy_Health);
                    }
                    // 队友设置
                    else if (selectedTarget == 1) {
                        Shadow::Switch(_XOR_(U8("启用队友ESP##esp_teammate")).str(), &g_Config::bESP_Teammate);
                        Shadow::Switch(_XOR_(U8("显示名字##teammate_name")).str(), &g_Config::bESP_Teammate_Name);
                        Shadow::Switch(_XOR_(U8("显示血量##teammate_health")).str(), &g_Config::bESP_Teammate_Health);
                    }
                }
                Shadow::EndTabItem();
            }
            Shadow::EndTabBar();
        }
        Shadow::End();
    }
}