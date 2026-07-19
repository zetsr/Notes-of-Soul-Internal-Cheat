#include "../../external/CppSDK/SDK.hpp"
#include "DrawESP.h"
#include "ESP.h"
#include "../Config/Configs.h"
#include "../Util/Util.h"
#include "../../XorStr.h"
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

        if (g_Config::bEnableMagicAttack) {
            SDK::APawn* LocalPawn = LocalPC->K2_GetPawn();
            if (LocalPawn && LocalPawn->IsA(SDK::ABP_PlayerGhost_C::StaticClass())) {
                SDK::FVector localLocation = LocalPawn->K2_GetActorLocation();

                SDK::TArray<SDK::AActor*> AllActors;
                SDK::UGameplayStatics::GetAllActorsOfClass(
                    World,
                    SDK::ABP_PlayerHuman_C::StaticClass(),
                    &AllActors
                );

                float closestDistance = g_Config::fMagicAttackRange * 100;
                SDK::AActor* closestHuman = nullptr;

                for (size_t j = 0; j < AllActors.Num(); j++) {
                    SDK::AActor* HumanActor = AllActors[j];
                    if (HumanActor && !HumanActor->bHidden) {
                        float distance = LocalPawn->GetDistanceTo(HumanActor);
                        if (distance <= g_Config::fMagicAttackRange * 100 && distance < closestDistance) {
                            closestDistance = distance;
                            closestHuman = HumanActor;
                        }
                    }
                }

                if (closestHuman) {
                    closestHuman->K2_SetActorLocation(localLocation, false, nullptr, false);
                }
            }
        }

        SDK::TArray<SDK::AActor*> Actors;
        SDK::UGameplayStatics::GetAllActorsOfClass(
            World,
            SDK::ACharacter::StaticClass(),
            &Actors
        );

        SDK::APawn* LocalPawn = LocalPC->K2_GetPawn();
        bool bLocalIsGhost = false;
        if (LocalPawn && LocalPawn->IsA(SDK::ABP_PlayerGhost_C::StaticClass())) {
            bLocalIsGhost = true;
        }

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

            if (PS == LocalPS) continue;

            SDK::AKxPlayerState* KxPS = static_cast<SDK::AKxPlayerState*>(PS);
            if (!KxPS) continue;

            bool bTargetIsGhost = Actor->IsA(SDK::ABP_PlayerGhost_C::StaticClass());

            bool isEnemy = false;
            if (bLocalIsGhost) {
                isEnemy = !bTargetIsGhost;
            }
            else {
                isEnemy = bTargetIsGhost;
            }

            bool bShowESP = isEnemy ? g_Config::bESP_Enemy : g_Config::bESP_Teammate;
            if (!bShowESP) continue;

            g_ESP::BoxRect rect = g_ESP::GetBox(Canvas, Actor, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f);
            if (!rect.valid) continue;

            std::string playerName = KxPS->PlayerNamePlatform.ToString();
            float health = Player->Health;
            float maxHealth = Player->MaxHealthValue;
            float armor = Player->ArmorValue;

            float distance = (LocalPC && LocalPC->Pawn && Actor) ? (LocalPC->Pawn->GetDistanceTo(Actor) / 100.0f) : 0.0f;

            g_ESP::FlagManager flagMgr;

            bool bShowName = isEnemy ? g_Config::bESP_Enemy_Name : g_Config::bESP_Teammate_Name;
            bool bShowDistance = isEnemy ? g_Config::bESP_Enemy_Distance : g_Config::bESP_Teammate_Distance;
            bool bShowHealth = isEnemy ? g_Config::bESP_Enemy_Health : g_Config::bESP_Teammate_Health;

            const float* nameColor = isEnemy ? g_Config::enemyNameColor : g_Config::teammateNameColor;
            const float* distanceColor = isEnemy ? g_Config::enemyDistanceColor : g_Config::teammateDistanceColor;

            if (bShowName && !playerName.empty()) {
                flagMgr.AddFlag(Canvas, rect, playerName,
                    SDK::FLinearColor{ nameColor[0], nameColor[1], nameColor[2], nameColor[3] },
                    g_ESP::FlagPos::Right, 1.0f);
            }

            if (bShowDistance) {
                std::string distanceText = std::vformat(
                    _XOR_("{:.1f}m").str(),
                    std::make_format_args(distance)
                );
                flagMgr.AddFlag(Canvas, rect, distanceText,
                    SDK::FLinearColor{ distanceColor[0], distanceColor[1], distanceColor[2], distanceColor[3] },
                    g_ESP::FlagPos::Right, 1.0f);
            }

            if (bShowHealth) {
                SDK::FLinearColor healthColor = g_Util::GetHealthColor(health, maxHealth);
                int percent = maxHealth > 0 ? static_cast<int>((health / maxHealth) * 100.0f) : 0;

                std::string healthText = std::vformat(
                    _XOR_("{:.0f}/{:.0f} ({}%)").str(),
                    std::make_format_args(health, maxHealth, percent)
                );
                flagMgr.AddFlag(Canvas, rect, healthText,
                    healthColor, g_ESP::FlagPos::Right, 1.0f);
            }

            SDK::ABP_PlayerHuman_C* Human = nullptr;
            if (Player->IsA(SDK::ABP_PlayerHuman_C::StaticClass())) {
                Human = static_cast<SDK::ABP_PlayerHuman_C*>(Player);
            }

            if (Human) {
                bool bShowInvincible = isEnemy ? g_Config::bESP_Enemy_Invincible : g_Config::bESP_Teammate_Invincible;
                bool bShowProtectSoul = isEnemy ? g_Config::bESP_Enemy_ProtectSoul : g_Config::bESP_Teammate_ProtectSoul;
                bool bShowPlayDead = isEnemy ? g_Config::bESP_Enemy_PlayDead : g_Config::bESP_Teammate_PlayDead;
                bool bShowControlled = isEnemy ? g_Config::bESP_Enemy_Controlled : g_Config::bESP_Teammate_Controlled;
                bool bShowHoldingItem = isEnemy ? g_Config::bESP_Enemy_HoldingItem : g_Config::bESP_Teammate_HoldingItem;

                const float* invincibleColor = isEnemy ? g_Config::enemyInvincibleColor : g_Config::teammateInvincibleColor;
                const float* protectSoulColor = isEnemy ? g_Config::enemyProtectSoulColor : g_Config::teammateProtectSoulColor;
                const float* playDeadColor = isEnemy ? g_Config::enemyPlayDeadColor : g_Config::teammatePlayDeadColor;
                const float* controlledColor = isEnemy ? g_Config::enemyControlledColor : g_Config::teammateControlledColor;
                const float* holdingItemColor = isEnemy ? g_Config::enemyHoldingItemColor : g_Config::teammateHoldingItemColor;

                if (bShowInvincible && Human->IsHealthLocked) {
                    flagMgr.AddFlag(Canvas, rect, _XOR_("无敌").str(),
                        SDK::FLinearColor{ invincibleColor[0], invincibleColor[1], invincibleColor[2], invincibleColor[3] },
                        g_ESP::FlagPos::Right, 1.0f);
                }

                if (bShowProtectSoul && Human->IsProtectingSoul) {
                    flagMgr.AddFlag(Canvas, rect, _XOR_("护魄").str(),
                        SDK::FLinearColor{ protectSoulColor[0], protectSoulColor[1], protectSoulColor[2], protectSoulColor[3] },
                        g_ESP::FlagPos::Right, 1.0f);
                }

                if (bShowPlayDead && Human->IsPlayDead) {
                    flagMgr.AddFlag(Canvas, rect, _XOR_("装死").str(),
                        SDK::FLinearColor{ playDeadColor[0], playDeadColor[1], playDeadColor[2], playDeadColor[3] },
                        g_ESP::FlagPos::Right, 1.0f);
                }

                if (bShowControlled && (Human->IsCatchByZhiRen || Human->IsTrapRopedByXiaoBai)) {
                    flagMgr.AddFlag(Canvas, rect, _XOR_("被控").str(),
                        SDK::FLinearColor{ controlledColor[0], controlledColor[1], controlledColor[2], controlledColor[3] },
                        g_ESP::FlagPos::Right, 1.0f);
                }

                if (bShowHoldingItem && Human->IsHoldingItem) {
                    flagMgr.AddFlag(Canvas, rect, _XOR_("持物").str(),
                        SDK::FLinearColor{ holdingItemColor[0], holdingItemColor[1], holdingItemColor[2], holdingItemColor[3] },
                        g_ESP::FlagPos::Right, 1.0f);
                }
            }
        }
    }
}