// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetBreak, ABreakableTarget*, _target);

UCLASS()
class PHYSICS_API ABreakableTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableTarget();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UGeometryCollectionComponent* GeometryCollection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	bool m_IsBroken = false;

public:	
	UFUNCTION()
	void GeometryCollectionBroken(const struct FChaosBreakEvent& BreakEvent);

  UFUNCTION(BlueprintCallable)
  void StatueDestruction();
  static FOnTargetBreak OnBreak;
};
