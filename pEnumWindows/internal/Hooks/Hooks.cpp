#include "../../external/CppSDK/SDK.hpp"
#include "Hooks.h"
#include "../CheatData/CheatData.h"
#include "../../external/MinHook/include/MinHook.h"
#include "../../external/AOBScan/AOBScan.hpp"
#include "../UGameViewportClient/PostRender.h"

namespace g_Hooks {
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
}