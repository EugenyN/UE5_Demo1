#include "NPC.h"
#include "NPCAIController.h"
#include "PickupBall.h"

#include <UObject/ConstructorHelpers.h>
#include <Engine/SkeletalMesh.h>
#include <Components/CapsuleComponent.h>


ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(
		TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(
		TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'"));

	if (MeshFinder.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshFinder.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (AnimFinder.Succeeded())
			GetMesh()->SetAnimInstanceClass(AnimFinder.Class);
	}

	HoldingSlot = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingSlot"));
	HoldingSlot->SetupAttachment(RootComponent);
	HoldingSlot->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ANPCAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(25.f, 88.0f);
}

void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPC::PickupItem(APickupBall* Item)
{
	if (Item == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Target == nullptr"));
		return;
	}

	Item->Pickup(this);
	HoldingItem = Item;
}

void ANPC::DropItem()
{
	if (HoldingItem == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("HoldingItem == nullptr"));
		return;
	}

	HoldingItem->Drop(GetActorForwardVector(), ItemDropPower);
	HoldingItem = nullptr;
}