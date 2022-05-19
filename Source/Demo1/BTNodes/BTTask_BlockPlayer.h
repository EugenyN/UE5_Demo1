#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BlockPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DEMO1_API UBTTask_BlockPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY(Category = Node, EditAnywhere)
	uint32 bBlockPlayer : 1;

	virtual FString GetStaticDescription() const override;
public:
	UBTTask_BlockPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
