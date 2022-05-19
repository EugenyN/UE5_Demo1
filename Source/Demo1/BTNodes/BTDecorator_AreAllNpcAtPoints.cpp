#include "BTDecorator_AreAllNpcAtPoints.h"
#include "Demo1/Actors/NPCAIController.h"

UBTDecorator_AreAllNpcAtPoints::UBTDecorator_AreAllNpcAtPoints(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Are All Npc at Points";
}

bool UBTDecorator_AreAllNpcAtPoints::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	for (TObjectIterator<ANPCAIController> Itr; Itr; ++Itr) {
		if ((*Itr)->IsFollowingAPath())
			return false;
	}

	return true;
}