// Copyright saberuster. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TBSGridUnit.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGridUnitSelectedNative);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridUnitSelected);
DECLARE_MULTICAST_DELEGATE(FOnGridUnitDeSelectedNative);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridUnitDeSelected);


UCLASS()
class GRIDSYSTEM_API ATBSGridUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGridUnit();
	
	UPROPERTY(BlueprintAssignable, Category="GridSystem")
   	FOnGridUnitSelected OnGridUnitSelected;
   
   	UPROPERTY(BlueprintAssignable, Category="GridSystem")
   	FOnGridUnitDeSelected OnGridUnitDeSelected;
    
  	FOnGridUnitSelectedNative OnGridUnitSelectedNative;
   	FOnGridUnitDeSelectedNative OnGridUnitDeSelectedNative;


	void SelectGridUnit();

	void DeSelectGridUnit();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
