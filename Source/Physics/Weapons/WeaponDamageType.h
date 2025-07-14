#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "WeaponDamageType.generated.h"

UENUM(BlueprintType)
enum class EImpulseType : uint8
{
	RAY,
	POINT,
	RADIAL
};

UCLASS(Blueprintable, EditInlineNew)
class PHYSICS_API UWeaponDamageType : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EImpulseType m_ImpulseType;


	UPROPERTY(EditAnywhere)
	float m_Damage;
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> m_DamageType;
};
