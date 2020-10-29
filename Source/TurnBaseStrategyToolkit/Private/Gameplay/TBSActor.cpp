// Copyright saberuster. All Rights Reserved.


#include "Gameplay/TBSActor.h"

// Sets default values
ATBSActor::ATBSActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

