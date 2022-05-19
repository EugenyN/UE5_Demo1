#include "BTTask_UpdateSurroundCircle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Demo1/D1GameMode.h"
#include "Demo1/Actors/D1Character.h"
#include "Kismet/GameplayStatics.h"


UBTTask_UpdateSurroundCircle::UBTTask_UpdateSurroundCircle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Surround Circle";
	
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_UpdateSurroundCircle, BlackboardKey));
}

EBTNodeResult::Type UBTTask_UpdateSurroundCircle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Player = Cast<AD1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player not found on scene"));
		return EBTNodeResult::Failed;
	}

	const auto GameMode = GetWorld()->GetAuthGameMode<AD1GameMode>();
	GameMode->UpdateSurroundCircle(Player->GetActorLocation());
	
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	MyBlackboard->SetValueAsVector(CircleCenterKey.SelectedKeyName, GameMode->CircleCenter);
	return EBTNodeResult::Succeeded;
}
