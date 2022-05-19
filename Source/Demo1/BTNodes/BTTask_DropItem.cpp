#include "BTTask_DropItem.h"

#include "AIController.h"
#include "Demo1/Actors/D1Character.h"
#include "Demo1/Actors/NPC.h"
#include "Kismet/GameplayStatics.h"


UBTTask_DropItem::UBTTask_DropItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Drop Item";
}

EBTNodeResult::Type UBTTask_DropItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Player = Cast<AD1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player not found on scene"));
		return EBTNodeResult::Failed;
	}

	const auto NPC = Cast<ANPC>(OwnerComp.GetAIOwner()->GetPawn());
	if (NPC->HoldingItem == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("HoldingItem == nullptr !"));
		return EBTNodeResult::Failed;
	}
	
	NPC->DropItem();
	
	return EBTNodeResult::Succeeded;
}
