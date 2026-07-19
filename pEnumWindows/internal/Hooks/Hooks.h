#pragma once

namespace g_Hooks {
	void __fastcall hkPostRender(SDK::UGameViewportClient* _this, SDK::UCanvas* Canvas);

	void FindPostRender();
}