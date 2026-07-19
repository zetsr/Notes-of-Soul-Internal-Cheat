#include "../../external/CppSDK/SDK.hpp"
#include "DrawESP.h"
#include "ESP.h"
#include "../Config/Configs.h"
#include "../Util/Util.h"
#include <string>
#include <algorithm>
#include <format>

namespace g_DrawESP {
    void DrawESP(SDK::UCanvas* Canvas) {
        if (!Canvas) return;

        SDK::UWorld* World = SDK::UWorld::GetWorld();
        if (!World) return;

        SDK::APlayerController* LocalPC = g_Util::GetLocalPC();
        if (!LocalPC) return;

        SDK::APlayerState* LocalPS = LocalPC->PlayerState;
        if (!LocalPS) return;

        SDK::AKxPlayerState* LocalKxPS = static_cast<SDK::AKxPlayerState*>(LocalPS);

        // 魔法攻击：鬼传送所有人类到身边
        if (g_Config::kMagicAttack) {
            SDK::APawn* LocalPawn = LocalPC->K2_GetPawn();
            if (LocalPawn && LocalPawn->IsA(SDK::ABP_PlayerGhost_C::StaticClass())) {
                SDK::FVector localLocation = LocalPawn->K2_GetActorLocation();

                SDK::TArray<SDK::AActor*> AllActors;
                SDK::UGameplayStatics::GetAllActorsOfClass(
                    World,
                    SDK::ABP_PlayerHuman_C::StaticClass(),
                    &AllActors
                );

                for (size_t j = 0; j < AllActors.Num(); j++) {
                    SDK::AActor* HumanActor = AllActors[j];
                    if (HumanActor && !HumanActor->bHidden) {
                        HumanActor->K2_SetActorLocation(localLocation, false, nullptr, false);
                    }
                }
            }
        }

        SDK::TArray<SDK::AActor*> Actors;
        SDK::UGameplayStatics::GetAllActorsOfClass(
            World,
            SDK::ACharacter::StaticClass(),
            &Actors
        );

        for (size_t i = 0; i < Actors.Num(); i++) {
            SDK::AActor* Actor = Actors[i];
            if (!Actor || Actor->bHidden) continue;

            if (!Actor->IsA(SDK::AKxPlayerBase::StaticClass())) {
                continue;
            }

            SDK::AKxPlayerBase* Player = static_cast<SDK::AKxPlayerBase*>(Actor);
            if (!Player) continue;

            if (Player->GetPlayerIsDead() || Player->bIsOB) continue;

            SDK::APlayerState* PS = Player->PlayerState;
            if (!PS) continue;

            // 跳过本地玩家
            if (PS == LocalPS) continue;

            SDK::AKxPlayerState* KxPS = static_cast<SDK::AKxPlayerState*>(PS);
            if (!KxPS) continue;

            // 阵营判断：以本地玩家视角判断对方是否为敌人
            bool isEnemy = false;
            if (LocalKxPS) {
                isEnemy = LocalKxPS->CheckIsEnemy(KxPS);
            }

            // 根据阵营读取不同配置
            bool bShowESP = isEnemy ? g_Config::bESP_Enemy : g_Config::bESP_Teammate;
            if (!bShowESP) continue;

            g_ESP::BoxRect rect = g_ESP::GetBox(Canvas, Actor, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f);
            if (!rect.valid) continue;

            std::string playerName = KxPS->PlayerNamePlatform.ToString();
            float health = Player->Health;
            float maxHealth = Player->MaxHealthValue;
            float armor = Player->ArmorValue;

            g_ESP::FlagManager flagMgr;

            bool bShowName = isEnemy ? g_Config::bESP_Enemy_Name : g_Config::bESP_Teammate_Name;
            bool bShowHealth = isEnemy ? g_Config::bESP_Enemy_Health : g_Config::bESP_Teammate_Health;

            // 1. 名字 (白色)
            if (bShowName && !playerName.empty()) {
                flagMgr.AddFlag(Canvas, rect, playerName,
                    SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f },
                    g_ESP::FlagPos::Right, 1.0f);
            }

            // 2. 血量 (使用 g_Util::GetHealthColor)
            if (bShowHealth) {
                SDK::FLinearColor healthColor = g_Util::GetHealthColor(health, maxHealth);
                int percent = maxHealth > 0 ? static_cast<int>((health / maxHealth) * 100.0f) : 0;
                std::string healthText = std::format("{:.0f}/{:.0f} ({}%)", health, maxHealth, percent);
                flagMgr.AddFlag(Canvas, rect, healthText,
                    healthColor, g_ESP::FlagPos::Right, 1.0f);
            }

            SDK::ABP_PlayerHuman_C* Human = nullptr;
            if (Player->IsA(SDK::ABP_PlayerHuman_C::StaticClass())) {
                Human = static_cast<SDK::ABP_PlayerHuman_C*>(Player);
            }

            if (Human) {
                // 1. 特殊状态
                if (Human->IsHealthLocked) {
                    flagMgr.AddFlag(Canvas, rect, "无敌",
                        SDK::FLinearColor{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄色
                        g_ESP::FlagPos::Right, 1.0f);
                }

                if (Human->IsProtectingSoul) {
                    flagMgr.AddFlag(Canvas, rect, "护魄",
                        SDK::FLinearColor{ 1.0f, 0.5f, 0.0f, 1.0f }, // 橙色
                        g_ESP::FlagPos::Right, 1.0f);
                }

                if (Human->IsPlayDead) {
                    flagMgr.AddFlag(Canvas, rect, "装死",
                        SDK::FLinearColor{ 0.5f, 0.5f, 0.5f, 1.0f }, // 灰色
                        g_ESP::FlagPos::Right, 1.0f);
                }

                // 2. 被控制状态
                if (Human->IsCatchByZhiRen || Human->IsTrapRopedByXiaoBai) {
                    flagMgr.AddFlag(Canvas, rect, "被控",
                        SDK::FLinearColor{ 1.0f, 0.0f, 0.0f, 1.0f }, // 红色
                        g_ESP::FlagPos::Right, 1.0f);
                }

                // 3. 物品显示
                if (Human->IsHoldingItem) {
                    flagMgr.AddFlag(Canvas, rect, "持物",
                        SDK::FLinearColor{ 0.0f, 1.0f, 1.0f, 1.0f }, // 青色
                        g_ESP::FlagPos::Right, 1.0f);
                }
            }
        }
    }
}