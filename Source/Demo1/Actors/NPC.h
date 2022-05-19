#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include "NPC.generated.h"

UCLASS()
class DEMO1_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ItemDropPower = 10000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	class APickupBall* HoldingItem;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PickupItem(APickupBall* Item);
	void DropItem();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* HoldingSlot;
};
