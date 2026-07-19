#include "../../external/CppSDK/SDK.hpp"
#include "Hooks.h"
#include "../CheatData/CheatData.h"
#include "../../external/MinHook/include/MinHook.h"
#include "../../external/AOBScan/AOBScan.hpp"
#include "../UGameViewportClient/PostRender.h"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include "../Util/Util.h"
#include "../Config/Configs.h"

namespace g_Hooks {
    HWND g_hwnd = NULL;
    WNDPROC oWndProc = NULL;

    inline BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == GetCurrentProcessId()) {
            if (IsWindowVisible(hwnd) && GetWindow(hwnd, GW_OWNER) == NULL) {
                LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
                if ((style & WS_CAPTION) || (style & WS_POPUP)) {
                    g_hwnd = hwnd;
                    return FALSE;
                }
            }
        }
        return TRUE;
    }

    LRESULT APIENTRY WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        // 始终处理全局热键（菜单打开或关闭都需要）
        Shadow::ProcessGlobalHotkeys(hwnd, uMsg, wParam, lParam);

        // 菜单切换键处理（如果正在分配热键，则不切换菜单）
        if (uMsg == WM_KEYDOWN && wParam == g_Config::kMenu && !Shadow::g_Ctx.AssigningHotkey) {
            bool isFirstPress = ((lParam & (1 << 30)) == 0);

            if (isFirstPress) {
                g_Config::bShowMenu = !g_Config::bShowMenu;
            }
        }

        if (g_Config::bShowMenu || currentAlpha > 0.001f) {
            // 键盘消息处理
            if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP || uMsg == WM_CHAR) {
                // 如果是切换键本身，放行给游戏（防止菜单无法关闭）
                if (wParam == g_Config::kMenu) {
                    return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
                }

                // 如果是已注册的全局热键，放行给游戏（热键状态已在 ProcessGlobalHotkeys 中更新）
                if (Shadow::IsHotkeyRegistered(static_cast<int>(wParam))) {
                    return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
                }

                // 如果不在允许列表中，阻塞该按键
                if (Shadow::IsKeyAllowed(static_cast<int>(wParam))) {
                    return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
                }

                // 其余菜单所需的键盘输入（如录入热键、打字），交由框架处理并阻塞游戏输入
                Shadow::Input(hwnd, uMsg, wParam, lParam);
                return 1;
            }

            // 如果是鼠标消息，且不在白名单里，直接阻塞
            if (!Shadow::IsMouseMsgAllowed(uMsg)) {
                // 处理输入（鼠标坐标、点击等，菜单打开时需要）
                Shadow::Input(hwnd, uMsg, wParam, lParam);
                return 1;
            }
        }

        // 处理输入（鼠标坐标、点击等，菜单打开时需要）
        Shadow::Input(hwnd, uMsg, wParam, lParam);

        return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
    }


	typedef void(__fastcall* tPostRender)(SDK::UGameViewportClient* _this, SDK::UCanvas* Canvas);
	tPostRender oPostRender = nullptr;

	void __fastcall hkPostRender(SDK::UGameViewportClient* _this, SDK::UCanvas* Canvas) {
		oPostRender(_this, Canvas);
		g_UGameViewportClient::PostRender(_this, Canvas);
	}

	void FindPostRender() {
		std::string pattern = g_CheatData::Signature::PostRender;
		AOB::Result ok = AOB::Scan(pattern);

		if (ok && ok.size() == 1) {
			void* targetAddr = ok[0];
			if (MH_CreateHook(targetAddr, &hkPostRender, reinterpret_cast<LPVOID*>(&oPostRender)) == MH_OK) {
				MH_EnableHook(targetAddr);
			}
		}
	}

    void FindWndProc() {
        EnumWindows(EnumWindowsProc, 0);
        if (g_hwnd) {
            oWndProc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProcHook);
            Shadow::SetAllowedKeys({ 'W', 'A', 'S', 'D', VK_SPACE }); // 放行常用移动按键
        }
    }
}