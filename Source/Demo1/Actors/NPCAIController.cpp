#include "NPCAIController.h"

#include <GameFramework/Character.h>
#include <Navigation/CrowdFollowingComponent.h>
#include <NavigationSystem.h>

#include "D1Character.h"
#include "BehaviorTree/BehaviorTree.h"


ANPCAIController::ANPCAIController(const FObjectInitializer& ObjectInitializer)
	: AAIController(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	// Detour UCrowdFollowingComponent used.
	CrowdFollowingComp = FindComponentByClass<UCrowdFollowingComponent>();
	if (CrowdFollowingComp != nullptr)
	{
		CrowdFollowingComp->SetBlockDetection(10, 0.5, 5);
		CrowdFollowingComp->SetCrowdSimulationState(ECrowdSimulationState::ObstacleOnly);
	}
}

ETeamAttitude::Type ANPCAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Cast<AD1Character>(&Other) != nullptr ? ETeamAttitude::Hostile : ETeamAttitude::Neutral;
}