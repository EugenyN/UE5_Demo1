#include "BTDecorator_IsPlayerBlocked.h"

#include "AIController.h"
#include "Demo1/Actors/D1Character.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_IsPlayerBlocked::UBTDecorator_IsPlayerBlocked(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Is Player Blocked";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

bool UBTDecorator_IsPlayerBlocked::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
																 uint8* NodeMemory) const
{
	return CalcConditionImpl(OwnerComp, NodeMemory);
}

void UBTDecorator_IsPlayerBlocked::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalcConditionImpl(OwnerComp, NodeMemory);
}

FORCEINLINE bool UBTDecorator_IsPlayerBlocked::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Player = Cast<AD1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player not found on scene"));
		return false;
	}

	return Player->IsBlocked();
}

void UBTDecorator_IsPlayerBlocked::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
	
	const bool bResult = CalcConditionImpl(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

uint16 UBTDecorator_IsPlayerBlocked::GetInstanceMemorySize() const
{
	return sizeof(TNodeInstanceMemory);
}