#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsPlayerBlocked.generated.h"

struct FBTIsPlayerBlocked
{
	bool bLastRawResult;
};


/**
 * 
 */
UCLASS()
class DEMO1_API UBTDecorator_IsPlayerBlocked : public UBTDecorator_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	typedef FBTIsPlayerBlocked TNodeInstanceMemory;

	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
