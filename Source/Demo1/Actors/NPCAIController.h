#pragma once

#include <CoreMinimal.h>
#include <AIController.h>
#include "NPCAIController.generated.h"

UCLASS()
class DEMO1_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANPCAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
private:
	class UCrowdFollowingComponent* CrowdFollowingComp;
};
