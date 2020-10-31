// Copyright saberuster. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UGActor.h"
#include "TBSGridUnit.generated.h"


UCLASS()
class GRIDSYSTEM_API ATBSGridUnit : public AUGActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGridUnit();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
