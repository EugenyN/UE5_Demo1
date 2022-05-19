#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AreAllNpcAtPoints.generated.h"

/**
 * 
 */
UCLASS()
class DEMO1_API UBTDecorator_AreAllNpcAtPoints : public UBTDecorator
{
	GENERATED_UCLASS_BODY()
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
