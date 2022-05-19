#include "BTDecorator_IsItemInCircle.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Demo1/Actors/PickupBall.h"

UBTDecorator_IsItemInCircle::UBTDecorator_IsItemInCircle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Is Item in Circle";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
	
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsItemInCircle, BlackboardKey), APickupBall::StaticClass());
}

bool UBTDecorator_IsItemInCircle::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
																 uint8* NodeMemory) const
{
	return CalcConditionImpl(OwnerComp, NodeMemory);
}

void UBTDecorator_IsItemInCircle::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcConditionImpl(OwnerComp, NodeMemory);
}

FORCEINLINE bool UBTDecorator_IsItemInCircle::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	const AActor* PickupItem = Cast<AActor>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (PickupItem == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PickupItem is null"));
		return false;
	}

	const FVector CircleCenter = MyBlackboard->GetValueAsVector(CircleCenterKey.SelectedKeyName);
	return FVector::DistXY(PickupItem->GetActorLocation(), CircleCenter) < GuardCircleRadius;
}

void UBTDecorator_IsItemInCircle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcConditionImpl(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

uint16 UBTDecorator_IsItemInCircle::GetInstanceMemorySize() const
{
	return sizeof(TNodeInstanceMemory);
}