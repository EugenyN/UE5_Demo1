#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsItemInCircle.generated.h"

struct FBTIsItemInCircle
{
	bool bLastRawResult;
};

/**
 * 
 */
UCLASS()
class DEMO1_API UBTDecorator_IsItemInCircle : public UBTDecorator_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	typedef FBTIsItemInCircle TNodeInstanceMemory;

	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
	UPROPERTY(EditAnywhere, Category=Blackboard, meta=(AllowPrivateAccess="true"))
	struct FBlackboardKeySelector CircleCenterKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float GuardCircleRadius = 500.0f;
};
