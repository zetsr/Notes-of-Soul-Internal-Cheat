#pragma once

namespace g_Hooks {
	inline float currentAlpha = 0.0f;
	inline const float fadeSpeed = 5.0f;

	void FindWndProc();

	void __fastcall hkPostRender(SDK::UGameViewportClient* _this, SDK::UCanvas* Canvas);
	void FindPostRender();
}