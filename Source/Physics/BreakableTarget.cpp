#include "BreakableTarget.h"
#include <GeometryCollection/GeometryCollectionComponent.h>


FOnTargetBreak ABreakableTarget::OnBreak;
// Sets default values
ABreakableTarget::ABreakableTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(StaticMesh);
  GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableTarget::GeometryCollectionBroken);
  GeometryCollection->SetNotifyBreaks(true);
}

void ABreakableTarget::GeometryCollectionBroken(const FChaosBreakEvent& BreakEvent)
{
	// @TODO: Call this function when the geometry collection breaks
  StatueDestruction();
}

void ABreakableTarget::StatueDestruction()
{
  if (!m_IsBroken)
  {
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Salta el rompìdo"));
    OnBreak.Broadcast(this);
    GeometryCollection->SetNotifyBreaks(false);
    m_IsBroken = true;
  }
}

