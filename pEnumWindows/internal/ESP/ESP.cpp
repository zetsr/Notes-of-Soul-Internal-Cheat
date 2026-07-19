// ESP.cpp
#include "../../external/CppSDK/SDK.hpp"
#include "ESP.h"
#include "../Config/Configs.h"
#include "../Util/Util.h"
#include "../../external/Shadow-Gui/include/Shadow.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

namespace {
    // 绘制一个支持Alpha通道的实心矩形
    void DrawFilledRect(SDK::UCanvas* Canvas, const SDK::FVector2D& Pos, const SDK::FVector2D& Size, const SDK::FLinearColor& Color) {
        if (!Canvas || !Canvas->DefaultTexture) return;
        Canvas->K2_DrawTexture(Canvas->DefaultTexture, Pos, Size, SDK::FVector2D{ 0, 0 }, SDK::FVector2D{ 1, 1 }, Color, SDK::EBlendMode::BLEND_Translucent, 0.0f, SDK::FVector2D{ 0, 0 });
    }
}

namespace g_ESP {
    void FlagManager::AddFlag(SDK::UCanvas* Canvas, BoxRect rect, const std::string& text, SDK::FLinearColor color, FlagPos pos, float alphaMult) {
        if (!rect.valid || text.empty() || !Canvas) return;

        static SDK::UFont* MainFont = nullptr;

        if (!MainFont) {
            SDK::UObject* _Font = SDK::UObject::FindObject("Font SIMHEI_Font.SIMHEI_Font");
            if (_Font && _Font->IsA(SDK::UFont::StaticClass()))  MainFont = (SDK::UFont*)_Font;
        }

        if (!MainFont) return;
        if (!Shadow::DefaultFont) Shadow::DefaultFont = MainFont;

        SDK::UFont* currentFont = MainFont;

        // 在函数作用域内保留 wstr，确保FString拥有生命周期安全的指针
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wstr = converter.from_bytes(text);
        SDK::FString fText(wstr.c_str());

        // 缩放字体
        SDK::FVector2D scaleSize = SDK::FVector2D{ 1.0f, 1.0f };

        // 获取精确宽高
        SDK::FVector2D textSize = Canvas->K2_TextSize(currentFont, fText, scaleSize);
        SDK::FVector2D drawPos;

        // 四个方向的 Offset
        if (pos == FlagPos::Right) {
            drawPos = { (float)(rect.bottomRight.X + 5.0f), (float)(rect.topLeft.Y + rightY) };
            rightY += textSize.Y + 1.0f;
        }
        else if (pos == FlagPos::Left) {
            drawPos = { (float)(rect.topLeft.X - 6.0f - textSize.X), (float)(rect.topLeft.Y + leftY) };
            leftY += textSize.Y + 1.0f;
        }
        else if (pos == FlagPos::Top) {
            float centerX = (rect.topLeft.X + rect.bottomRight.X) * 0.5f;
            drawPos = { (float)(centerX - textSize.X * 0.5f), (float)(rect.topLeft.Y - topY - textSize.Y - 3.0f) };
            topY += textSize.Y + 1.0f;
        }
        else if (pos == FlagPos::Bottom) {
            float centerX = (rect.topLeft.X + rect.bottomRight.X) * 0.5f;
            drawPos = { (float)(centerX - textSize.X * 0.5f), (float)(rect.bottomRight.Y + bottomY + 3.0f) };
            bottomY += textSize.Y + 1.0f;
        }

        color.A *= alphaMult;
        SDK::FLinearColor shadowCol = { 0.0f, 0.0f, 0.0f, color.A };

        // 渲染文字
        Canvas->K2_DrawText(currentFont, fText, drawPos, scaleSize, color, 0.0f, shadowCol, SDK::FVector2D{ 1.0f, 1.0f }, false, false, true, shadowCol);
    }

    BoxRect GetBox(SDK::UCanvas* Canvas, SDK::AActor* Actor, float r, float g, float b, float a, float width_scale) {
        BoxRect rect;
        rect.valid = false;

        if (!Actor || Actor->bHidden) {
            return rect;
        }

        SDK::APlayerController* PC = g_Util::GetLocalPC();
        if (!PC) {
            return rect;
        }

        SDK::FVector origin, extent;
        Actor->GetActorBounds(true, &origin, &extent, false);

        SDK::FVector worldTop = { origin.X, origin.Y, origin.Z + extent.Z };
        SDK::FVector worldBottom = { origin.X, origin.Y, origin.Z - extent.Z };
        SDK::FVector2D screenTop, screenBottom;

        if (PC->ProjectWorldLocationToScreen(worldTop, &screenTop, false) &&
            PC->ProjectWorldLocationToScreen(worldBottom, &screenBottom, false)) {

            float height = std::abs(screenBottom.Y - screenTop.Y);
            float width = height * width_scale;
            rect.topLeft = { (float)(screenTop.X - width * 0.5f), (float)screenTop.Y };
            rect.bottomRight = { (float)(screenTop.X + width * 0.5f), (float)screenBottom.Y };
            rect.valid = true;
        }

        return rect;
    }

    void DrawBox(SDK::UCanvas* Canvas, const BoxRect& rect, SDK::FLinearColor color, float alpha) {
        if (!rect.valid || alpha < 0.001f || !Canvas) return;

        color.A *= alpha;

        float x = rect.topLeft.X;
        float y = rect.topLeft.Y;
        float w = rect.bottomRight.X - rect.topLeft.X;
        float h = rect.bottomRight.Y - rect.topLeft.Y;

        float thickness = 1.0f; // 边框线条粗细

        // 1. 绘制黑色外边框阴影
        SDK::FLinearColor shadowCol = { 0.0f, 0.0f, 0.0f, color.A };
        float s_t = thickness + 2.0f; // 阴影比主线稍微粗一点点

        DrawFilledRect(Canvas, { x - 1.0f, y - 1.0f }, { w + 2.0f, s_t }, shadowCol); // 顶
        DrawFilledRect(Canvas, { x - 1.0f, y + h - thickness - 1.0f }, { w + 2.0f, s_t }, shadowCol); // 底
        DrawFilledRect(Canvas, { x - 1.0f, y - 1.0f }, { s_t, h + 2.0f }, shadowCol); // 左
        DrawFilledRect(Canvas, { x + w - thickness - 1.0f, y - 1.0f }, { s_t, h + 2.0f }, shadowCol); // 右

        // 2. 绘制彩色主边框
        DrawFilledRect(Canvas, { x, y }, { w, thickness }, color); // 顶
        DrawFilledRect(Canvas, { x, y + h - thickness }, { w, thickness }, color); // 底
        DrawFilledRect(Canvas, { x, y }, { thickness, h }, color); // 左
        DrawFilledRect(Canvas, { x + w - thickness, y }, { thickness, h }, color); // 右
    }
}