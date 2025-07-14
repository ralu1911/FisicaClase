// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsGameMode.h"
#include "PhysicsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "BreakableTarget.h"
#include <Kismet/GameplayStatics.h>

APhysicsGameMode::APhysicsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void APhysicsGameMode::BeginPlay()
{
	Super::BeginPlay();
  TArray<AActor*> outActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABreakableTarget::StaticClass(), outActors);
  m_TotalTargets = outActors.Num();
  m_RemainingTargets = outActors.Num();
  OnTargetCountChange.Broadcast();
  ABreakableTarget::OnBreak.AddDynamic(this, &APhysicsGameMode::ReduceRemainingTargets);
}

void APhysicsGameMode::ReduceRemainingTargets(ABreakableTarget* BrokenTarget)
{
  GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Salta el delegado"));
  --m_RemainingTargets;
  OnTargetCountChange.Broadcast();
  if (m_RemainingTargets <= 0)
  {
    OnWinConditionMet.Broadcast();
  }
}

