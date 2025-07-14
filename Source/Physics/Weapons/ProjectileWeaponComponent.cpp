// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponComponent.h"
#include "PhysicsCharacter.h"
#include "PhysicsProjectile.h"

void UProjectileWeaponComponent::Fire()
{
	Super::Fire();

	// Try and fire a projectile
  if (m_ProjectileClass == nullptr)
  {
    return;
  }
	UWorld* const World = GetWorld();
  if (World == nullptr)
  {
    return;
  }

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	APhysicsProjectile* ProjectileActor = World->SpawnActor<APhysicsProjectile>(m_ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	if (ProjectileActor)
	{
		ProjectileActor->m_OwnerWeapon = this;
	}
	
}
