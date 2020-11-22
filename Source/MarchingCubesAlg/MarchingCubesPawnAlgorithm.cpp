// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesPawnAlgorithm.h"

// Sets default values
AMarchingCubesPawnAlgorithm::AMarchingCubesPawnAlgorithm()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMarchingCubesPawnAlgorithm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMarchingCubesPawnAlgorithm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMarchingCubesPawnAlgorithm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

