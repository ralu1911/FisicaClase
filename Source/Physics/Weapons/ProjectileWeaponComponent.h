#pragma once

#include "CoreMinimal.h"
#include "Weapons/PhysicsWeaponComponent.h"
#include "ProjectileWeaponComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PHYSICS_API UProjectileWeaponComponent : public UPhysicsWeaponComponent
{
	GENERATED_BODY()

public:

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class APhysicsProjectile> m_ProjectileClass;

	UPROPERTY(EditAnywhere)
	bool bProjectileExplode;
public:
	/** UPhysicsWeaponComponent **/
	virtual void Fire() override;
};
