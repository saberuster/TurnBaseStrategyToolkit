// Copyright saberuster. All Rights Reserved.


#include "Gameplay/TBSPlayerPawn.h"

// Sets default values
ATBSPlayerPawn::ATBSPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATBSPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

