#include "PickupBall.h"

#include <GameFramework/Character.h>
#include <Components/StaticMeshComponent.h>

APickupBall::APickupBall()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;
}

USceneComponent* APickupBall::GetHoldingSlot(const AActor* HolderNPC) const
{
	// alternatively we can use interface IPickupItemHolder { APickupItem* HoldingItem; USceneComponent* GetHoldingSlot(); }

	TArray<USceneComponent*> Components;
	HolderNPC->GetComponents(Components);
	if (Components.Num() > 0)
	{
		for (const auto& Comp : Components) {
			if (Comp->GetName() == "HoldingSlot")
				return Cast<USceneComponent>(Comp);
		}
	}
	return nullptr;
}

void APickupBall::Pickup(const AActor* HolderNPC)
{
	// another way is to use UPhysicsHandleComponent::GrabComponentAtLocationWithRotation/ReleaseComponent

	const auto HoldingSlot = GetHoldingSlot(HolderNPC);
	if (HoldingSlot == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("'HoldingSlot' not found !"));
		return;
	}

	MeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->AttachToComponent(HoldingSlot, FAttachmentTransformRules::KeepRelativeTransform);
	SetActorLocation(HoldingSlot->GetComponentLocation());
}

void APickupBall::Drop(const FVector& Direction, float Power) const
{
	MeshComponent->SetEnableGravity(true);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MeshComponent->AddForce(Direction * Power * MeshComponent->GetMass());
}
