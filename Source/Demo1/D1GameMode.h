#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>
#include "D1GameMode.generated.h"

class ANPC;

extern int32 DebugGizmoDraw;

UCLASS(minimalapi)
class AD1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AD1GameMode();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float SurroundCircleRadius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float GuardCircleRadius = 500;
	
	FVector CircleCenter;
	TMap<ANPC*, FVector> SurroundPoints;

	void UpdateSurroundCircle(const FVector& Target);
protected:
	virtual void BeginPlay() override;

private:
	TArray<FVector> GetSurroundPositions(const FVector& Center, uint8 PositionsCount) const;
	bool IsReachablePoint(const FVector& SourcePoint, const FVector& TargetPoint) const;
	TMap<ANPC*, FVector> GetSurroundData(const FVector& Center) const;
};



