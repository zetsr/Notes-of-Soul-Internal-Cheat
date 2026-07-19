#pragma once

namespace g_ESP {
    struct BoxRect {
        SDK::FVector2D topLeft;
        SDK::FVector2D bottomRight;
        bool valid = false;
    };

    enum class FlagPos {
        Left,
        Right,
        Top,
        Bottom
    };

    class FlagManager {
    private:
        float leftY = 0.0f;
        float rightY = 0.0f;
        float topY = 0.0f;
        float bottomY = 0.0f;

    public:
        void Reset() {
            leftY = 0.0f;
            rightY = 0.0f;
            topY = 0.0f;
            bottomY = 0.0f;
        }

        void AddFlag(SDK::UCanvas* Canvas, BoxRect rect, const std::string& text, SDK::FLinearColor color, FlagPos pos, float alphaMult);
    };

    BoxRect GetBox(SDK::UCanvas* Canvas, SDK::AActor* entity, float r, float g, float b, float a, float width_scale);
    void DrawBox(SDK::UCanvas* Canvas, const BoxRect& rect, SDK::FLinearColor color, float alpha);

    struct OOFFlag {
        std::string text;
        SDK::FLinearColor color;
    };
}