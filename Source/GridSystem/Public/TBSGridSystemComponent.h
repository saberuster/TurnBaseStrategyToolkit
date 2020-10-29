// Copyright saberuster. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TBSGridSystemTypes.h"
#include "Components/ActorComponent.h"
#include "TBSGridSystemComponent.generated.h"

class ATBSGridUnit;
class ATBSGridSystemManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostGridUnitSelected, ATBSGridUnit*, GridUnit);
DECLARE_MULTICAST_DELEGATE_OneParam(FPostGridUnitSelectedNative, ATBSGridUnit*);

UCLASS( ClassGroup=(GridSystem), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UTBSGridSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTBSGridSystemComponent();

	UFUNCTION(BlueprintCallable, Category="GridSystem")
	void SelectGridUnit(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="GridSystem")
	void SelectGridUnits(TArray<FGridUnitCoordinate> Coordinates);


	UFUNCTION(BlueprintPure, Category="GridSystem")
	void GridUnitNeighbors(ATBSGridUnit* InGridUnit, TArray<ATBSGridUnit*>& OutGridUnits) const;

	UFUNCTION(BlueprintCallable, Category="GridSystem")
	void RadiusOverlap(ATBSGridUnit* InGridUnit, float Radius, TArray<ATBSGridUnit*>& HitActors);

	UPROPERTY(BlueprintAssignable, Category="GridSystem")
	FPostGridUnitSelected PostGridUnitSelected;

	FPostGridUnitSelectedNative PostGridUnitSelectedNative;

protected:

	UPROPERTY(BlueprintReadOnly)
	ATBSGridUnit* SelectedUnit;

	UPROPERTY(BlueprintReadOnly)
	ATBSGridSystemManager* GridSystemManager;

	
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
