#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateSurroundCircle.generated.h"

/**
 * 
 */
UCLASS()
class DEMO1_API UBTTask_UpdateSurroundCircle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_UpdateSurroundCircle(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category=Blackboard, meta=(AllowPrivateAccess="true"))
	struct FBlackboardKeySelector CircleCenterKey;
};
