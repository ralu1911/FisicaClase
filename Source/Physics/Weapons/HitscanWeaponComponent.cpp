// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/HitscanWeaponComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "PhysicsCharacter.h"
#include "PhysicsWeaponComponent.h"
#include <Camera/CameraComponent.h>
#include <Components/SphereComponent.h>

void UHitscanWeaponComponent::Fire()
{
	Super::Fire();

	FVector vStart = GetComponentLocation();
	FVector vForwardVector = Character->FirstPersonCameraComponent->GetForwardVector();
	FVector vEnd = vStart + (vForwardVector * m_fRange);

	// @TODO: Add firing functionality
	if (GetOwner())
	{
    FHitResult oHitResult;
    FCollisionQueryParams oParams;
    if (GetWorld()->LineTraceSingleByChannel(oHitResult, vStart, vEnd, ECC_Visibility, oParams))
    {
      AActor* OtherActor = oHitResult.GetActor();
      ApplyDamage(OtherActor, oHitResult);
      onHitscanImpact.Broadcast(OtherActor, oHitResult.ImpactPoint, vForwardVector);
    }
	}
}
