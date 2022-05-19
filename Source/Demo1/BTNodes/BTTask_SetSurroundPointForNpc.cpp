#include "BTTask_SetSurroundPointForNpc.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Demo1/D1GameMode.h"
#include "Demo1/Actors/NPC.h"
#include "Demo1/Actors/NPCAIController.h"

UBTTask_SetSurroundPointForNpc::UBTTask_SetSurroundPointForNpc(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Set Surround Point For Npc";
	
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetSurroundPointForNpc, BlackboardKey));
}

EBTNodeResult::Type UBTTask_SetSurroundPointForNpc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ANPCAIController* AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr || AIController->GetPawn() == nullptr)
		return EBTNodeResult::Failed;

	const auto GameMode = GetWorld()->GetAuthGameMode<AD1GameMode>();
	const auto NPC = Cast<ANPC>(AIController->GetPawn());
	if (GameMode->SurroundPoints.Contains(NPC))
	{
		const auto Pos = GameMode->SurroundPoints[NPC];
		UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		MyBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Pos);
		return EBTNodeResult::Succeeded;
	}
	
	return  EBTNodeResult::Failed;
}