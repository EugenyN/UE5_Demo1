#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include "D1Character.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AD1Character : public ACharacter
{
	GENERATED_BODY()
public:
	AD1Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ItemDropPower = 100000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	class APickupBall* HoldingItem = nullptr;

	void Block(bool bBlock);

	bool IsBlocked() const { return bBlocked; }
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	APickupBall* GetItemInView() const;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void PickupItem();
	void DropItem();
	void DisableCollisionWithItem(float Time) const;
	
	struct FTouchData
	{
		FTouchData(): FingerIndex(), bMoved(false)
		{
			bIsPressed = false;
			Location = FVector::ZeroVector;
		}

		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	FTouchData	TouchItem;

	bool EnableTouchscreenMovement(UInputComponent* InputComponent);
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* ArmsMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* HoldingSlot;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	class USoundBase* ActionSound;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	class UAnimMontage* ActionAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bBlocked = false;
};

