#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetSurroundPointForNpc.generated.h"

/**
 * 
 */
UCLASS()
class DEMO1_API UBTTask_SetSurroundPointForNpc : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
