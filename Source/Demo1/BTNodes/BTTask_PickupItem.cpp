#include "BTTask_PickupItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Demo1/Actors/D1Character.h"
#include "Demo1/Actors/NPC.h"
#include "Demo1/Actors/PickupBall.h"
#include "Kismet/GameplayStatics.h"

class AD1GameMode;

UBTTask_PickupItem::UBTTask_PickupItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Pickup Item";

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PickupItem, BlackboardKey), APickupBall::StaticClass());
}

EBTNodeResult::Type UBTTask_PickupItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	const auto Item = Cast<APickupBall>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));

	const auto NPC = Cast<ANPC>(OwnerComp.GetAIOwner()->GetPawn());
	NPC->PickupItem(Item);

	const auto Player = Cast<AD1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player not found on scene"));
		return EBTNodeResult::Failed;
	}
	
	if (Player->HoldingItem != nullptr)
		Player->HoldingItem = nullptr;
	
	return EBTNodeResult::Succeeded;
}
