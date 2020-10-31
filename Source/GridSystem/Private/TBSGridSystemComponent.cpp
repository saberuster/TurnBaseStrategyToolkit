// Copyright saberuster. All Rights Reserved.


#include "TBSGridSystemComponent.h"

#include "TBSGridSystemManager.h"
#include "EngineUtils.h"
#include "TBSGridUnit.h"
#include "UnrealGameplayStatics.h"

// Sets default values for this component's properties
UTBSGridSystemComponent::UTBSGridSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTBSGridSystemComponent::SelectGridUnit(AActor* Actor)
{
	ATBSGridUnit* GridUnit = Cast<ATBSGridUnit>(Actor);
	if (GridUnit)
	{
		if (SelectedUnit)
		{
			UUnrealGameplayStatics::DeSelectActor(SelectedUnit, GetOwner());
		}
		SelectedUnit = GridUnit;
		UUnrealGameplayStatics::SelectActor(SelectedUnit, GetOwner());

		PostGridUnitSelectedNative.Broadcast(GridUnit);
		PostGridUnitSelected.Broadcast(GridUnit);
	}
}

void UTBSGridSystemComponent::SelectGridUnits(TArray<FGridUnitCoordinate> Coordinates)
{
	TArray<ATBSGridUnit*> Units;
	GridSystemManager->GetCoordinateUnits(Coordinates, Units);
	for (ATBSGridUnit* Unit : Units)
	{
	//	Unit->DispatchOnSelected(GetOwner());
	}
}

void UTBSGridSystemComponent::GridUnitNeighbors(ATBSGridUnit* InGridUnit, TArray<ATBSGridUnit*>& OutGridUnits) const
{
	check(GridSystemManager);
	GridSystemManager->GridUnitNeighbors(GridSystemManager->FindGridUnitCoordinate(InGridUnit), OutGridUnits);
}

void UTBSGridSystemComponent::RadiusOverlap(ATBSGridUnit* InGridUnit, float Radius, TArray<ATBSGridUnit*>& HitActors)
{
	FGridUnitCoordinate Coordinate = GridSystemManager->FindGridUnitCoordinate(InGridUnit);
	if (Coordinate.IsValid())
	{
       	FCollisionQueryParams Params(SCENE_QUERY_STAT(GridUnityRadiusTargetingOverlap), false);
		Params.AddIgnoredActor(InGridUnit);
       	Params.bReturnPhysicalMaterial = false;
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_Visibility);
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		TArray<FOverlapResult> Overlaps;
		GetWorld()->OverlapMultiByObjectType(Overlaps, InGridUnit->GetActorLocation(),FQuat::Identity,QueryParams,FCollisionShape::MakeSphere(Radius), Params);

		for (const FOverlapResult& Overlap : Overlaps)
		{
			ATBSGridUnit* OverlapUnit = Cast<ATBSGridUnit>(Overlap.GetActor());
			if (OverlapUnit)
			{
				HitActors.AddUnique(OverlapUnit);
			}
		}
	}

}



// Called when the game starts
void UTBSGridSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	TActorIterator<ATBSGridSystemManager> It(GetWorld());
	GridSystemManager = It ? *It : nullptr;
	checkf(GridSystemManager, TEXT("GridSystemManager Actor must place in world!"));

}


// Called every frame
void UTBSGridSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

