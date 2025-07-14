// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UProjectileWeaponComponent;

UCLASS(config=Game)
class APhysicsProjectile : public AActor
{
	GENERATED_BODY()

public:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** Destroy projectile on Hit*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool m_DestroyOnHit;

	UPROPERTY(EditAnywhere)
	float m_Radius;

	UPROPERTY(EditAnywhere)
	float m_Strength;

	UPROPERTY(EditAnywhere)
	float m_Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage)
	UProjectileWeaponComponent* m_OwnerWeapon;
public:
	APhysicsProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

