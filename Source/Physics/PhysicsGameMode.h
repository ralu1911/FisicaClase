#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PhysicsGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinConditionMet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetCountChange);

class ABreakableTarget;

UCLASS(minimalapi)
class APhysicsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APhysicsGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReduceRemainingTargets(ABreakableTarget* BrokenTarget);

public:
	UPROPERTY(BlueprintAssignable)
	FWinConditionMet OnWinConditionMet;
	UPROPERTY(BlueprintAssignable)
	FTargetCountChange OnTargetCountChange;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameData, meta = (AllowPrivateAccess = "true"))
	int m_TotalTargets;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameData, meta = (AllowPrivateAccess = "true"))
	int m_RemainingTargets;
};



