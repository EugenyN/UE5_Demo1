#include "BTTask_BlockPlayer.h"
#include "AIController.h"
#include "Demo1/Actors/D1Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BlockPlayer::UBTTask_BlockPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Block Player";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bBlockPlayer = true;
}

EBTNodeResult::Type UBTTask_BlockPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Player = Cast<AD1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Player not found on scene"));
		return EBTNodeResult::Failed;
	}

	Player->Block(bBlockPlayer);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_BlockPlayer::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), bBlockPlayer ? TEXT("True") : TEXT("False"));
}
