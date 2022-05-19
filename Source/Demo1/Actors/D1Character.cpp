#include "D1Character.h"
#include "PickupBall.h"

#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/InputComponent.h>
#include <GameFramework/InputSettings.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>


AD1Character::AD1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.f, 88.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	ArmsMesh->SetOnlyOwnerSee(true);
	ArmsMesh->SetupAttachment(FirstPersonCameraComponent);
	ArmsMesh->bCastDynamicShadow = false;
	ArmsMesh->CastShadow = false;
	ArmsMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	ArmsMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	HoldingSlot = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingSlot"));
	HoldingSlot->SetupAttachment(FirstPersonCameraComponent);
	HoldingSlot->SetRelativeLocation(FVector(70.0f, 0.0f, -35.0f));
}

void AD1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("PickupItem", IE_Pressed, this, &AD1Character::PickupItem);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AD1Character::DropItem);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AD1Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AD1Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AD1Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AD1Character::LookUpAtRate);
}

APickupBall* AD1Character::GetItemInView() const
{
	const float HitDistance = 300.f;
	FVector TraceStart = FirstPersonCameraComponent->GetComponentLocation();
	FVector TraceEnd = FirstPersonCameraComponent->GetForwardVector() * HitDistance + TraceStart;

	FHitResult Hit;
	FComponentQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (Hit.GetActor()->GetClass()->IsChildOf(APickupBall::StaticClass()))
			return Cast<APickupBall>(Hit.GetActor());
	}

	return nullptr;
}

void AD1Character::PickupItem()
{
	if (HoldingItem != nullptr)
		return;

	if (ActionAnimation != nullptr) {
		UAnimInstance* AnimInstance = ArmsMesh->GetAnimInstance();
		if (AnimInstance != nullptr)
			AnimInstance->Montage_Play(ActionAnimation, 1.f);
	}

	HoldingItem = GetItemInView();
	if (HoldingItem != nullptr) {
		HoldingItem->Pickup(this);

		if (ActionSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, ActionSound, GetActorLocation());
	}
}

void AD1Character::DropItem()
{
	if (ActionAnimation != nullptr) {
		UAnimInstance* AnimInstance = ArmsMesh->GetAnimInstance();
		if (AnimInstance != nullptr)
			AnimInstance->Montage_Play(ActionAnimation, 1.f);
	}

	if (HoldingItem != nullptr) 
	{
		// disable collision with item for 1 sec when drop item.
		DisableCollisionWithItem(1.0f);

		auto PlayerCamera = FindComponentByClass<UCameraComponent>();
		HoldingItem->Drop(PlayerCamera->GetForwardVector(), ItemDropPower);

		if (ActionSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, ActionSound, GetActorLocation());

		HoldingItem = nullptr;
	}
}

void AD1Character::DisableCollisionWithItem(float Time) const
{
	auto RootComp = GetCapsuleComponent();
	if (RootComp != nullptr) 
	{
		RootComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
		FTimerDelegate RestoreCollisionCallback;
		RestoreCollisionCallback.BindLambda([RootComp] {
			RootComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
			});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, RestoreCollisionCallback, Time, false);
	}
}

void AD1Character::Block(bool bBlock)
{
	bBlocked = bBlock;

	//TEMP WORKAROUND: remove collision between Player and NPC when blocked
	//auto RootComponent = GetCapsuleComponent();
	//if (RootComponent != nullptr) {
	//	RootComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
	//		bBlock ? ECollisionResponse::ECR_Ignore : ECollisionResponse::ECR_Block);
	//}
}

void AD1Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
		return;
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
		PickupItem();
	
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AD1Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AD1Character::MoveForward(float Value)
{
	if (bBlocked)
		return;
	
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AD1Character::MoveRight(float Value)
{
	if (bBlocked)
		return;
	
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AD1Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AD1Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AD1Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (bBlocked)
		return false;
	
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AD1Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AD1Character::EndTouch);

		return true;
	}
	
	return false;
}
