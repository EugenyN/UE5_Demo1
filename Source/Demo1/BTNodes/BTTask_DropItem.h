#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DropItem.generated.h"

/**
 * 
 */
UCLASS()
class DEMO1_API UBTTask_DropItem : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_DropItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
