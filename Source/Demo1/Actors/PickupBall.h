#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "PickupBall.generated.h"

UCLASS()
class DEMO1_API APickupBall : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupBall();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void Pickup(const AActor* HolderNPC);
	UFUNCTION()
	void Drop(const FVector& Direction, float Power) const;

protected:
	USceneComponent* GetHoldingSlot(const AActor* HolderNPC) const;
};
