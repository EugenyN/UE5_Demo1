#include "Demo1/D1GameMode.h"
#include "Demo1/D1HUD.h"
#include <UObject/ConstructorHelpers.h>
#include <EngineUtils.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <NavigationSystem.h>
#include "Actors/NPC.h"


int32 DebugGizmoDraw = 1;
FAutoConsoleVariableRef CVarDebugGizmoDraw(TEXT("demo1.DebugGizmoDraw"), DebugGizmoDraw,
                                      TEXT("Draw debug info demo1.DebugGizmoDraw.\n0: Disable, 1: Enable"),
                                      ECVF_Default);

AD1GameMode::AD1GameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = AD1HUD::StaticClass();

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AD1GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AD1GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (DebugGizmoDraw) {
		DrawDebugCircle(GetWorld(), CircleCenter, GuardCircleRadius, 50, FColor::Yellow,
			false, -1, 0, 0, FVector(0.f, 1.f, 0.f), FVector(1.f, 0.f, 0.f), true);

		for (auto& SP : SurroundPoints)
			DrawDebugSphere(GetWorld(), SP.Value, 10, 26, FColor::Yellow, false, -1);
	}
}

bool AD1GameMode::IsReachablePoint(const FVector& SourcePoint, const FVector& TargetPoint) const
{
	const auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys == nullptr)
		return false;

	FNavLocation ProjectedTargetPoint;
	if (!NavSys->ProjectPointToNavigation(TargetPoint, ProjectedTargetPoint, INVALID_NAVEXTENT))
		return false;

	auto Dist = FVector::Dist2D(TargetPoint, ProjectedTargetPoint.Location);
	const float PermissibleInaccuracy = 40.0f;
	if (Dist > PermissibleInaccuracy)
		return false; // the point is not reachable with the necessary accuracy

	float Lenght = 0;
	return NavSys->GetPathLength(SourcePoint, TargetPoint, Lenght) != ENavigationQueryResult::Type::Fail;
}

TArray<FVector> AD1GameMode::GetSurroundPositions(const FVector& Center, uint8 PositionsCount) const
{
	TArray<FVector> Result;

	for (int32 Idx = 0; Idx < PositionsCount; Idx++)
	{
		float Radian = Idx * PI / (PositionsCount / 2.0f);
		FVector Loc = SurroundCircleRadius * FVector(FMath::Cos(Radian), FMath::Sin(Radian), 0.0f);

		if (IsReachablePoint(Center, Center + Loc))
			Result.Add(Center + Loc);
	}

	return Result;
}

TMap<ANPC*, FVector> AD1GameMode::GetSurroundData(const FVector& Center) const
{
	TArray<ANPC*> FoundNPCs;
	for (TActorIterator<ANPC> Itr(GetWorld()); Itr; ++Itr)
		FoundNPCs.Add(*Itr);

	// move rvalue result to Positions
	auto Positions = GetSurroundPositions(Center, FoundNPCs.Num());

	// sort NPC positions relative to Center. item holder is first.
	FoundNPCs.Sort([Center](const ANPC& A, const ANPC& B)
	{
		if (A.HoldingItem != nullptr)
			return true;
		if (B.HoldingItem != nullptr)
			return false;
		float DistanceA = FVector::DistSquared(Center, A.GetActorLocation());
		float DistanceB = FVector::DistSquared(Center, B.GetActorLocation());
		return DistanceA < DistanceB;
	});

	// take farther NPC from center
	const FVector& FartherLoc = FoundNPCs.Last()->GetActorLocation();

	// sort points by distance from farther NPC
	Positions.Sort([FartherLoc](const FVector& A, const FVector& B)
	{
		float DistanceA = FVector::DistSquared(FartherLoc, A);
		float DistanceB = FVector::DistSquared(FartherLoc, B);
		return DistanceA > DistanceB;
	});

	TMap<ANPC*, FVector> Result;

	for (int32 Idx = 0; Idx < Positions.Num(); Idx++)
	{
		// nearest point for item holder.
		int32 PointIdx = Idx == 0 ? Positions.Num() - 1 : Idx - 1;
		Result.Add(FoundNPCs[Idx], Positions[PointIdx]);
	}

	return Result;
}

void AD1GameMode::UpdateSurroundCircle(const FVector& Center)
{
	FNavLocation ProjectedTargetPoint;
	const auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys != nullptr && !NavSys->ProjectPointToNavigation(Center, ProjectedTargetPoint, INVALID_NAVEXTENT))
	{
		UE_LOG(LogTemp, Warning, TEXT("Circle center projection failed!"));
		return;
	}

	CircleCenter = ProjectedTargetPoint.Location;
	SurroundPoints = GetSurroundData(CircleCenter);
}