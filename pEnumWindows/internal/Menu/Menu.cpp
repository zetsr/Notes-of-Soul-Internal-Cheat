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
        Shadow::StyleColorsOcean();

        for (Shadow::Color& color : Shadow::GetStyle().Colors) {
            color.a *= g_Hooks::currentAlpha;
        }

        if (g_Hooks::currentAlpha <= 0.001f) return;

        static int selectedTarget = 0;
        static int selectedMagicAttackMode = 0;
        static int selectedTeleportMode = 0;

        std::vector<std::string> tempOptions{
            _XOR_(U8("敌人")).str(),
            _XOR_(U8("队友")).str()
        };

        std::vector<std::string> magicAttackModeOptions{
            _XOR_(U8("跟随距离")).str(),
            _XOR_(U8("跟随准星")).str()
        };

        std::vector<std::string> teleportModeOptions{
            _XOR_(U8("跟随距离")).str(),
            _XOR_(U8("跟随准星")).str()
        };

        if (Shadow::Begin(_XOR_(U8("黑梦##main_window")).str(), Shadow::ShadowWindowFlags_::ShadowWindowFlags_TextAlignCenter | Shadow::ShadowWindowFlags_::ShadowWindowFlags_NoScrollbar)) {
            if (Shadow::BeginTabBar(_XOR_(U8("MainTabs##tabs")).str())) {

                if (Shadow::BeginTabItem(_XOR_(U8("设置##tab0")).str())) {
                    Shadow::HotKey(_XOR_(U8("菜单开关##menu_key")).str(), &g_Config::kMenu, Shadow::ShadowHotkeyFlags_NoRightAlign);
                }
                Shadow::EndTabItem();

                if (Shadow::BeginTabItem(_XOR_(U8("功能##tab1")).str())) {
                    Shadow::HotKey(_XOR_(U8("魔法攻击##magic_attack__key")).str(), &g_Config::kMagicAttack, &g_Config::bEnableMagicAttack, &g_Config::eMagicAttackMode, Shadow::ShadowHotkeyFlags_NoRightAlign | Shadow::ShadowHotkeyFlags_NoStateDisplay);
                    Shadow::SameLine();
                    Shadow::Combo(_XOR_(U8("魔法攻击模式##magic_attack__mode")).str(), &selectedMagicAttackMode, magicAttackModeOptions, Shadow::ShadowComboFlags_NoText | Shadow::ShadowComboFlags_NoRightAlign | Shadow::ShadowComboFlags_FitText);
                    g_Config::nMagicAttackMode = selectedMagicAttackMode;
                    if (g_Config::nMagicAttackMode == 0) {
                        Shadow::Slider(_XOR_(U8("魔法攻击范围##magic_attack__range")).str(), &g_Config::fMagicAttackRange, 0.1f, 100.0f, 0.1f, Shadow::ShadowSliderFlags_NoRightAlign);
                    }
                    else if (g_Config::nMagicAttackMode == 1) {
                        Shadow::Slider(_XOR_(U8("魔法攻击Fov##magic_attack__fov")).str(), &g_Config::fMagicAttackFov, 0.1f, 180.0f, 0.1f, Shadow::ShadowSliderFlags_NoRightAlign);
                    }

                    Shadow::Separator();

                    Shadow::HotKey(_XOR_(U8("一键传送##teleport__key")).str(), &g_Config::kTeleport, &g_Config::bEnableTeleport, &g_Config::eTeleportMode, Shadow::ShadowHotkeyFlags_NoRightAlign | Shadow::ShadowHotkeyFlags_NoStateDisplay);
                    Shadow::SameLine();
                    Shadow::Combo(_XOR_(U8("传送模式##teleport__mode")).str(), &selectedTeleportMode, teleportModeOptions, Shadow::ShadowComboFlags_NoText | Shadow::ShadowComboFlags_NoRightAlign | Shadow::ShadowComboFlags_FitText);
                    g_Config::nTeleportTargetMode = selectedTeleportMode;
                    if (g_Config::nTeleportTargetMode == 0) {
                        Shadow::Slider(_XOR_(U8("传送范围##teleport__range")).str(), &g_Config::fTeleportRange, 0.1f, 100.0f, 0.1f, Shadow::ShadowSliderFlags_NoRightAlign);
                    }
                    else if (g_Config::nTeleportTargetMode == 1) {
                        Shadow::Slider(_XOR_(U8("传送Fov##teleport__fov")).str(), &g_Config::fTeleportFov, 0.1f, 180.0f, 0.1f, Shadow::ShadowSliderFlags_NoRightAlign);
                    }
                }
                Shadow::EndTabItem();

                if (Shadow::BeginTabItem(_XOR_(U8("视觉##tab2")).str())) {
                    Shadow::Combo(_XOR_(U8("阵营##target_combo")).str(), &selectedTarget, tempOptions, Shadow::ShadowComboFlags_NoRightAlign | Shadow::ShadowComboFlags_FitText);

                    if (selectedTarget == 0) {
                        Shadow::Switch(_XOR_(U8("开启透视##esp_enemy")).str(), &g_Config::bESP_Enemy);

                        Shadow::Switch(_XOR_(U8("显示名字##enemy_name")).str(), &g_Config::bESP_Enemy_Name);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("名字颜色##enemy_name_color")).str(), &g_Config::enemyNameColor[0], &g_Config::enemyNameColor[1], &g_Config::enemyNameColor[2], &g_Config::enemyNameColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示距离##enemy_distance")).str(), &g_Config::bESP_Enemy_Distance);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("距离颜色##enemy_distance_color")).str(), &g_Config::enemyDistanceColor[0], &g_Config::enemyDistanceColor[1], &g_Config::enemyDistanceColor[2], &g_Config::enemyDistanceColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示血量##enemy_health")).str(), &g_Config::bESP_Enemy_Health);

                        Shadow::Switch(_XOR_(U8("显示无敌##enemy_invincible")).str(), &g_Config::bESP_Enemy_Invincible);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("无敌颜色##enemy_invincible_color")).str(), &g_Config::enemyInvincibleColor[0], &g_Config::enemyInvincibleColor[1], &g_Config::enemyInvincibleColor[2], &g_Config::enemyInvincibleColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示护魄##enemy_protect_soul")).str(), &g_Config::bESP_Enemy_ProtectSoul);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("护魄颜色##enemy_protect_soul_color")).str(), &g_Config::enemyProtectSoulColor[0], &g_Config::enemyProtectSoulColor[1], &g_Config::enemyProtectSoulColor[2], &g_Config::enemyProtectSoulColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示装死##enemy_play_dead")).str(), &g_Config::bESP_Enemy_PlayDead);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("装死颜色##enemy_play_dead_color")).str(), &g_Config::enemyPlayDeadColor[0], &g_Config::enemyPlayDeadColor[1], &g_Config::enemyPlayDeadColor[2], &g_Config::enemyPlayDeadColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示被控##enemy_controlled")).str(), &g_Config::bESP_Enemy_Controlled);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("被控颜色##enemy_controlled_color")).str(), &g_Config::enemyControlledColor[0], &g_Config::enemyControlledColor[1], &g_Config::enemyControlledColor[2], &g_Config::enemyControlledColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示持物##enemy_holding_item")).str(), &g_Config::bESP_Enemy_HoldingItem);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("持物颜色##enemy_holding_item_color")).str(), &g_Config::enemyHoldingItemColor[0], &g_Config::enemyHoldingItemColor[1], &g_Config::enemyHoldingItemColor[2], &g_Config::enemyHoldingItemColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                    }
                    else if (selectedTarget == 1) {
                        Shadow::Switch(_XOR_(U8("开启透视##esp_teammate")).str(), &g_Config::bESP_Teammate);

                        Shadow::Switch(_XOR_(U8("显示名字##teammate_name")).str(), &g_Config::bESP_Teammate_Name);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("名字颜色##teammate_name_color")).str(), &g_Config::teammateNameColor[0], &g_Config::teammateNameColor[1], &g_Config::teammateNameColor[2], &g_Config::teammateNameColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示距离##teammate_distance")).str(), &g_Config::bESP_Teammate_Distance);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("距离颜色##teammate_distance_color")).str(), &g_Config::teammateDistanceColor[0], &g_Config::teammateDistanceColor[1], &g_Config::teammateDistanceColor[2], &g_Config::teammateDistanceColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示血量##teammate_health")).str(), &g_Config::bESP_Teammate_Health);

                        Shadow::Switch(_XOR_(U8("显示无敌##teammate_invincible")).str(), &g_Config::bESP_Teammate_Invincible);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("无敌颜色##teammate_invincible_color")).str(), &g_Config::teammateInvincibleColor[0], &g_Config::teammateInvincibleColor[1], &g_Config::teammateInvincibleColor[2], &g_Config::teammateInvincibleColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示护魄##teammate_protect_soul")).str(), &g_Config::bESP_Teammate_ProtectSoul);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("护魄颜色##teammate_protect_soul_color")).str(), &g_Config::teammateProtectSoulColor[0], &g_Config::teammateProtectSoulColor[1], &g_Config::teammateProtectSoulColor[2], &g_Config::teammateProtectSoulColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示装死##teammate_play_dead")).str(), &g_Config::bESP_Teammate_PlayDead);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("装死颜色##teammate_play_dead_color")).str(), &g_Config::teammatePlayDeadColor[0], &g_Config::teammatePlayDeadColor[1], &g_Config::teammatePlayDeadColor[2], &g_Config::teammatePlayDeadColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示被控##teammate_controlled")).str(), &g_Config::bESP_Teammate_Controlled);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("被控颜色##teammate_controlled_color")).str(), &g_Config::teammateControlledColor[0], &g_Config::teammateControlledColor[1], &g_Config::teammateControlledColor[2], &g_Config::teammateControlledColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                        Shadow::Switch(_XOR_(U8("显示持物##teammate_holding_item")).str(), &g_Config::bESP_Teammate_HoldingItem);
                        Shadow::SameLine();
                        Shadow::ColorPicker(_XOR_(U8("持物颜色##teammate_holding_item_color")).str(), &g_Config::teammateHoldingItemColor[0], &g_Config::teammateHoldingItemColor[1], &g_Config::teammateHoldingItemColor[2], &g_Config::teammateHoldingItemColor[3], Shadow::ShadowColorPickerFlags_NoRightAlign | Shadow::ShadowColorPickerFlags_NoText);

                    }
                }
                Shadow::EndTabItem();
            }
            Shadow::EndTabBar();
        }
        Shadow::End();
    }
}