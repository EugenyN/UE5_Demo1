#include "BTDecorator_IsItemHeldByNpc.h"
#include "AIController.h"
#include "Demo1/Actors/NPC.h"

UBTDecorator_IsItemHeldByNpc::UBTDecorator_IsItemHeldByNpc(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Is Item Held By Npc";
}

bool UBTDecorator_IsItemHeldByNpc::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto NPC = Cast<ANPC>(OwnerComp.GetAIOwner()->GetPawn());
	return NPC->HoldingItem != nullptr;
}