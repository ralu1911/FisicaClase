// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsCharacter.h"
#include "PhysicsProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/StaticMeshComponent.h>
#include <PhysicsEngine/PhysicsHandleComponent.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APhysicsCharacter::APhysicsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	m_PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}
void APhysicsCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterMovementComponent = GetCharacterMovement();
	m_CurrentStamina = m_MaxStamina;
	m_bBlockSprint = false;

}

void APhysicsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateStamina(DeltaSeconds);

	SearchGrabbableObjects();

	SetLocationGrabbedObject();
}

void APhysicsCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APhysicsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APhysicsCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhysicsCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APhysicsCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APhysicsCharacter::Sprint);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &APhysicsCharacter::GrabObject);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Completed, this, &APhysicsCharacter::ReleaseObject);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APhysicsCharacter::Sprinting(bool _IsSprinting)
{
	m_bRunningBlock = _IsSprinting;
  if (!_IsSprinting)
  {
    m_bBlockSprint = false;
  }


	if (_IsSprinting && m_CurrentStamina > 0 && !m_bBlockSprint)
	{
		CharacterMovementComponent->MaxWalkSpeed = m_WalkSpeed * m_SprintSpeedMultiplier;
	}
	else
	{
		CharacterMovementComponent->MaxWalkSpeed = m_WalkSpeed;

	}
}

float APhysicsCharacter::GetStamina() const
{
	return m_CurrentStamina;
}

void APhysicsCharacter::SetLocationGrabbedObject()
{
	if (!m_GrabbedComponent)
	{
		return;
	}
	FVector Forward = FirstPersonCameraComponent->GetForwardVector();
	FVector Location = GetActorLocation() + Forward * m_DistanceWithGrabbedObject;
	m_PhysicsHandle->SetTargetLocation(Location);
}

void APhysicsCharacter::SearchGrabbableObjects()
{
  FHitResult Hit;
  FVector Start = GetActorLocation() + FirstPersonCameraComponent->GetRelativeLocation();
  FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * m_MaxGrabDistance);
  GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);


  if (m_HighlightedMesh != nullptr)
  {
    SetHighlightedMesh(nullptr);
  }

	if (!Hit.GetActor())
	{

		return;
	}
  if (!(Hit.GetComponent()->Mobility == EComponentMobility::Movable))
  {
    return;
  }
  if (!Hit.GetComponent()->IsSimulatingPhysics())
  {
    return;
  }

	UMeshComponent* MeshComponent = Hit.GetActor()->GetComponentByClass<UMeshComponent>();
	SetHighlightedMesh(MeshComponent);
}

void APhysicsCharacter::UpdateStamina(float DeltaSeconds)
{
	if (!CharacterMovementComponent)
	{
		CharacterMovementComponent = GetCharacterMovement();
	}

	if (CharacterMovementComponent->GetVelocityForNavMovement().Length() >= m_WalkSpeed * m_SprintSpeedMultiplier)
	{
		float AuxStamina = m_CurrentStamina - m_StaminaDepletionRate * DeltaSeconds;
    if (AuxStamina < 0)
    {
      m_CurrentStamina = 0;
    }
    else
    {
      m_CurrentStamina = AuxStamina;
    }

		if (m_CurrentStamina <= KINDA_SMALL_NUMBER)
		{
			m_bBlockSprint = true;
		}
	}
	else if(m_bRunningBlock)
	{
    float AuxStamina = m_CurrentStamina + m_StaminaRecoveryRate * DeltaSeconds;
    if (AuxStamina > m_MaxStamina)
    {
      m_CurrentStamina = m_MaxStamina;
    }
    else
    {
      m_CurrentStamina = AuxStamina;
    }
	}
	else
	{
		float AuxStamina = m_CurrentStamina + m_StaminaRecoveryRate * DeltaSeconds;
    if (AuxStamina > m_MaxStamina)
    {
      m_CurrentStamina = m_MaxStamina;
    }
    else
    {
      m_CurrentStamina = AuxStamina;
    }
	}
}

void APhysicsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APhysicsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APhysicsCharacter::Sprint(const FInputActionValue& Value)
{
	Sprinting(Value.Get<bool>());
}

void APhysicsCharacter::GrabObject(const FInputActionValue& Value)
{
	// @TODO: Grab objects using UPhysicsHandleComponent
  if (m_GrabbedComponent)
  {
    return;
  }

  FHitResult hit;
  FVector start = GetActorLocation() + FirstPersonCameraComponent->GetRelativeLocation();
  FVector end = start + (FirstPersonCameraComponent->GetForwardVector() * m_MaxGrabDistance);
  GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility);
	if (!hit.GetActor() || !(hit.GetComponent()->Mobility == EComponentMobility::Movable))
	{
		return;
	}
	
  m_GrabbedComponent = hit.GetActor()->GetComponentByClass<UPrimitiveComponent>();

	if (hit.GetActor()->Tags.Contains("Door"))
	{
		m_PhysicsHandle->GrabComponentAtLocation(m_GrabbedComponent, hit.BoneName, hit.Location);
	}
	else
	{
		m_PhysicsHandle->GrabComponentAtLocationWithRotation(m_GrabbedComponent, hit.BoneName, hit.Location, hit.GetActor()->GetActorRotation());
	}
	m_DistanceWithGrabbedObject = hit.Distance;
	
}


void APhysicsCharacter::SetHighlightedMesh(UMeshComponent* StaticMesh)
{
	if (m_HighlightedMesh)
	{
		m_HighlightedMesh->SetOverlayMaterial(nullptr);
	}
	m_HighlightedMesh = StaticMesh;
	if (m_HighlightedMesh)
	{
		m_HighlightedMesh->SetOverlayMaterial(m_HighlightMaterial);
	}
}

void APhysicsCharacter::ReleaseObject(const FInputActionValue& Value)
{
	// @TODO: Release grabebd object using UPhysicsHandleComponent
	m_DistanceWithGrabbedObject = 0;
	m_GrabbedComponent = nullptr;
	m_PhysicsHandle->ReleaseComponent();
}